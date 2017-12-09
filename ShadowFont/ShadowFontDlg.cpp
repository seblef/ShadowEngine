
// ShadowFontDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShadowFont.h"
#include "ShadowFontDlg.h"
#include "afxdialogex.h"
#include "../Renderer2D/FontFileFormat.h"
#include "../Renderer2D/R2D_FontCharArea.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CShadowFontDlg dialog

const int		g_FontSizes[]={4,6,8,9,10,11,12,14,16,18,20,22,24,26,28,36,48,56,68,72,0};
const int		g_TexSizes[]={64,128,256,512,1024,2048};

CShadowFontDlg::CShadowFontDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShadowFontDlg::IDD, pParent)
	, _name(_T("Font"))
	, _texSize(2)
	, _bold(FALSE)
	, _italic(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShadowFontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, _name);
	DDX_CBIndex(pDX, IDC_TEXSIZE, _texSize);
	DDX_Control(pDX, IDC_FONTNAME, _fontNames);
	DDX_Control(pDX, IDC_FONTSIZE, _fontSize);
	DDX_Control(pDX, IDC_PREVIEW, _preview);
	DDX_Check(pDX, IDC_BOLD, _bold);
	DDX_Check(pDX, IDC_ITALIC, _italic);
}

BEGIN_MESSAGE_MAP(CShadowFontDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_SELCHANGE(IDC_FONTNAME, &CShadowFontDlg::OnLbnSelchangeFontname)
	ON_LBN_SELCHANGE(IDC_FONTSIZE, &CShadowFontDlg::OnLbnSelchangeFontsize)
	ON_BN_CLICKED(IDC_APPLY, &CShadowFontDlg::OnBnClickedApply)
END_MESSAGE_MAP()


// CShadowFontDlg message handlers

BOOL CShadowFontDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	fillFontList();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CShadowFontDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CShadowFontDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CShadowFontDlg::OnLbnSelchangeFontname()
{
	preview();
}


void CShadowFontDlg::OnLbnSelchangeFontsize()
{
	preview();
}

inline int getTextureSizeFromSurfaceSize(int size)
{
	int ts = 0x01;
	while(ts < size)
		ts <<= 1;

	return ts;
}

void CShadowFontDlg::OnBnClickedApply()
{
	UpdateData(TRUE);

	int surfSize=g_TexSizes[_texSize];
	int sizeSel=_fontSize.GetCurSel();
	int fontSel=_fontNames.GetCurSel();

	if(fontSel==-1 || sizeSel==-1)
		return;

	R2D_FontCharArea* table=new R2D_FontCharArea[256];
	wchar_t textString[257];

	for(int i=0;i<256;++i)
		textString[i]=i;
	textString[256]=0;

	HDC dc=::GetDC(_preview.m_hWnd);
	int fontSize=g_FontSizes[sizeSel];
	char fontName[64];
	_fontNames.GetText(fontSel,fontName);

	HFONT font = CreateFont(
			-MulDiv(fontSize, GetDeviceCaps(dc, LOGPIXELSY), 72),
			0,0,0,
			_bold ? FW_BOLD : 0,
			_italic,
			0,0,0,0,0,0,0,
			fontName);

	HGDIOBJ oldfont = SelectObject(dc, font);

	SIZE size;
	size.cx = 0;
	size.cy = 0;
	int posx = 0;
	int posy = 0;

	for(int i=0; i<256; ++i)
	{
		GetTextExtentPoint32W(dc, &textString[i], 1, &size);
		++size.cx;

		if (posx + size.cx > surfSize)
		{
			posx = 0;
			posy += size.cy+1;
		}

		table[i].setPosition(Vector2((float)posx,(float)posy));
		table[i].setSize(Vector2((float)size.cx,(float)size.cy));

		posx += size.cx;
	}

	int surfaceHeight=(int)(table[255].getPosition().y + table[255].getSize().y + 1.0f);
	surfaceHeight=getTextureSizeFromSurfaceSize(surfaceHeight);

	float inv_w=1.0f / (float)surfSize;
	float inv_h=1.0f / (float)surfaceHeight;

	for(int i=0;i<256;++i)
	{
		table[i].setUVCoords(BBox2(
			Vector2(table[i].getPosition().x * inv_w, table[i].getPosition().y * inv_h),
			Vector2(
			(table[i].getPosition().x + table[i].getSize().x)*inv_w,
			(table[i].getPosition().y + table[i].getSize().y)*inv_h)));
	}

	// draw characters

	HBITMAP bmp = CreateCompatibleBitmap(dc, surfSize, surfaceHeight);
	HDC bmpdc = CreateCompatibleDC(dc);

	LOGBRUSH lbrush;
	lbrush.lbColor = RGB(0,0,0);
	lbrush.lbHatch = 0;
	lbrush.lbStyle = BS_SOLID;
	
	HBRUSH brush = CreateBrushIndirect(&lbrush);
	HPEN pen = CreatePen(PS_NULL, 0, 0);

	HGDIOBJ oldbmp = SelectObject(bmpdc, bmp);
	HGDIOBJ oldbmppen = SelectObject(bmpdc, pen);
	HGDIOBJ oldbmpbrush = SelectObject(bmpdc, brush);
	HGDIOBJ oldbmpfont = SelectObject(bmpdc, font);

	SetTextColor(bmpdc, RGB(255,255,255));

	Rectangle(bmpdc, 0,0,surfSize,surfaceHeight);
	SetBkMode(bmpdc, TRANSPARENT);

	textString[1]=0;
	R2D_FontCharArea *a;
	for(int i=0; i<256; ++i)
	{
		a=&table[i];
		textString[0]=i;

		TextOutW(bmpdc, (int)a->getPosition().x, (int)a->getPosition().y, textString, 1);
	}

	// copy to clipboard

	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_BITMAP, bmp);
	CloseClipboard();	
	
	SelectObject(bmpdc, oldbmp);
	SelectObject(bmpdc, oldbmppen);
	SelectObject(bmpdc, oldbmpbrush);
	SelectObject(bmpdc, oldbmpfont);

	SelectObject(dc, oldfont);
	::ReleaseDC(_preview.m_hWnd, dc);

	DeleteDC(bmpdc);
	DeleteObject(font);
	DeleteObject(brush);
	DeleteObject(pen);
	DeleteObject(bmp);

	saveTable(table);
}

