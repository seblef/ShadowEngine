
#ifndef _MATRICEHIERARCHY_H_
#define _MATRICEHIERARCHY_H_

#include "TTab.h"
#include "Matrix4.h"

namespace Core
{

class MatrixHierarchy : public TTab<Matrix4>
{
protected:

	int*				_parents;
	Matrix4*			_offsets;
	bool*				_offsetsEnable;

public:

	MatrixHierarchy(int count, bool offsets) : TTab<Matrix4>(count), _offsets(0), _offsetsEnable(0)
	{
		_parents=new int[count];
		for(int i=0;i<count;++i)
			_parents[i]=-1;

		if (offsets)
		{
			_offsets = new Matrix4[count];
			_offsetsEnable = new bool[count];

			for (int i = 0; i < count; ++i)
				_offsetsEnable[i] = false;
		}
	}

	MatrixHierarchy(const MatrixHierarchy& mh) : TTab<Matrix4>(*((const TTab<Matrix4>*)&mh)), _offsets(0), _offsetsEnable(0)
	{
		_parents=new int[mh.getCount()];
		for(int i=0;i<mh.getCount();++i)
			_parents[i]=mh._parents[i];

		if (mh._offsets)
		{
			_offsets = new Matrix4[mh.getCount()];
			_offsetsEnable = new bool[mh.getCount()];

			memcpy(_offsets, mh._offsets, sizeof(Matrix4)* mh.getCount());
			memcpy(_offsetsEnable, mh._offsetsEnable, sizeof(bool)* mh.getCount());
		}
	}

	~MatrixHierarchy()
	{
		delete[] _parents;
		if (_offsets)
		{
			delete[] _offsets;
			delete[] _offsetsEnable;
		}
	}

						operator TTab<Matrix4>&()				{ return *((TTab<Matrix4>*)this); }
						operator const TTab<Matrix4>&()	const	{ return *((TTab<Matrix4>*)this); }

	int					getParent(int n) const					{ return _parents[n]; }
	void				setParent(int n, int p)					{ _parents[n]=p; }

	int*				getParents() const						{ return _parents; }

	void				enableOffset(int idx, bool enable=true)	{ _offsetsEnable[idx] = enable; }
	void				setOffset(int idx, const Matrix4& offset) { _offsets[idx] = offset; }

	void				update(const Matrix4& world, const TTab<Matrix4>& pose)
	{
		if (_offsets)
		{
			_tab[0] = pose.getItem(0)*world;
			for (int i = 1; i < _count; ++i)
			{
				_tab[i] = pose.getItem(i) * _tab[_parents[i]];
				if (_offsetsEnable[i])
					_tab[i] = _offsets[i] * _tab[i];
			}
		}
		else
		{
			_tab[0] = pose.getItem(0)*world;
			for (int i = 1; i<_count; ++i)
				_tab[i] = pose.getItem(i) * _tab[_parents[i]];
		}
	}
};

};

#endif