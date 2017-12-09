
#ifndef _IEXPORTER_H_
#define _IEXPORTER_H_

#include <IGame/IGame.h>

class IExporter
{
protected:

	IGameScene*				_scene;

public:

	IExporter();
	virtual ~IExporter();
};

#endif