
#ifndef _MATHUTILS_H_
#define _MATHUTILS_H_

#define _USE_MATH_DEFINES
#include <math.h>

#define SQREPSILON				  0.000001f


inline unsigned long& FloatBits(float& f)
{
	return *reinterpret_cast<unsigned long*>(&f);
}

inline unsigned long const& FloatBits(const float& f )
{
	return *reinterpret_cast<unsigned long const*>(&f);
}

inline float BitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

inline bool IsFinite(float f)
{
	return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
}

inline unsigned long FloatAbsBits(float f)
{
	return FloatBits(f) & 0x7FFFFFFF;
}

inline float FloatMakeNegative(float f)
{
	return BitsToFloat(FloatBits(f) | 0x80000000);
}

inline float FloatMakePositive(float f )
{
	return BitsToFloat(FloatBits(f) & 0x7FFFFFFF);
}

inline float FastInvSqrt( float fN )
{
	unsigned int iN;
	float fTemp;

	fTemp  = fN;
	iN     = *(long*)&fTemp;
	iN     = 0x5f3759df - ( iN >> 1 );
	fTemp  = *(float*)&iN;
	fTemp  = fTemp * ( 1.5f - ( fN * 0.5f * fTemp * fTemp ) );

	return fTemp;
}

#endif