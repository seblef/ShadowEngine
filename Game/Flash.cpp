
#include "stdafx.h"
#include "Flash.h"

Flash::Flash(const Color& c, float ascend, float stable, float descend,
	float range, const Vector3& pos) :
	_color(c), _time(ascend), _state(FLASH_ASCENDING)
{
	_durations[0] = ascend;
	_durations[1] = stable;
	_durations[2] = descend;

	LightCreate_t lc;
	lc._color = c;
	lc._castShadows = false;
	lc._range = range;
	lc._world.createTranslate(pos.x, pos.y, pos.z);
	_rLight = new Light(Light::LT_OMNI, lc);

	Renderer::getSingletonRef().addRenderable(_rLight);
}

Flash::~Flash()
{
	Renderer::getSingletonRef().remRenderable(_rLight);
	delete _rLight;
}

void Flash::update(float time)
{
	_time -= time;

	if (_time < 0.0f)
	{
		if (_state == FLASH_DESCENDING)
		{
			_dead = true;
			return;
		}
		else
		{
			++_state;
			_time += _durations[_state];
		}
	}

	Color res;
	float fact = 1.0f - _time / _durations[_state];

	switch (_state)
	{
	case FLASH_ASCENDING:
		AnimVector4Evaluator::evaluate(Color::Black, _color, fact, res);
		break;

	case FLASH_STABLE:
		res = _color;
		break;

	case FLASH_DESCENDING:
		AnimVector4Evaluator::evaluate(_color, Color::Black, fact, res);
		break;
	}

	_rLight->setColor(res);
}