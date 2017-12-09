
#ifndef _IINDEXBUFFER_H_
#define _IINDEXBUFFER_H_

#include "VideoDefs.h"

class IIndexBuffer
{
protected:

	bool				_good;
	int					_idxCount;
	IndexType			_idxType;
	BufferUsage			_usage;

public:

	IIndexBuffer(int idxCount, IndexType idxType, BufferUsage bu) : _good(false), _idxCount(idxCount), _idxType(idxType), _usage(bu) {}
	virtual ~IIndexBuffer() {}

	bool				isGood() const				{ return _good; }
	int					getIndexCount() const		{ return _idxCount; }
	IndexType			getIndexType() const		{ return _idxType; }
	BufferUsage			getUsage() const			{ return _usage; }

	virtual void		set()=0;
	virtual void		fill(const void* data, int count=0, int offset=0)=0;
};


#endif