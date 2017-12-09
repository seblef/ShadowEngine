
#ifndef _QUADTREENODE_H_
#define _QUADTREENODE_H_

#include "Renderable.h"
#include <set>

using namespace std;

class QuadTreeNode
{
protected:

	int					_id;
	int					_x,_y,_size;
	int					_parent;
	int					_childrenCount;
	int					_children[4];

	typedef set<Renderable*>		RenderableSet;
	RenderableSet		_renderables;

public:

	QuadTreeNode(int id, int x, int y, int size, int parent) : _id(id), _x(x), _y(y),
		_childrenCount(0), _parent(parent), _size(size)			{}
	~QuadTreeNode()							{}

	int					getID() const				{ return _id; };
	int					getX() const				{ return _x; }
	int					getY() const				{ return _y; }
	int					getSize() const				{ return _size; }

	int					getChildrenCount() const	{ return _childrenCount; }
	int					getChild(int n) const		{ return _children[n]; }
	
	void				addChild(int c)				{ _children[_childrenCount++]=c; }
	
	void				addRenderable(Renderable*r)	{ _renderables.insert(r); r->setVisibilityID(_id); }
	void				remRenderable(Renderable*r)	{ _renderables.erase(r); }
	void				enqueueRenderables()
	{
		RenderableSet::iterator r(_renderables.begin());
		for(;r!=_renderables.end();++r)
			(*r)->enqueue();
	}

	const set<Renderable*>&	getRenderables() const	{ return _renderables; }
};

#endif