void CShadowFontDlg::saveTable(const R2D_FontCharArea* table) const
{
	string filebase("../2D/");
	filebase+=_name;
	filebase+=".sfn";

	string texFile("2D/");
	texFile+=_name;
	texFile+=".png";

	ofstream ofs(filebase,fstream::out | fstream::binary);
	if(ofs.good())
	{
		File::FontFileHeader h;
		h._header=FONT_HEADER;
		strcpy(h._texFile,texFile.c_str());

		ofs.write((const char*)&h,sizeof(File::FontFileHeader));
		ofs.write((const char*)table,sizeof(R2D_FontCharArea) * 256);
	}
}

int CALLBACK enumFontCallBack(CONST LOGFONT *lplf, CONST TEXTMETRIC *lptm, DWORD FontType, LPARAM lpData) 
{ 
	((CShadowFontDlg*)lpData)->_fontNames.AddString(lplf->lfFaceName);
	return 1;
}

void CShadowFontDlg::fillFontList()
{
	_fontNames.ResetContent();
	HDC hdc=::GetDC(this->m_hWnd);

	EnumFonts(hdc,0,&enumFontCallBack,(LPARAM)this);

	::ReleaseDC(this->m_hWnd,hdc);

	_fontSize.ResetContent();
	
	char buf[128];
	for (int i=0; g_FontSizes[i] != 0; ++i)
		_fontSize.AddString(_itoa(g_FontSizes[i], buf, 10));

	_fontSize.SetCurSel(4);
}

void CShadowFontDlg::preview()
{
	UpdateData(TRUE);

	int sizeSel=_fontSize.GetCurSel();
	int fontSel=_fontNames.GetCurSel();

	if(sizeSel!=-1 && fontSel!=-1)
	{
		int fontSize=g_FontSizes[sizeSel];
		char fontName[64];

		_fontNames.GetText(fontSel,fontName);

		HDC hdc=::GetDC(this->m_hWnd);
		HFONT font=CreateFont(-MulDiv(fontSize,GetDeviceCaps(hdc,LOGPIXELSX),72),
			0,0,0,
			_bold ? FW_BOLD : 0,
			_italic,
			0,0,0,0,0,0,0,fontName);

		::ReleaseDC(this->m_hWnd,hdc);

		RECT r;
		_preview.GetClientRect(&r);

		LOGBRUSH lbrush;
		lbrush.lbColor = GetSysColor(COLOR_3DFACE);
		lbrush.lbHatch = 0;
		lbrush.lbStyle = BS_SOLID;
		
		HBRUSH brush = CreateBrushIndirect(&lbrush);
		HPEN pen = CreatePen(PS_NULL, 0, 0);

		hdc=::GetDC(_preview.m_hWnd);

		HGDIOBJ oldfont = SelectObject(hdc, font);
		HGDIOBJ oldpen = SelectObject(hdc, pen);
		HGDIOBJ oldbrush = SelectObject(hdc, brush);

		const char *text = "ABCDEF abcdef 12345";

		SetBkMode(hdc, OPAQUE);
		SetBkColor(hdc, GetSysColor(COLOR_3DFACE));
		Rectangle(hdc, 0,0, r.right, r.bottom);

		DrawText(hdc, text, strlen(text), &r, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

		SelectObject(hdc, oldfont);
		SelectObject(hdc, oldpen);
		SelectObject(hdc, oldbrush);

		::ReleaseDC(_preview.m_hWnd, hdc);

		DeleteObject(font);
		DeleteObject(brush);
		DeleteObject(pen);
	}
}