
#ifndef _EFFECT_H_
#define _EFFECT_H_

class Effect
{
public:

	enum EffectType
	{
		EXPLOSION=0,

		EFFECT_COUNT
	};

private:

	EffectType				_type;

public:

	Effect(EffectType type) : _type(type)			{}
	virtual ~Effect()								{}

	EffectType				getType() const			{ return _type; }

	virtual void			load()					{}
	virtual void			unload()				{}
};

#endif