
#include "stdafx.h"
#include "LightAreaRenderer.h"

LightAreaRenderer::LightAreaRenderer(const LightRendererCreate_t& c) :
	LightRenderer(c, _T("Lights/Area"), _T("Lights/AreaShadows"))
{
	VxBase sSpotVertices[5]={
		VxBase(Vector3::NullVector,Color::White,Vector2::NullVector),
		VxBase(Vector3(-1.0f,-1.0f,1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3( 1.0f,-1.0f,1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3( 1.0f, 1.0f,1.0f),Color::White,Vector2::NullVector),
		VxBase(Vector3(-1.0f, 1.0f,1.0f),Color::White,Vector2::NullVector) };

	unsigned short swSpotIndices[18]={
		0,2,1,
		0,3,2,
		0,4,3,
		0,1,4,
		1,2,4,
		2,3,4 };

	_vBuffer=_device->createVertexBuffer(5,VX_BASE,BU_IMMUTABLE,sSpotVertices);
	_iBuffer=_device->createIndexBuffer(18,IDX_16BITS,BU_IMMUTABLE,swSpotIndices);
	_indicesCount=18;
}

void LightAreaRenderer::fillBufferAndGetMatrix(LightBuffer_t& b, const Light* l, Matrix4& world) const
{
	const LightArea *s=(const LightArea*)l;
	float sfar=s->getFarAngle() * M_PI / 360.0f;
	float snear=s->getNearAngle() * M_PI / 360.0f;

	float cf=cosf(sfar);
	float sf=sinf(sfar);

	b._nearAngle=cosf(snear);
	b._farAngle=cf;
	b._dir=s->getDirection();

	float aspect=s->getAreaWidth() / s->getAreaHeight();
	float xscale,yscale;
	float div=cf / sf;

	float dist=s->getAreaHeight() * div * 0.5f;
	float range=s->getRange() + dist;
	yscale=range / div;
	xscale=yscale * aspect;

	const Matrix4& mw(s->getWorldMatrix());
	Vector3 pos(mw);
	pos-=s->getDirection() * dist;

	Vector3 up(Vector3::YAxisVector);
	up.transformNoTranslation(mw);
	up.normalize();

	world.createScale(xscale,yscale,range);
	world*=mw;
	world=pos;

	Camera cam(pos,pos + l->getDirection(),dist,range,Vector2(s->getAreaWidth()*0.5f,s->getAreaHeight()*0.5f),s->getFarAngle());
	cam.setUpVector(up);
	cam.buildMatrices();
	b._areaViewProj=cam.getViewProjMatrix();
}