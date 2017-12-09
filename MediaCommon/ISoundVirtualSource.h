
#ifndef _ISOUNDVIRTUALSOURCE_H_
#define _ISOUNDVIRTUALSOURCE_H_

#include "ISoundCone.h"
#include <Core.h>

using namespace Core;

class ISoundVirtualSource
{
public:

	ISoundVirtualSource() {}
	virtual ~ISoundVirtualSource()	{}

	virtual void		setPosition(const Vector3& pos) = 0;
	virtual void		setVelocity(const Vector3& vel) = 0;
	virtual void		setInRadius(float r) = 0;
	virtual void		setInRadiusAngle(float a) = 0;
	virtual void		setCone(ISoundCone* c) = 0;
	virtual void		setFrontAxis(const Vector3& f) = 0;
	virtual void		setTopAxis(const Vector3& t) = 0;
	virtual void		setDopplerScale(float d) = 0;
	virtual void		setRelative(bool relative) = 0;
	virtual void		setVolume(float volume) = 0;

	virtual const Vector3&		getPosition() const = 0;
	virtual const Vector3&		getVelocity() const = 0;
	virtual float				getInRadius() const = 0;
	virtual float				getInRadiusAngle() const = 0;
	virtual ISoundCone*			getCone() const = 0;
	virtual const Vector3&		getFrontAxis() const = 0;
	virtual const Vector3&		getTopAxis() const = 0;
	virtual float				getDopplerScale() const = 0;
	virtual bool				isRelative() const = 0;
	virtual float				getVolume() const = 0;
};

#endif