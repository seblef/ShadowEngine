
#ifndef _G_UTILS_H_
#define _G_UTILS_H_

#include <Windows.h>
#include <Core.h>

using namespace Core;

class G_Utils
{
public:

	G_Utils()				{}

	static COLORREF			toColorRef(const Color& c)
	{
		int r,g,b;
		r=smin((int)(c.r * 255.0f),255);
		g=smin((int)(c.g * 255.0f),255);
		b=smin((int)(c.b * 255.0f),255);
		return RGB(r,g,b);
	}

	static COLORREF			toColorRefIntensity(const Color& c, float& intensity)
	{
		intensity=smax(smax(c.r,c.g),c.b);
		return toColorRef(c / intensity);
	}

	static void				toColor(COLORREF o, Color& c)
	{
		c.r=((float)GetRValue(o)) / 255.0f;
		c.g=((float)GetGValue(o)) / 255.0f;
		c.b=((float)GetBValue(o)) / 255.0f;
		c.a=1.0f;
	}

	static void				toColor(COLORREF o, float intensity, Color& c)
	{
		c.r=((float)GetRValue(o)) / 255.0f;
		c.g=((float)GetGValue(o)) / 255.0f;
		c.b=((float)GetBValue(o)) / 255.0f;
		c*=intensity;
		c.a=1.0f;
	}
};

#endif