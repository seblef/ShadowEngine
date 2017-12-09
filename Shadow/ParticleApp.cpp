
#include "stdafx.h"
#include "ParticleApp.h"
#include <ParticleLib.h>
#include <DX11Media.h>
#include <time.h>
#include <sstream>

void ParticleApp::run(HINSTANCE hInst)
{
	initShadowCore();

	FileSystemFactory::createFileSystem(_T("std"));

	IMedia *media=createDX11Media(hInst,800,600,true,false,SINPUT_MOUSE | SINPUT_KEYBOARD);

	IConstantBuffer *cbuf=media->getVideo()->createConstantBuffer(4,0);
	cbuf->set(ST_VERTEX);

	new ParticleEngine(10000,media->getVideo());

	ParticleMaterial mat("Particles/Textures/Smoke.bmp",false,BLEND_SRCCOLOR,BLEND_INVSRCCOLOR);
	ParticleRenderer *prenderer=ParticleEngine::getSingletonRef().getQuadRenderer();
	
	ParticleEmitterParams p1(1.0f,0.5f,0.0f,Color(0.8f,0.0f,0.0f,0.8f),
		Color(0.0f,0.0f,0.0f,0.0f),
		Vector3::NullVector,
		Vector3(0.1f,0.1f,0.1f),
		Vector3(0.01f,0.01f,0.01f));
	ParticleEmitterParams p2(2.0f,1.0f,0.0f,Color(1.0f,0.0f,0.0f,1.0f),
		Color(0.0f,0.0f,0.0f,0.1f),
		Vector3::NullVector,
		Vector3(0.1f,0.1f,0.1f),
		Vector3(0.01f,0.01f,0.01f));
	ParticleEmitter *emitter[1];
	emitter[0]=ParticleEmitterFactory::createParticleEmitter("point",
		prenderer,&mat,10000,Vector3::NullVector,p1,p2,0);
	ParticleSystem ps(1,emitter);
	ps.setWorldMatrix(Matrix4::Identity);

	ParticleLinearEmitter::emit(*emitter[0],Vector3(-5.0f,2.0f,0.0f),Vector3(5.0f,2.0f,0.0f),200);

	BBox3 b(Vector3(-2.5,0,-2.5),Vector3(2.5,2.5,2.5));
	Camera cam(Vector3(0.0f,0.0f,-1.0f),Vector3::NullVector,0.1f,500.0f,Vector2(400,300),45.0f);
	cam.center(b);

	Vector2 camRot(Vector2::NullVector);
	float camDist=(cam.getTarget() - cam.getPosition()).getLength();

	clock_t lastTime=clock();
	float total_time=0;

	bool end=false;
	while(!end)
	{
		clock_t now=clock();
		float elapsed=((float)(now-lastTime)) / ((float)CLOCKS_PER_SEC);
		lastTime=now;

		total_time+=elapsed;
		if(total_time > 2.0f)
		{
			ParticleLinearEmitter::emit(*emitter[0], Vector3(-5.0f, 2.0f, 0.0f), Vector3(5.0f, 2.0f, 0.0f), 200);
			total_time -= 2.0f;
		}

		media->update();

		cam.buildMatrices();
		cbuf->fill((const float*)&cam.getViewProjMatrix());

		ps.update(elapsed,cam);
		ParticleEngine::getSingletonRef().enqueueSystem(&ps);
//		cam.center(ps.getWorldBBox());

		media->getVideo()->resetRenderTargets();
		media->getVideo()->clearRenderTargets(Color(0.1f,0.1f,0.1f,1.0f));
		media->getVideo()->clearDepthStencil();
		ParticleEngine::getSingletonRef().draw(cam);

		media->getVideo()->newFrame();

		Event e;
		while(EventPump::getSingletonRef().getEvent(e))
		{
			if(e._type==ET_CLOSE || (e._type==ET_KEYDOWN && e._param1==DIK_ESCAPE))
				end=true;
			else if(e._type==ET_MOUSEMOVE)
			{
				camRot.x+=(float)e._param2;
				camRot.y+=(float)e._param1;
				updateCamera(cam,camRot,camDist);
			}
			else if(e._type==ET_MOUSEWHEEL)
			{
				if(e._param1 < 0)
					camDist*=1.1f;
				else
					camDist/=1.1f;

				updateCamera(cam,camRot,camDist);
			}
		}

	}
}

void ParticleApp::updateCamera(Camera& c, const Vector2& rot, float dist) const
{
	Vector3 t(c.getTarget());

	Quaternion rot_y(Vector3::YAxisVector,rot.y*M_PI / 180.0f);
	Vector3 z(Vector3::ZAxisVector);
	Vector3 x(Vector3::XAxisVector);

	x=x*rot_y;
	z=z*rot_y;

	Quaternion rot_x(x,rot.x*M_PI/180.0f);
	z=z*rot_x;

	c.setPosition(t-(z*dist));
}