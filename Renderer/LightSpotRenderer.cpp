
#include "stdafx.h"
#include "LightSpotRenderer.h"

LightSpotRenderer::LightSpotRenderer(const LightRendererCreate_t& c) :
LightRenderer(c, _T("Lights/Spot"), _T("Lights/SpotShadows"))
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