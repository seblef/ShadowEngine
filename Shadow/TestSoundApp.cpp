
#include "stdafx.h"
#include "TestSoundApp.h"
#include <DX11Media.h>
#include <time.h>
#include <sstream>



void TestSoundApp::run(HINSTANCE hInst)
{
	initShadowCore();

	FileSystemFactory::createFileSystem(_T("std"));

	IMedia *media = createDX11Media(hInst, 800, 600, true, false, SINPUT_MOUSE | SINPUT_KEYBOARD);

	ISound *snd = media->getAudio()->loadSound("Sounds/Test.wav");
	media->getAudio()->getSource(0)->play(0, snd, true);

	clock_t lastTime = clock();
	float total_time = 0;

	bool end = false;
	while (!end)
	{
		clock_t now = clock();
		float elapsed = ((float)(now - lastTime)) / ((float)CLOCKS_PER_SEC);
		lastTime = now;

		total_time += elapsed;
		if (total_time > 2.0f)
		{
			total_time -= 2.0f;
		}

		media->update();

		media->getVideo()->resetRenderTargets();
		media->getVideo()->clearRenderTargets(Color(0.1f, 0.1f, 0.1f, 1.0f));
		media->getVideo()->newFrame();

		Event e;
		while (EventPump::getSingletonRef().getEvent(e))
		{
			if (e._type == ET_CLOSE || (e._type == ET_KEYDOWN && e._param1 == DIK_ESCAPE))
				end = true;
		}

	}
}