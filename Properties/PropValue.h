
#ifndef _PROPVALUE_H_
#define _PROPVALUE_H_

#include <Core.h>

using namespace Core;

enum PValueType
{
	PV_BOOL=0,
	PV_INT,
	PV_FLOAT,
	PV_STRING,
	PV_VECTOR,
	PV_COLOR,

	PV_COUNT
};

const char* const g_PValueNames[PV_COUNT] = {
	"bool",
	"int",
	"float",
	"string",
	"vector",
	"color"
};

class PropValue
{
protected:

	PValueType			_type;

	struct vec_col
	{
		float x, y, z, w;
	};

#define PTOVEC(v)			*((Vector3*)&v)
#define PTOVEC_C(v)			*((const Vector3*)&v)

#define PFROMVEC(v)			*((vec_col*)&v)
#define PFROMVEC_C(v)		*((const vec_col*)&v)

#define PTOCOL(c)			*((Color*)&c)
#define PTOCOL_C(c)			*((const Color*)&c)

#define PFROMCOL(c)			*((vec_col*)&c)
#define PFROMCOL_C(c)		*((const vec_col*)&c)

	union
	{
		bool			_bool;
		int				_int;
		float			_float;
		vec_col			_vector;
	};

	string			_string;

public:

	PropValue() : _type(PV_COUNT)							{}
	PropValue(bool b) : _bool(b), _type(PV_BOOL)			{}
	PropValue(int i) : _int(i), _type(PV_INT)				{}
	PropValue(float f) : _float(f), _type(PV_FLOAT)			{}
	PropValue(const Vector3& v) : _vector(PFROMVEC_C(v)), _type(PV_VECTOR)		{}
	PropValue(const Color& c) : _vector(PFROMCOL_C(c)), _type(PV_COLOR)			{}
	PropValue(const string& s) : _string(s), _type(PV_STRING)					{}
	PropValue(const char* s) : _string(s), _type(PV_STRING)						{}
	PropValue(const PropValue& p) : _type(p._type), _string(p._string)
	{
		_vector.x = p._vector.x;
		_vector.y = p._vector.y;
		_vector.z = p._vector.z;
		_vector.w = p._vector.w;
	}

	PropValue&			operator=(const PropValue& v)
	{
		_type = v._type;
		if (v._type == PV_STRING)
			_string = v._string;
		else
		{
			_vector.x = v._vector.x;
			_vector.y = v._vector.y;
			_vector.z = v._vector.z;
			_vector.w = v._vector.w;
		}
		return *this;
	}

	PValueType			getValueType() const			{ return _type; }

	bool				operator==(const PropValue& v) const
	{
		if (_type == v._type)
		{
			if (_type == PV_STRING)
				return _string == v._string;
			else if (_type == PV_COLOR)
				return _vector.x == v._vector.x && _vector.y == v._vector.y && _vector.z == v._vector.z && _vector.w == v._vector.w;
			else if (_type == PV_VECTOR)
				return _vector.x == v._vector.x && _vector.y == v._vector.y && _vector.z == v._vector.z;
			else
				return _int == v._int;
		}
		else
			return false;
	}

	bool				operator!=(const PropValue& v) const
	{
		return !((*this) == v);
	}

	PropValue&			operator=(bool b)
	{
		_bool = b;
		_type = PV_BOOL;
		return *this;
	}
	PropValue&			operator=(int i)
	{
		_int = i;
		_type = PV_INT;
		return *this;
	}
	PropValue&			operator=(float f)
	{
		_float = f;
		_type = PV_FLOAT;
		return *this;
	}
	PropValue&			operator=(const string& s)
	{
		_string = s;
		_type = PV_STRING;
		return *this;
	}
	PropValue&			operator=(const char* s)
	{
		_string = s;
		_type = PV_STRING;
		return *this;
	}
	PropValue&			operator=(const Vector3& v)
	{
		_vector.x = v.x;	_vector.y = v.y;	_vector.z = v.z;
		_type = PV_VECTOR;
		return *this;
	}
	PropValue&			operator=(const Color& c)
	{
		_vector.x = c.r;	_vector.y = c.g;	_vector.z = c.b;	_vector.w = c.a;
		_type = PV_COLOR;
		return *this;
	}

						operator bool() const				{ return _bool; }
						operator int() const				{ return _int; }
						operator float() const				{ return _float; }

						operator string&()					{ return _string; }
						operator const string&() const		{ return _string; }

						operator Vector3&()					{ return PTOVEC(_vector); }
						operator const Vector3&() const		{ return PTOVEC_C(_vector); }

						operator Color&()					{ return PTOCOL(_vector); }
						operator const Color&() const		{ return PTOCOL_C(_vector); }


};

inline ostream& operator << (ostream& os, const PropValue& pv)
{
	switch (pv.getValueType())
	{
	case PV_BOOL:
		os << (bool)pv;
		break;
		
	case PV_INT:
		os << (int)pv;
		break;

	case PV_FLOAT:
		os << (float)pv;
		break;

	case PV_STRING:
		os << (const string&)pv;
		break;

	case PV_VECTOR:
		os << ((const Vector3&)pv).x << " " << ((const Vector3&)pv).y << " " << ((const Vector3&)pv).z;
		break;

	case PV_COLOR:
		os << ((const Color&)pv).r << " " << ((const Color&)pv).g << " " << ((const Color&)pv).b << " " << ((const Color&)pv).a;
		break;
	}

	return os;
}


#endif