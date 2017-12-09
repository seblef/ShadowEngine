
#ifndef _TDYNAMICTAB_H_
#define _TDYNAMICTAB_H_

#include <memory.h>

namespace Core
{

template<typename T>
class TDynamicTab
{
	protected:

		T*				_buffer;
		T*				_current;
		int				_count;
		int				_allocated;

		void			realloc()
		{
			if(!_buffer)
			{
				_current=_buffer=new T[2];
				_allocated=2;
			}
			else
			{
				T *new_buf=new T[_allocated*2];
				memcpy(new_buf,_buffer,_count * sizeof(T));
				delete[] _buffer;
				_buffer=new_buf;

				_current=_buffer+_count;
				_allocated*=2;
			}
		}

	public:

		TDynamicTab() : _buffer(0), _count(0), _allocated(0), _current(0) {};
		TDynamicTab(int iInit) : _count(0), _allocated(iInit)
		{
			_buffer=_current=new T[iInit];
		}
		~TDynamicTab()						{ release(); };

		void			release()
		{
			if(_buffer)		delete[] _buffer;
			_buffer=_current=0;
			_count=_allocated=0;
		}

		void			clear()
		{
			_count=0;
			_current=_buffer;
		}

		void			add(const T& val)
		{
			if(_count==_allocated)		realloc();

			*_current=val;
			++_current;
			++_count;
		}

		void			add(const T *val)		{ add(*val); }

		void addByMemcpy(const T *val)
		{
			if(_count==_allocated)
				realloc();

			memcpy(_current,val,sizeof(T));
			++_current;
			++_count;
		}

		void			addTab(const T *tab, int count)
		{
			while((_count + count) > _allocated)	realloc();

			memcpy(_current,tab,sizeof(T) * count);
			_current+=count;
			_count+=count;
		}

		void			popBack()
		{
			if(_count > 0)
			{
				--_count;
				--_current;
			}
		}

		void			initialize(int count)
		{
			while(_allocated < count)		realloc();

			_count=count;
			_current=_buffer + count;
		}

		int				getCount() const			{ return _count; }
		int				getAllocated() const		{ return _allocated; }
		T*				getBuffer() const			{ return _buffer; }

		const T&		operator[](size_t n) const	{ return _buffer[n]; }
		T&				operator[](size_t n)		{ return _buffer[n]; }

		void			deleteItem(int n)
		{
			if(n < _count-1)
				memmove(&_buffer[n],&_buffer[n+1],sizeof(T) * (_count - n - 1));
			popBack();
		}

		void				insertItem(int n)
		{
			if(_count==_allocated)		realloc();

			memmove(&_buffer[n+1],&_buffer[n],sizeof(T) * (_count - n));
			++_count;
		}
};

}

#endif
