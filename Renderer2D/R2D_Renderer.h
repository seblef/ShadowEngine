
#ifndef _R2D_RENDERER_H_
#define _R2D_RENDERER_H_

#include "R2D_Object.h"

#define R2D_MAX_OBJECTS						256

class R2D_Renderer
{
protected:

	typedef vector<const R2D_Object*>	ObjectVector;
	ObjectVector						_objects;

	bool								_relativePosition;
	Vx2D*								_verts;
	IVideoDevice*						_device;
	IVertexBuffer*						_vBuffer;
	IIndexBuffer*						_iBuffer;
	ITexture*							_whiteTexture;
	Vector2								_scale;
	PrimitiveType						_primitiveType;

	virtual void						sortObjects()						{}
	virtual void						renderObjects()=0;

	void								scaleVertex(float x, float y, Vx2D& vout) const
	{
		vout.pos.x=x*_scale.x - 1.0f;
		vout.pos.y=y*_scale.y - 1.0f;
		vout.pos.z=0.5f;
	}
	void								scaleVertex(const Vector2& vin, Vx2D& vout) const		{ return scaleVertex(vin.x,vin.y,vout); }

	void								makeRectVertices(const Vector2& p, const Vector2& s, const Color& c, Vx2D *v) const
	{
		scaleVertex(p.x,p.y,v[0]);
		scaleVertex(p.x+s.x,p.y,v[1]);
		scaleVertex(p.x+s.x,p.y+s.y,v[2]);
		scaleVertex(p.x,p.y + s.y,v[3]);

		for(int i=0;i<4;++i)
			v[i].color=c;
	}
	void								makeRectVertices(const Vector2& p, const Vector2& s, const Vector2& uvStart, const Vector2& uvEnd, const Color& c, Vx2D *v) const
	{
		makeRectVertices(p,s,c,v);
		v[0].uv.x=uvStart.x;					v[0].uv.y=uvStart.y;
		v[1].uv.x=uvEnd.x;						v[1].uv.y=uvStart.y;
		v[2].uv.x=uvEnd.x;						v[2].uv.y=uvEnd.y;
		v[3].uv.x=uvStart.x;					v[3].uv.y=uvEnd.y;
	}

	void								makeRectVertices(const R2D_Object& o, Vx2D* v) const	{ makeRectVertices(o.getPosition(),o.getSize(),o.getColor(),v); }
	void								makeRectVertices(const R2D_Object& o, const Vector2& uvStart, const Vector2& uvEnd, Vx2D* v) const
	{
		makeRectVertices(o.getPosition(),o.getSize(),uvStart,uvEnd,o.getColor(),v);
	}
	void								makeLineVertices(const R2D_Object& o, Vx2D *v) const
	{
		scaleVertex(o.getPosition(),v[0]);
		scaleVertex(o.getPosition() + o.getSize(),v[1]);
		v[0].color=o.getColor();
		v[1].color=o.getColor();
	}

public:

	R2D_Renderer(R2D_Object::Type type, bool relativePos, ITexture* white, IVideoDevice* device);
	virtual ~R2D_Renderer();

	void								addObject(const R2D_Object* obj)				{ _objects.push_back(obj); }
	void								execute();
};

#endif