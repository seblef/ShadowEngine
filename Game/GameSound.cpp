
#include "stdafx.h"
#include "GameSound.h"

extern ofstream g_MediaOut;

GameSound::GameSound(TemplateSound* snd, const Matrix4& world) : GameObject(SOUND,world, BBox3::NullBox, 0), _playing(false), _template(snd)
{
	_source = new SoundVirtualSource(SoundSystem::getSingletonRef().getAudio(), snd->getPriority(), 1.0f, true);
	_source->getSource()->setCone(snd->getCone());
	_source->getSource()->setInRadius(snd->getRadius());
	_source->getSource()->setInRadiusAngle(snd->getRadiusAngle());
	_source->setSound(snd->getSound());

	onUpdateMatrix(world);
}

GameSound::~GameSound()
{
	if (_playing)			stop();
	delete _source;
}

void GameSound::play()
{
	if (!_playing)
	{
		_playing = true;
		SoundSystem::getSingletonRef().play(_source, _template->getSound());
	}
}

void GameSound::stop()
{
	if (_playing)
	{
		_playing = false;
		SoundSystem::getSingletonRef().stopSound(_source);
	}
}

void GameSound::onUpdateMatrix(const Matrix4& world)
{
	ISoundVirtualSource* src = _source->getSource();
	src->setPosition(world);
	src->setVelocity(Vector3::NullVector);

	Vector3 front(Vector3::ZAxisVector), top(Vector3::YAxisVector);
	front.transformNoTranslation(world);
	top.transformNoTranslation(world);

	src->setFrontAxis(front);
	src->setTopAxis(top);
}