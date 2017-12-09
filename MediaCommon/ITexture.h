
#ifndef _ITEXTURE_H_
#define _ITEXTURE_H_

#include "VideoDefs.h"
#include <string>

using namespace std;

#define TF_NONE					0
#define TF_GOOD					1
#define TF_RENDERTARGET			2
#define TF_CUBETEXTURE			4

class ITexture
{
protected:

	unsigned int			_flags;
	string					_name;
	int						_width, _height;
	TextureFormat			_format;
	BufferUsage				_usage;

public:

	ITexture(const string& name, unsigned int flags, int width, int height, TextureFormat format, BufferUsage bu) : _flags(flags), _name(name),
		_width(width), _height(height), _format(format), _usage(bu) {}
	virtual ~ITexture() {}

	bool					isGood() const				{ return (_flags & TF_GOOD)!=0; }
	bool					isRenderTarget() const		{ return (_flags & TF_RENDERTARGET)!=0; }
	bool					isCubeTexture() const		{ return (_flags & TF_CUBETEXTURE)!=0; }
	TextureFormat			getFormat() const			{ return _format; }
	BufferUsage				getUsage() const			{ return _usage; }
	int						getWidth() const			{ return _width; }
	int						getHeight() const			{ return _height; }
	const string&			getName() const				{ return _name; }

	virtual void			set(int stage, ShaderType=ST_PIXEL)=0;
	virtual bool			lock(void **ppData, int& rowPitch, int& depthPitch)=0;
	virtual void			unlock()=0;
	virtual void			fill(const void* data)=0;
	virtual bool			save(const string& fileName)=0;
	virtual bool			generateMipMaps()=0;
};

#endif