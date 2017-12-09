
#ifndef _IUPDATABLE_H_
#define _IUPDATABLE_H_

class IUpdatable
{
protected:

	bool						_sleeping;
	float						_time;

public:

	IUpdatable() : _sleeping(true), _time(0)			{}

	virtual void				update(float time)		{ _time+=time; }
	void						resetTime()				{ _time=0; }
	float						getTime() const			{ return _time; }

	void						wakeUp();
	void						sleep();

	bool						isSleeping() const				{ return _sleeping; }
};

#endif