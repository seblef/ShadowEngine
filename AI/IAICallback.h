
#ifndef _IAICALLBACK_H_
#define _IAICALLBACK_H_

#include <Core.h>

using namespace Core;

class IAICallback
{
protected:

	bool							_walking;
	bool							_running;
	bool							_seePlayer;

	Vector3							_agentPos;
	Vector3							_agentDir;

public:

	IAICallback() : _walking(false), _running(false), _seePlayer(false),
		_agentPos(Vector3::NullVector), _agentDir(Vector3::ZAxisVector) {}
	virtual ~IAICallback()			{}

	virtual void					idle() = 0;
	virtual bool					walkTo(const Vector3& pos, const Vector3& dir) = 0;
	virtual bool					runTo(const Vector3& pos, const Vector3& dir) = 0;
	virtual void					goHome() = 0;
	virtual void					orientTo(const Vector3& dir) = 0;
	virtual void					fire() = 0;
	virtual bool					canFire() = 0;
	virtual float					getFireRange() = 0;

	bool							isWalking() const				{ return _walking; }
	bool							isRunning() const				{ return _running; }
	bool							isSeeingPlayer() const			{ return _seePlayer; }

	const Vector3&					getAgentPosition() const		{ return _agentPos; }
	const Vector3&					getAgentDirection() const		{ return _agentDir; }
};

#endif