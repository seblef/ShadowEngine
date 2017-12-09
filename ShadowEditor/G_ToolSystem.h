
#ifndef _G_TOOLSYSTEM_H_
#define _G_TOOLSYSTEM_H_

#include <Core.h>
#include "G_Tool.h"

using namespace Core;

enum GBaseTool
{
	GBT_NULL=0,
	GBT_CAMROTATE,
	GBT_CAMTRANSLATE,
	GBT_CREATE,
	GBT_TRANSLATE,
	GBT_ROTATE,
	GBT_HEIGHT,
	GBT_ROTATEZ,
	GBT_PASTE,

	GBT_COUNT
};

class G_ToolSystem : public TSingleton<G_ToolSystem>
{
protected:

	G_Tool*					_currentTool;
	unsigned short			_currentFlags;

	G_Tool*					_baseTools[GBT_COUNT];

public:

	G_ToolSystem();
	~G_ToolSystem();

	void					setFlags(unsigned short f)				{ _currentFlags|=f; }
	void					unsetFlags(unsigned short f)			{ _currentFlags&= ~f; }
	unsigned short			getFlags(unsigned short f) const		{ return _currentFlags & f; }

	void					setCurrentTool(G_Tool* t)
	{
		_currentTool->end();
		_currentTool=t;
		_currentTool->begin();
	}
	void					setCurrentTool(GBaseTool t)
	{
		setCurrentTool(_baseTools[t]);
	}

	G_Tool*					getCurrentTool() const					{ return _currentTool; }
	bool					isCurrentToolNull() const				{ return _currentTool==_baseTools[GBT_NULL]; }

	void					mouseDown(int x, int y) const			{ _currentTool->mouseDown(x,y,_currentFlags); }
	void					mouseUp(int x, int y) const				{ _currentTool->mouseUp(x,y); }
	void					mouseMove(int x, int y) const			{ _currentTool->mouseMove(x,y); }

	void					cancel() const							{ _currentTool->cancel(); }

	void					draw(G_EditorObjDrawer* d)				{ _currentTool->draw(d); }
};

#endif