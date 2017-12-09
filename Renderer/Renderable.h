
#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include <Core.h>

using namespace Core;

class Renderable
{
protected:

	unsigned long			_lastFrame;
	Matrix4					_worldMatrix;
	BBox3					_localBBox;
	BBox3					_worldBBox;
	bool					_alwaysVisible;
	int						_visID;

public:

	Renderable(const BBox3& local, const Matrix4& world, bool alwaysVisible) : _lastFrame(0xffffffff), _worldMatrix(world),
		_localBBox(local), _alwaysVisible(alwaysVisible), _visID(-1)
	{
		_worldBBox.transformFromBox(local,world);
	}
	Renderable(const Renderable* r) : _lastFrame(0xffffffff), _worldMatrix(r->_worldMatrix),
		_localBBox(r->_localBBox), _alwaysVisible(r->_alwaysVisible), _worldBBox(r->_worldBBox), _visID(-1)		{}

	virtual ~Renderable()															{}

	void					setLastFrame(unsigned long lastFrame)					{ _lastFrame=lastFrame; }
	unsigned long			getLastFrame() const									{ return _lastFrame; }

	virtual void			setWorldMatrix(const Matrix4& world);
	const Matrix4&			getWorldMatrix() const									{ return _worldMatrix; }

	const BBox3&			getLocalBBox() const									{ return _localBBox; }
	const BBox3&			getWorldBBox() const									{ return _worldBBox; }

	bool					isAlwaysVisible() const									{ return _alwaysVisible; }
	void					setVisibilityID(int id)									{ _visID=id; }
	int						getVisibilityID() const									{ return _visID; }

	virtual void			enqueue()=0;
};

#endif