
#include "stdafx.h"
#include "G_EditorObjDrawer.h"
#include <StdRenderer.h>


G_EditorObjDrawer::G_EditorObjDrawer(IVideoDevice *d) : _device(d),
	_vCount(0), _color(Color::White)
{
	_shader=d->createShader("Editor/Drawer");
	_vBuffer=d->createVertexBuffer(MAX_DRAWER_LINES*2,VX_BASE,BU_DYNAMIC);
	_renderState=d->createRenderState(CULL_NONE);
	_depthState=d->createDepthStencilState(true,true,COMP_LESSEQUAL);
	_depthOverState=d->createDepthStencilState(false,false,COMP_ALWAYS);
	_blendState=d->createBlendState(false,BLEND_ONE,BLEND_ZERO);
}

G_EditorObjDrawer::~G_EditorObjDrawer()
{
	delete _shader;
	delete _vBuffer;

	_device->destroyBlendState(_blendState);
	_device->destroyRenderState(_renderState);
	_device->destroyDepthStencilState(_depthState);
	_device->destroyDepthStencilState(_depthOverState);
}

void G_EditorObjDrawer::begin(bool noZBuffer)
{
	_shader->set();
	_vBuffer->set();
	_device->setBlendState(_blendState);
	_device->setRenderState(_renderState);
	_device->setDepthStencilState(noZBuffer ? _depthOverState : _depthState);
	_device->setInputLayer(VX_BASE);
	_device->setPrimitiveType(PT_LINELIST);
	_device->resetRenderTargets();

	Renderer::getSingletonRef().setObjectTransformation(Matrix4::Identity);
}


void G_EditorObjDrawer::setWorldMatrix(const Matrix4& mWorld)
{
	execute();
	Renderer::getSingletonRef().setObjectTransformation(mWorld);
}

void G_EditorObjDrawer::execute()
{
	if(_vCount)
	{
		_vBuffer->fill(_vertices,_vCount);
		_device->render(_vCount);
		_vCount=0;
	}
}

void G_EditorObjDrawer::drawLine(const Vector3& v1, const Vector3& v2)
{
	_vertices[_vCount].pos=v1;
	_vertices[_vCount].color = _color;

	_vertices[_vCount+1].pos=v2;
	_vertices[_vCount + 1].color = _color;

	_vCount+=2;
	if(_vCount >= MAX_DRAWER_LINES*2)
		execute();
}

void G_EditorObjDrawer::drawDot(const Vector3& v, float fSize)
{
	drawLine(Vector3(v.x - fSize,v.y,v.z),Vector3(v.x + fSize,v.y,v.z));
	drawLine(Vector3(v.x,v.y - fSize,v.z),Vector3(v.x,v.y + fSize,v.z));
	drawLine(Vector3(v.x,v.y,v.z - fSize),Vector3(v.x,v.y,v.z + fSize));
}

void G_EditorObjDrawer::drawCircle(const Vector3& vCenter, const Vector3& vAxe, float fSize)
{
	const float fStep=2 * M_PI / 36.0f;
	float fAngle=0.0f;
	for(int i=0;i<36;++i,fAngle+=fStep)
	{
		Quaternion q1(vAxe,fAngle);
		Quaternion q2(vAxe,fAngle + fStep);

		drawLine(vCenter + (q1 * Vector3::XAxisVector) * fSize,vCenter + (q2 * Vector3::XAxisVector) * fSize);
	}
}

void G_EditorObjDrawer::drawBox(const Vector3& v1, const Vector3& v2)
{
	drawLine(v1,Vector3(v2.x,v1.y,v1.z));
	drawLine(Vector3(v2.x,v1.y,v1.z),Vector3(v2.x,v2.y,v1.z));
	drawLine(Vector3(v2.x,v2.y,v1.z),Vector3(v1.x,v2.y,v1.z));
	drawLine(Vector3(v1.x,v2.y,v1.z),v1);

	drawLine(Vector3(v1.x,v1.y,v2.z),Vector3(v2.x,v1.y,v2.z));
	drawLine(Vector3(v2.x,v1.y,v2.z),Vector3(v2.x,v2.y,v2.z));
	drawLine(Vector3(v2.x,v2.y,v2.z),Vector3(v1.x,v2.y,v2.z));
	drawLine(Vector3(v1.x,v2.y,v2.z),Vector3(v1.x,v1.y,v2.z));

	drawLine(Vector3(v1.x,v1.y,v2.z),Vector3(v1.x,v1.y,v1.z));
	drawLine(Vector3(v2.x,v1.y,v2.z),Vector3(v2.x,v1.y,v1.z));
	drawLine(Vector3(v2.x,v2.y,v2.z),Vector3(v2.x,v2.y,v1.z));
	drawLine(Vector3(v1.x,v2.y,v2.z),Vector3(v1.x,v2.y,v1.z));
}

void G_EditorObjDrawer::drawRect(const Vector3& v1, const Vector3& v2,
								const Vector3& v3, const Vector3& v4)
{
	drawLine(v1,v2);
	drawLine(v2,v3);
	drawLine(v3,v4);
	drawLine(v4,v1);
}
