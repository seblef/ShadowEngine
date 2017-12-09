
#ifndef _FLASH_H_
#define _FLASH_H_

#include "Action.h"
#include <StdRenderer.h>

#define FLASH_ASCENDING				0
#define FLASH_STABLE				1
#define FLASH_DESCENDING			2

class Flash : public Action
{
protected:

	float			_durations[3];
	int				_state;
	float			_time;
	Color			_color;

	Light*			_rLight;

public:

	Flash(const Color& c, float ascendDuration, float stableDuration,
		float descendDuration, float range, const Vector3& position);
	~Flash();

	void			update(float time);
};

#endif