
#ifndef _TALLOCATED_H_
#define _TALLOCATED_H_

#include "TVector.h"

namespace Core
{

template<typename T>
class TAllocated
{
private:

	typedef vector<int> IntVector;

	static TVector<T>				_data;
	static IntVector				_free;

	int								_allocIdx;

public:

	TAllocated() : _allocIdx(-1)	{}
	virtual ~TAllocated()			{}

	virtual void					clearAllocated()					{}

	static T*						New()
	{
		T* t;

		if(_free.empty())
		{
			t=new T;
			t->_allocIdx=_data.getVector().size();
			_data.getVector().push_back(t);
		}
		else
		{
			t=_data.getVector()[_free[_free.size()-1]];
			_free.pop_back();
		}

		return t;
	}

	void						Delete()
	{
		clearAllocated();
		_free.push_back(_allocIdx);
	}
};

template<class T>
typename TVector<T> TAllocated<T>::_data;
template<class T>
vector<int> TAllocated<T>::_free;

};

#endif