
#include "stdafx.h"
#include "TemplateSound.h"
#include "GameSystem.h"

TemplateSound::~TemplateSound()
{
	if (_sound)
		SoundSystem::getSingletonRef().releaseSound(_sound);

	if (_cone)
		delete _cone;
}