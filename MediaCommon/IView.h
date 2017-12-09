
#ifndef _IVIEW_H_
#define _IVIEW_H_

class IView
{
protected:

	int						_width;
	int						_height;

public:

	IView(int w, int h) : _width(w), _height(h)			{}
	virtual ~IView()									{}

	int						getWidth() const			{ return _width; }
	int						getHeight() const			{ return _height; }

	virtual void			set()=0;
	virtual void			setDepthBufferAsTexture(int slot)=0;

};

#endif