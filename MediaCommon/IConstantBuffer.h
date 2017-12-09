
#ifndef _ICONSTANTBUFFER_H_
#define _ICONSTANTBUFFER_H_

#include "VideoDefs.h"

class IConstantBuffer
{
protected:

	bool			_good;
	int				_size, _slot;

public:

	IConstantBuffer(int size, int slot) : _good(false), _size(size), _slot(slot) {}
	virtual ~IConstantBuffer() {}

	bool			isGood() const					{ return _good; }
	int				getSize() const					{ return _size; }
	int				getSlot() const					{ return _slot; }

	virtual void	set(ShaderType st)=0;
	virtual void	fill(const void* data)=0;
};

#endif