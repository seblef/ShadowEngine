
#include "stdafx.h"
#include "IExporter.h"

IExporter::IExporter()
{
	_scene=GetIGameInterface();
	IGameConversionManager * cm = GetConversionManager();
	cm->SetCoordSystem(IGameConversionManager::IGAME_D3D);
	_scene->InitialiseIGame(false);
	_scene->SetStaticFrame(0);
}

IExporter::~IExporter()
{
	_scene->ReleaseIGame();
}