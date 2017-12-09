
#ifndef _VISIBILITYQUAD_H_
#define _VISIBILITYQUAD_H_

#include "IVisibilitySystem.h"
#include "QuadTreeNode.h"
#include <vector>

class VisibilityQuad : public IVisibilitySystem
{
protected:

	int						_minSize;
	int						_width, _height;
	typedef vector<QuadTreeNode*> NodeVector;
	NodeVector				_nodes;

	int						getNearestPowerOfTwo(int n) const
	{
		int r=1;
		while(n > (1 << r))
			++r;

		return 1 << r;
	}
	void					clearNodes();
	void					recursVisibility(const SRect& r, int id);
	void					recursCreate(int parent_id, int x, int y, int size);

	void					getRenderableBounds(const Renderable* r, SRect& rect) const
	{
		const BBox3& b(r->getWorldBBox());
		rect.x1=(int)(b.getMin().x);
		rect.x2=(int)(b.getMax().x);
		rect.y1=(int)(b.getMin().z);
		rect.y2=(int)(b.getMax().z);
	}
	int						findBestNode(const SRect& r) const;
	bool					isIn(const QuadTreeNode* n,const SRect& r) const
	{
		return (r.x1 >= n->getX() && r.x2 <= n->getX() + n->getSize() &&
				r.y1 >= n->getY() && r.y2 <= n->getY() + n->getSize());
	}

public:

	VisibilityQuad(int min_size);
	~VisibilityQuad()					{ clearNodes(); }

	void					create(int w, int h);
	void					onResize(int w, int h);

	void					addRenderable(Renderable* r);
	void					remRenderable(Renderable* r);
	void					visibility(const ViewFrustum& vf, const SRect& bounds)				{ recursVisibility(bounds,0); }

	void					onRenderableMoved(Renderable *r);
};

#endif