
#ifndef _LIGHTSPOT_H_
#define _LIGHTSPOT_H_

#include "Light.h"

class LightSpot : public Light
{
protected:

	float				_nearAngle;
	float				_farAngle;
	float				_nearZ;

public:

	LightSpot(const LightCreate_t& c) : Light(LT_SPOT,c), _nearAngle(c._nearAngle), _farAngle(c._farAngle), _nearZ(c._nearZ)						{}
	LightSpot(const LightSpot* l) : Light(l), _nearAngle(l->_nearAngle), _farAngle(l->_farAngle), _nearZ(l->_nearZ)				{}

	float				getNearAngle() const				{ return _nearAngle; }
	float				getFarAngle() const					{ return _farAngle; }
	float				getNearZ() const					{ return _nearZ; }

	void				setNearAngle(float a)				{ _nearAngle=a; }
	void				setFarAngle(float a)				{ _farAngle=a; }
	void				setNearZ(float z)					{ _nearZ=z; }
};

#endif