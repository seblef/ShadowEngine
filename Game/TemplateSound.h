
#ifndef _TEMPLATESOUND_H_
#define _TEMPLATESOUND_H_

#include <SoundLib.h>

class TemplateSound
{
protected:

	ISound*				_sound;
	ISoundCone*			_cone;
	float				_radius;
	float				_radiusAngle;
	int					_priority;

public:

	TemplateSound(ISound *snd, float radius, float radiusAngle, int priority, ISoundCone* cone) :
		_sound(snd), _cone(cone), _radius(radius), _radiusAngle(radiusAngle), _priority(priority)			{}
	~TemplateSound();

	ISound*				getSound() const			{ return _sound; }
	ISoundCone*			getCone() const				{ return _cone; }
	float				getRadius() const			{ return _radius; }
	float				getRadiusAngle() const		{ return _radiusAngle; }
	int					getPriority() const			{ return _priority; }
};

#endif