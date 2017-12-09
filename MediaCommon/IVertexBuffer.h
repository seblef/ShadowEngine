
#ifndef _IVERTEXBUFFER_H_
#define _IVERTEXBUFFER_H_

#include <Core.h>
#include "VideoDefs.h"

using namespace Core;

class IVertexBuffer
{
protected:

	bool			_good;
	int				_vxCount;
	VertexType		_vxType;
	BufferUsage		_usage;

public:

	IVertexBuffer(int vxCount, VertexType vxType, BufferUsage bu) : _good(false), _vxCount(vxCount), _vxType(vxType), _usage(bu) {}
	virtual ~IVertexBuffer() {}

	bool			isGood() const						{ return _good; }
	int				getVertexCount() const				{ return _vxCount; }
	VertexType		getVertexType() const				{ return _vxType; }
	BufferUsage		getUsage() const					{ return _usage; }

	virtual void	set()=0;
	virtual void	fill(const void* data, int count=0, int offset=0)=0;
};

#endif