
#ifndef _CORECOMMON_H_
#define _CORECOMMON_H_

//			Min / Max

#define SMIN(x,y)			((x) < (y) ? (x) : (y))
#define SMAX(x,y)			((x) > (y) ? (x) : (y))


template<typename T>
inline T smin(T t1, T t2)		{ return t1 < t2 ? t1 : t2; }

template<typename T>
inline T smax(T t1, T t2)		{ return t1 > t2 ? t1 : t2; }

template<typename T>
inline void sclamp(T& t, T cmin, T cmax)
{
	t=smin(t,cmax);
	t=smax(t,cmin);
}

template<typename T>
inline void sminmax(T t1, T t2, T& _min, T& _max)
{
	_min=smin(t1,t2);
	_max=smax(t1,t2);
}

template<typename T>
inline const T& smin_r(const T& t1, const T& t2)		{ return t1 < t2 ? t1 : t2; }

template<typename T>
inline const T& smax_r(const T& t1, const T& t2)		{ return t1 > t2 ? t1 : t2; }


//			Swap

template<typename T>
inline void sswap(T& t1, T& t2)		{ T temp=t1; t1=t2; t2=temp; }

//			World bounds

#define	WBOUNDS_P			(+1e23f)
#define WBOUNDS_N			(-1e23f)

#define SEPSILON			(1e-5f)

#endif