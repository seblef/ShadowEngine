
#ifndef _MATERIALFADEANIMATION_H_
#define _MATERIALFADEANIMATION_H_

#include "MaterialAnimation.h"

class MaterialFadeAnimation : public MaterialAnimation
{
protected:

	float		_start;
	float		_end;

public:

	MaterialFadeAnimation(Material *m) : MaterialAnimation(m), _start(0), _end(1)			{}
	MaterialFadeAnimation(Material *m, float start, float end) : MaterialAnimation(m), _start(start), _end(end)	{}
	MaterialFadeAnimation(const MaterialFadeAnimation& a) : MaterialAnimation(a._material), _start(a._start), _end(a._end)	{}
	MaterialFadeAnimation(Material *m, ScriptFile& sf) : MaterialAnimation(m), _start(0), _end(0)
	{
		_start=stof(sf.getToken());
		_end=stof(sf.getToken());
	}

	const char*	getAnimationName() const				{ return "fade"; }

	void		set(float time)
	{
		Color c(_material->getStdBuffer().getDiffuse());
		c.a=0;
		if(time <= _start)				c.a=1;
		else if(time < _end)			c.a=(time - _start) / (_end - _start);

		_material->getStdBuffer().setDiffuse(c);
	}

	float		getStartTime() const					{ return _start; }
	float		getEndTime() const						{ return _end; }

	void		setStartTime(float st)					{ _start=st; }
	void		setEndTime(float et)					{ _end=et; }
};

#endif