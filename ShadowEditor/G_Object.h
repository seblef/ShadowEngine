
#ifndef _G_OBJECT_H_
#define _G_OBJECT_H_

#include "G_SelectionItem.h"
#include "G_EditorObjDrawer.h"

class G_Object
{
public:

	enum Type
	{
		MESH=0,
		ACTOR,
		LIGHT,
		PARTICLESYSTEM,
		SOUND,
		ENTITY,

		COUNT
	};

protected:

	Type					_type;

	Vector3					_pos;
	Vector3					_rot;

	Matrix4					_world;
	BBox3					_localBox;

	G_SelectionItem*		_selItem;

	virtual void			rebuildMatrix();

public:

	G_Object(Type type) : _type(type), _pos(Vector3::NullVector), _rot(Vector3::NullVector), _world(Matrix4::Identity),
		_selItem(0), _localBox(BBox3::NullBox)		{}
	G_Object(const G_Object* o) : _type(o->_type), _pos(o->_pos), _rot(o->_rot), _world(o->_world),
		_localBox(o->_localBox)						{}
	virtual ~G_Object()
	{
		if(_selItem)		delete _selItem;
	}

	Type					getType() const									{ return _type; }

	void					setPosition(const Vector3& p)					{ _pos=p; rebuildMatrix(); }
	void					translate(const Vector3& p)						{ _pos+=p; rebuildMatrix(); }
	const Vector3&			getPosition() const								{ return _pos; }

	void					setRotation(const Vector3& r)					{ _rot=r; rebuildMatrix(); }
	void					rotate(const Vector3& r)						{ _rot+=r; rebuildMatrix(); }
	const Vector3&			getRotation() const								{ return _rot; }

	const Matrix4&			getWorldMatrix() const							{ return _world; }

	const BBox3&			getLocalBox() const								{ return _localBox; }

	virtual void			onAddToScene()
	{
		if(_selItem)		_selItem->addToScene();
	}
	virtual void			onRemFromScene()
	{
		if(_selItem)		_selItem->remFromScene();
	}

	virtual void			drawSelection(G_EditorObjDrawer* d)
	{
		d->setColor(Color(0.8f,0.8f,0.8f,1.0f));
		d->setWorldMatrix(_world);
		d->drawBox(_localBox.getMin(),_localBox.getMax());
	}

	virtual G_Object*		copy()=0;
};

#endif