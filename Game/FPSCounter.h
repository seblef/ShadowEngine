
#ifndef _FPSCOUNTER_H_
#define _FPSCOUNTER_H_

#include <R2D.h>

class FPSCounter
{
protected:

	double			_minFrameTime;
	double			_maxFrameTime;
	double			_totalFrameTime;
	double			_totalFrameCount;
	double			_lastTime;

	R2D_Font*		_font;
	R2D_Text*		_text;
	R2D_Layer*		_layer;

	string			_counterName;

public:

	FPSCounter(const string& counterName, bool showCounter, R2D_Font* fpsFont);
	~FPSCounter();

	R2D_Layer*		getLayer() const			{ return _layer; }

	void			updateCounter();
	void			outputStats(ofstream& ofs);
};

#endif