
#ifndef _G_EDITORDLGFACTORY_H_
#define _G_EDITORDLGFACTORY_H_

#include "EditDlg.h"
#include "G_Object.h"

class G_EditorDlgFactory
{
public:

	G_EditorDlgFactory()					{}

	static CEditDlg*						createEditorDlg(G_Object *o, bool ground, CWnd* parent);
};

#endif