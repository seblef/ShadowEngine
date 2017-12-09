
#ifndef _ACTION_H_
#define _ACTION_H_

class Action
{
protected:

	bool				_dead;
	float				_delayTime;

public:

	Action() : _dead(false)				{}
	virtual ~Action()					{}

	bool				isDead() const				{ return _dead; }
	float&				getDelayTime()				{ return _delayTime; }

	virtual void		update(float time) = 0;
};

#endif