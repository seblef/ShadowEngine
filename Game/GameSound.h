
#ifndef _GAMESOUND_H_
#define _GAMESOUND_H_

#include "TemplateSound.h"
#include "GameObject.h"

class GameSound : public GameObject
{
protected:

	SoundVirtualSource*			_source;
	TemplateSound*				_template;
	bool						_playing;

public:

	GameSound(TemplateSound* snd, const Matrix4& world);
	~GameSound();

	void						play();
	void						stop();

	void						onAddToScene()								{ play(); }
	void						onRemFromScene()							{ stop(); }

	void						onUpdateMatrix(const Matrix4& world);
};

#endif