
#include "stdafx.h"
#include "NavTest.h"
#include "GameApp.h"
#include "ParticleApp.h"
#include "TestSoundApp.h"
#include <sstream>

ofstream g_MediaOut("Shadow.log");
HINSTANCE			g_AppHInstance = 0;

int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
{
	g_AppHInstance = hInstance;
//	NavTest app;
	GameApp app;
//	ParticleApp app;
//	TestSoundApp app;

	app.run(hInstance);

	return 1;
}