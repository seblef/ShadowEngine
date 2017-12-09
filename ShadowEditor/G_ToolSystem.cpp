
#include "stdafx.h"
#include "G_ToolSystem.h"
#include "G_BaseToolNull.h"
#include "G_BaseToolCamRotate.h"
#include "G_BaseToolCamTranslate.h"
#include "G_BaseToolCreate.h"
#include "G_BaseToolRotate.h"
#include "G_BaseToolRotateZ.h"
#include "G_BaseToolHeight.h"
#include "G_BaseToolPaste.h"


G_ToolSystem::G_ToolSystem() : _currentFlags(0)
{
	_baseTools[GBT_NULL]=new G_BaseToolNull;
	_baseTools[GBT_CAMROTATE]=new G_BaseToolCamRotate;
	_baseTools[GBT_CAMTRANSLATE]=new G_BaseToolCamTranslate;
	_baseTools[GBT_CREATE]=new G_BaseToolCreate;
	_baseTools[GBT_TRANSLATE]=new G_BaseToolTranslate;
	_baseTools[GBT_ROTATE]=new G_BaseToolRotate;
	_baseTools[GBT_HEIGHT]=new G_BaseToolHeight;
	_baseTools[GBT_ROTATEZ]=new G_BaseToolRotateZ;
	_baseTools[GBT_PASTE]=new G_BaseToolPaste;

	_currentTool=_baseTools[GBT_NULL];
	_currentTool->begin();
}

G_ToolSystem::~G_ToolSystem()
{
	for(int i=0;i<GBT_COUNT;++i)
		delete _baseTools[i];
}