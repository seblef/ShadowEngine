
#ifndef _ISHADOWMAP_H_
#define _ISHADOWMAP_H_


class IShadowMap
{
protected:

	bool					_good;
	int						_width, _height;

public:

	IShadowMap(int width, int height) : _good(false), _width(width), _height(height)	{}
	virtual ~IShadowMap() {}

	bool					isGood() const					{ return _good; }
	int						getWidth() const				{ return _width; }
	int						getHeight() const				{ return _height; }

	virtual void			set(int stage)=0;
	virtual void			setAsRenderTarget()=0;
};


#endif