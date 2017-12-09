
#ifndef _RENDERERSTATS_H_
#define _RENDERERSTATS_H_

#ifdef _OUTPUTSTATS

#include <Core.h>

class RendererStats : public TSingleton<RendererStats>
{
protected:

	Stat<unsigned long>			_tri;
	Stat<unsigned long>			_time;

public:

	RendererStats() : _tri("Triangle count",0xffffffff,0), _time("Rendering time",0xffffffff,0)			{}

	void						addTriangles(unsigned long count)					{ _tri.addValue(count); }
	void						addRenderTime(unsigned long time)					{ _time.addValue(time); }
	void						newFrame()
	{
		_tri.addRef();
		_time.addRef();
	}

	void						output(const ostream& os) const
	{
		os << _tri;
		os << _time;
	}
};

#endif

#endif