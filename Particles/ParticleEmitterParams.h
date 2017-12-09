
#ifndef _PARTICLEEMITTERPARAMS_H_
#define _PARTICLEEMITTERPARAMS_H_

#include <Core.h>

using namespace Core;

class ParticleEmitterParams
{
	protected:

		float				_life;
		float				_mass;
		float				_friction;

		Color				_startColor, _endColor;
		Vector3				_velocity;
		Vector3				_startSize, _endSize;

	public:

		ParticleEmitterParams()	: _life(1.0f), _mass(0.5f), _friction(0.0f),
			_startColor(Color::White), _endColor(Color::Black),
			_velocity(Vector3::YAxisVector),
			_startSize(Vector3::UnitVector),
			_endSize(Vector3::UnitVector)		{}
		ParticleEmitterParams(float life, float mass, float friction,
			const Color& startColor, const Color& endColor,
			const Vector3& velocity,
			const Vector3& startSize, const Vector3& endSize) : _life(life), _mass(mass), _friction(friction),
			_startColor(startColor), _endColor(endColor),
			_velocity(velocity),
			_startSize(startSize), _endSize(endSize)			{}
		ParticleEmitterParams(const ParticleEmitterParams& p) : _life(p._life), _mass(p._mass), _friction(p._friction),
			_startColor(p._startColor), _endColor(p._endColor),
			_velocity(p._velocity),
			_startSize(p._startSize), _endSize(p._endSize)		{}

		void				setLife(float l)					{ _life=l; }
		void				setMass(float m)					{ _mass=m; }
		void				setFriction(float f)				{ _friction=f; }
		void				setStartColor(const Color& c)		{ _startColor=c; }
		void				setEndColor(const Color& c)			{ _endColor=c; }
		void				setVelocity(const Vector3& v)		{ _velocity=v; }
		void				setStartSize(const Vector3& s)		{ _startSize=s; }
		void				setEndSize(const Vector3& s)		{ _endSize=s; }

		float				getLife() const						{ return _life; }
		float				getMass() const						{ return _mass; }
		float				getFriction() const					{ return _friction; }
		const Color&		getStartColor() const				{ return _startColor; }
		const Color&		getEndColor() const					{ return _endColor; }
		const Vector3&		getVelocity() const					{ return _velocity; }
		const Vector3&		getStartSize() const				{ return _startSize; }
		const Vector3&		getEndSize() const					{ return _endSize; }

		Color&				getStartColor() 					{ return _startColor; }
		Color&				getEndColor() 						{ return _endColor; }
		Vector3&			getVelocity() 						{ return _velocity; }
		Vector3&			getStartSize()						{ return _startSize; }
		Vector3&			getEndSize() 						{ return _endSize; }
};

#endif