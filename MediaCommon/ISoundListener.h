
#ifndef _ISOUNDLISTENER_H_
#define _ISOUNDLISTENER_H_

#include "ISoundCone.h"
#include <Core.h>

using namespace Core;

class ISoundListener
{
public:

	ISoundListener()			{}
	virtual ~ISoundListener()	{}

	virtual void			setPosition(const Vector3& pos) = 0;
	virtual void			setVelocity(const Vector3& vel) = 0;
	virtual void			setFrontAxis(const Vector3& f) = 0;
	virtual void			setTopAxis(const Vector3& t) = 0;
	virtual void			setCone(ISoundCone* c) = 0;

	virtual const Vector3&	getPosition() const = 0;
	virtual const Vector3&	getVelocity() const = 0;
	virtual const Vector3&	getFrontAxis() const = 0;
	virtual const Vector3&	getTopAxis() const = 0;
	virtual ISoundCone*		getCone() const = 0;
};

#endif