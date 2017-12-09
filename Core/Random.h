
#ifndef _SRANDOM_H_
#define _SRANDOM_H_

#include "Color.h"

namespace Core
{

#define RAND ((float) rand() / (float) RAND_MAX)

inline void randomVector(Vector3 &result, const Vector3 &start, const Vector3 &end)
{
	result[0] = start[0] + ((end[0] - start[0]) * RAND);
	result[1] = start[1] + ((end[1] - start[1]) * RAND);
	result[2] = start[2] + ((end[2] - start[2]) * RAND);
}

inline void randomCounterVector(Vector3 &result, const Vector3 &current,
	const Vector3 &start, const Vector3 &end,
	float life)
{
	randomVector(result, start, end);
	result -= current;
	result /= life;
}

inline void randomColor(Color &result, const Color &start, const Color &end)
{
	for (int i = 0; i<4; ++i)
		result[i] = start[i] + ((end[i] - start[i]) * RAND);
}

inline void randomCounterColor(Color &result, const Color &current,
	const Color &start, const Color &end,
	float life)
{
	randomColor(result, start, end);
	result -= current;
	result /= life;
}

inline float randomScalar(float start, float end)
{
	return start + ((end - start) * RAND);
}

inline float randomCounterScalar(float current,
	float start, float end,
	float life)
{
	float result = randomScalar(start, end);
	result -= current;
	result /= life;
	return result;
}

};

#endif