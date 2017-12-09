
#include "stdafx.h"
#include "G_Light.h"
#include "G_Selection.h"

G_Light::G_Light(Light::LightType type, const LightCreate_t& c) : G_Object(LIGHT), _originalDir(c._direction)
{
	_selItem=G_Selection::getSingletonRef().createSphereActor(0.25f);
	_selItem->setUserData((G_Object*)this);

	switch(type)
	{
	case Light::LT_OMNI:			_rLight=new LightOmni(c);			break;
	case Light::LT_SPOT:			_rLight=new LightSpot(c);			break;
	case Light::LT_AREA:			_rLight=new LightArea(c);			break;
	}
}

G_Light::G_Light(Light::LightType type) : G_Object(LIGHT), _originalDir(Vector3::ZAxisVector)
{
	_selItem=G_Selection::getSingletonRef().createSphereActor(0.25f);
	_selItem->setUserData((G_Object*)this);

	LightCreate_t c;
	c._world=Matrix4::Identity;
	c._areaHeight=1.0f;
	c._areaWidth=2.0f;
	c._castShadows=(type!=Light::LT_OMNI);
	c._color=Color(0.8f,0.8f,0.8f,1.0f);
	c._direction=Vector3::ZAxisVector;
	c._farAngle=90.0f;
	c._nearAngle=60.0f;
	c._nearZ=0.1f;
	c._range=3.0f;
	c._shadowMapSize=256;
	c._shadowMapFilter=4;

	switch(type)
	{
	case Light::LT_OMNI:			_rLight=new LightOmni(c);			break;
	case Light::LT_SPOT:			_rLight=new LightSpot(c);			break;
	case Light::LT_AREA:			_rLight=new LightArea(c);			break;
	}
}

G_Light::G_Light(const G_Light& l) : G_Object(&l), _originalDir(l._originalDir)
{
	_selItem=G_Selection::getSingletonRef().createSphereActor(0.25f);
	_selItem->setUserData((G_Object*)this);

	switch(l._rLight->getLightType())
	{
	case Light::LT_OMNI:			_rLight=new LightOmni((const LightOmni*)l._rLight);		break;
	case Light::LT_SPOT:			_rLight=new LightSpot((const LightSpot*)l._rLight);		break;
	case Light::LT_AREA:			_rLight=new LightArea((const LightArea*)l._rLight);		break;
	}
}

void G_Light::rebuildMatrix()
{
	if(_rLight->getLightType()==Light::LT_OMNI)
	{
		_world.createIdentity();
		_world=_pos;

		if(_selItem)
			_selItem->updateWorldMatrix(_world);

		_rLight->setWorldMatrix(_world);
	}
	else
	{
		G_Object::rebuildMatrix();
		Matrix4 w(_world);

		Vector3 d(_originalDir);
		d.transformNoTranslation(w);

		_rLight->setDirection(d);

		Light::computeMatrix(d,w);
		_rLight->setWorldMatrix(w);
	}
}

void G_Light::drawSelection(G_EditorObjDrawer* d)
{
	d->setColor(_rLight->getColor());
	d->setWorldMatrix(_rLight->getWorldMatrix());
	switch(_rLight->getLightType())
	{
	case Light::LT_OMNI:			drawOmni(d);			break;
	case Light::LT_SPOT:			drawSpot(d);			break;
	case Light::LT_AREA:			drawArea(d);			break;
	}
}

void G_Light::drawOmni(G_EditorObjDrawer* d) const
{
	float r=_rLight->getRange() * 0.5f;

	d->drawCircle(Vector3::NullVector,Vector3::XAxisVector,r);
	d->drawCircle(Vector3::NullVector,Vector3::YAxisVector,r);
	d->drawCircle(Vector3::NullVector,Vector3::ZAxisVector,r);
}

void G_Light::drawSpot(G_EditorObjDrawer* d) const
{
	float r=_rLight->getRange();
	float angle=getSpot().getFarAngle() * M_PI / 360.0f;
	float s=sinf(angle);
	float c=cosf(angle);
	float scale=r * s / c;

	d->drawCircle(Vector3(0.0f,0.0f,r),Vector3::ZAxisVector,scale);

	d->drawLine(Vector3::NullVector,Vector3(-scale,0.0f,r));
	d->drawLine(Vector3::NullVector,Vector3( scale,0.0f,r));
	d->drawLine(Vector3::NullVector,Vector3(0.0f,-scale,r));
	d->drawLine(Vector3::NullVector,Vector3(0.0f, scale,r));

	angle=getSpot().getNearAngle() * M_PI / 360.0f;
	s=sinf(angle);
	c=cosf(angle);
	scale=r * s / c;

	d->setColor(_rLight->getColor()*0.9f);
	d->drawCircle(Vector3(0.0f,0.0f,r),Vector3::ZAxisVector,scale);

	d->drawLine(Vector3::NullVector,Vector3(-scale,0.0f,r));
	d->drawLine(Vector3::NullVector,Vector3( scale,0.0f,r));
	d->drawLine(Vector3::NullVector,Vector3(0.0f,-scale,r));
	d->drawLine(Vector3::NullVector,Vector3(0.0f, scale,r));
}

void G_Light::drawArea(G_EditorObjDrawer* d) const
{
	float r=_rLight->getRange();
	float w=getArea().getAreaWidth()*0.5f;
	float h=getArea().getAreaHeight()*0.5f;
	float aspect=w / h;
	float xscale,yscale;
	float angle=getArea().getFarAngle() * M_PI / 360.0f;
	float s=sinf(angle);
	float c=cosf(angle);
	float div=c / s;

	yscale=0.5f * (r) / div + h;
	xscale=yscale * aspect + w;

	d->drawRect(Vector3(-w,-h,0.0f),Vector3( w,-h,0.0f),Vector3( w, h,0.0f),Vector3(-w, h,0.0f));

	d->drawRect(Vector3(-xscale,-yscale,r),
		Vector3( xscale,-yscale,r),
		Vector3( xscale, yscale,r),
		Vector3(-xscale,yscale,r));
	d->drawLine(Vector3(-w,-h,0.0f),Vector3(-xscale,-yscale,r));
	d->drawLine(Vector3( w,-h,0.0f),Vector3( xscale,-yscale,r));
	d->drawLine(Vector3( w, h,0.0f),Vector3( xscale, yscale,r));
	d->drawLine(Vector3(-w, h,0.0f),Vector3(-xscale, yscale,r));

	angle=getSpot().getNearAngle() * M_PI / 360.0f;
	s=sinf(angle);
	c=cosf(angle);
	div=c / s;

	yscale=0.5f * (r) / div + h;
	xscale=yscale * aspect + w;

	d->setColor(_rLight->getColor()*0.9f);
	d->drawRect(Vector3(-xscale,-yscale,r),
		Vector3( xscale,-yscale,r),
		Vector3( xscale, yscale,r),
		Vector3(-xscale,yscale,r));
	d->drawLine(Vector3(-w,-h,0.0f),Vector3(-xscale,-yscale,r));
	d->drawLine(Vector3( w,-h,0.0f),Vector3( xscale,-yscale,r));
	d->drawLine(Vector3( w, h,0.0f),Vector3( xscale, yscale,r));
	d->drawLine(Vector3(-w, h,0.0f),Vector3(-xscale, yscale,r));
}