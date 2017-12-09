
#include "stdafx.h"
#include "R2D_StdFrameLoader.h"
#include "R2D_FontLoader.h"
#include "R2D_Line.h"
#include "R2D_Image.h"
#include "R2D_Rect.h"
#include "R2D_Text.h"
#include "Renderer2D.h"


R2D_Frame* R2D_StdFrameLoader::loadFrame(const string& frameFile, IVideoDevice* d)
{
	ScriptFile sf(frameFile);
	if(!sf.good())
		return 0;

	if(sf.getToken()!=_T("frame"))
		return 0;

	string frame_name(sf.getToken());
	R2D_Frame* f=new R2D_Frame;

	string t(sf.getToken());
	while(sf.good() && t!=_T("end_frame"))
	{
		if(t==_T("texture"))			parseTexture(sf,d);
		else if(t==_T("font"))			parseFont(sf,d);
		else if(t==_T("layer"))			parseLayer(sf,*f,d);

		t=sf.getToken();
	}

	return f;
}

void R2D_StdFrameLoader::parseTexture(ScriptFile& sf, IVideoDevice* d) const
{
	string texName(sf.getToken());

	if(Renderer2D::getSingletonRef().getTextureDB().getData(texName))
		sf.getToken();		// texture filename
	else
	{
		ITexture* t=d->createTexture(sf.getToken());
		Renderer2D::getSingletonRef().getTextureDB().registerData(texName,t);
	}
}

void R2D_StdFrameLoader::parseFont(ScriptFile& sf, IVideoDevice* d) const
{
	string fontName(sf.getToken());

	if(Renderer2D::getSingletonRef().getTextureDB().getData(fontName))
		sf.getToken();		// font filename
	else
	{
		R2D_Font* f=R2D_FontLoader::loadFont(sf.getToken(),d);
		Renderer2D::getSingletonRef().getFontDB().registerData(fontName,f);
	}
}

void R2D_StdFrameLoader::parseLayer(ScriptFile& sf, R2D_Frame& f, IVideoDevice* d) const
{
	R2D_Layer *l=new R2D_Layer;
	f.getVector().push_back(l);

	string t(sf.getToken());
	while(sf.good() && t!=_T("end_layer"))
	{
		if(t==_T("line"))			parseLine(sf,*l);
		else if(t==_T("rect"))		parseRect(sf,*l);
		else if(t==_T("image"))		parseImage(sf,*l);
		else if(t==_T("text"))		parseText(sf,*l);
		t=sf.getToken();
	}
}

void R2D_StdFrameLoader::parseLine(ScriptFile& sf, R2D_Layer& l) const
{
	Vector2 p(Vector2::NullVector);
	Vector2 s(Vector2::UnitVector);
	Color c(Color::White);

	string t(sf.getToken());
	while(sf.good() && t!=_T("end_line"))
	{
		if(t==_T("position"))			sf.parseVector(p);
		else if(t==_T("size"))			sf.parseVector(s);
		else if(t==_T("color"))			sf.parseColor(c);

		t=sf.getToken();
	}

	l.getSet().insert(new R2D_Line(p,s,c));
}

void R2D_StdFrameLoader::parseRect(ScriptFile& sf, R2D_Layer& l) const
{
	Vector2 p(Vector2::NullVector);
	Vector2 s(Vector2::UnitVector);
	Color c(Color::White);

	string t(sf.getToken());
	while(sf.good() && t!=_T("end_rect"))
	{
		if(t==_T("position"))			sf.parseVector(p);
		else if(t==_T("size"))			sf.parseVector(s);
		else if(t==_T("color"))			sf.parseColor(c);

		t=sf.getToken();
	}

	l.getSet().insert(new R2D_Rect(p,s,c));
}

void R2D_StdFrameLoader::parseImage(ScriptFile& sf, R2D_Layer& l) const
{
	Vector2 p(Vector2::NullVector);
	Vector2 s(Vector2::UnitVector);
	Vector2 uv_start(Vector2::NullVector);
	Vector2 uv_end(Vector2::NullVector);
	Color c(Color::White);
	ITexture* tex=0;

	string t(sf.getToken());
	while(sf.good() && t!=_T("end_image"))
	{
		if(t==_T("position"))			sf.parseVector(p);
		else if(t==_T("size"))			sf.parseVector(s);
		else if(t==_T("color"))			sf.parseColor(c);
		else if(t==_T("uv_start"))		sf.parseVector(uv_start);
		else if(t==_T("uv_end"))		sf.parseVector(uv_end);
		else if(t==_T("texture"))		tex=Renderer2D::getSingletonRef().getTextureDB().getData(sf.getToken());

		t=sf.getToken();
	}

	l.getSet().insert(new R2D_Image(p,s,c,uv_start,uv_end-uv_start,tex,false));
}

void R2D_StdFrameLoader::parseText(ScriptFile& sf, R2D_Layer& l) const
{
	Vector2 p(Vector2::NullVector);
	Color c(Color::White);
	R2D_Font* f=0;
	string text;

	string t(sf.getToken());
	while(sf.good() && t!=_T("end_text"))
	{
		if(t==_T("position"))			sf.parseVector(p);
		else if(t==_T("color"))			sf.parseColor(c);
		else if(t==_T("font"))			f=Renderer2D::getSingletonRef().getFontDB().getData(sf.getToken());
		else if(t==_T("text"))			text=sf.getToken();

		t=sf.getToken();
	}

	l.getSet().insert(new R2D_Text(f,text,p,Vector2::UnitVector,c));
}
