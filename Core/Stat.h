
#ifndef _STAT_H_
#define _STAT_H_

#include <ostream>
#include <string>

using namespace std;

namespace Core
{

template<typename T>
class Stat
{
protected:

	T				_min,_max,_total;
	unsigned long	_count;
	string			_name;

public:

	Stat(const string& name, T minVal, T maxVal) : _name(name), _min(minVal), _max(maxVal), _total(0), _count(0)		{}

	void			addValue(T val)
	{
		_min=val < _min ? val : _min;
		_max=val > _max ? val : _max;
		_total+=val;
	}
	void			addRef()					{ ++_count; }

	const string&	getName() const				{ return _name; }

	T				getMin() const				{ return _min; }
	T				getMax() const				{ return _max; }
	T				getTotal() const			{ return _total; }
	double			getMean() const				{ return (double)_total / (double)_count; }
};

template<typename T>
inline ostream& operator << (ostream& os, const Stat<T>& s)
{
	os << "Stats for " << s.getName() << ":" << endl << "  - min: " << s.getMin() << endl;
	os << "  - max: " << s.getMax() << endl << "  - total: " << s.getTotal() << endl << "  - mean: " << s.getMean() << endl;
	return os;
}

}

#endif