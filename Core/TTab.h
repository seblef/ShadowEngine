
#ifndef _TTAB_H_
#define _TTAB_H_

namespace Core
{

template<typename T>
class TTab
{
protected:

	int					_count;
	T*					_tab;

public:

	TTab(int count) : _count(count)
	{
		_tab=new T[count];
	}
	TTab(int count, const T* vals) : _count(t._count)
	{
		_tab=new T[count];
		for(int i=0;i<count;++i)
			_tab[i]=vals[i];
	}
	TTab(const TTab<T>& t) : _count(t._count)
	{
		_tab=new T[_count];
		for(int i=0;i<_count;++i)
			_tab[i]=t._tab[i];
	}
	virtual ~TTab()
	{
		delete[] _tab;
	}

	int					getCount() const					{ return _count; }

	T*					getItems() const					{ return _tab; }
						operator T*() const					{ return _tab; }
						operator const T*() const			{ return _tab; }

	T&					getItem(int n)						{ return _tab[n]; }
	const T&			getItem(int n) const				{ return _tab[n]; }

	T&					operator[](size_t n)				{ return _tab[n]; }
	const T&			operator[](size_t n) const			{ return _tab[n]; }

	TTab<T>&			operator=(const TTab<T>& t)
	{
		assert(t.getCount()==_count);
		for(int i=0;i<_count;++i)
			_tab[i]=t._tab[i];
		return *this;
	}
};

};

#endif