
#ifndef _TRACE_H_
#define _TRACE_H_

#include "Action.h"
#include "TraceTemplate.h"
#include <StdRenderer.h>


class Trace : public Action
{
protected:

	Particles*				_particleSystem;

public:

	Trace(const TraceTemplate& t, const Vector3& from, const Vector3& to);
	~Trace();

	void					update(float time);
};
#endif