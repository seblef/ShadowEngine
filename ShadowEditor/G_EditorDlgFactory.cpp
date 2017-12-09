
#include "stdafx.h"
#include "resource.h"
#include "G_EditorDlgFactory.h"
#include "G_Light.h"
#include "GroundEditDlg.h"
#include "NullEditDlg.h"
#include "LightEditDlg.h"


CEditDlg* G_EditorDlgFactory::createEditorDlg(G_Object* o, bool ground, CWnd *parent)
{
	if(ground)
		return new CGroundEditDlg(parent);
	else if(o)
	{
		switch(o->getType())
		{
		case G_Object::LIGHT:		return new CLightEditDlg(&((G_Light*)o)->getLight(),parent);		break;
		};
	}

	return new CNullEditDlg(parent);
}