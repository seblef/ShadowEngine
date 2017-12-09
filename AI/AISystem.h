
#ifndef _AISYSTEM_H_
#define _AISYSTEM_H_

#include <Core.h>
#include "AIAgent.h"
#include "AIMachine.h"

using namespace Core;

class AISystem : public TSingleton<AISystem>
{
protected:

	unsigned int			_currentAgentID;
	unsigned int			_currentMachineID;

	unsigned int			_now;

	typedef map<unsigned int, AIAgent*>		AgentMap;
	typedef map<unsigned int, AIMachine*>	MachineMap;
	typedef vector<unsigned int>			UIntVector;
	typedef vector<AIEvent>					EventVector;

	AgentMap				_agents;
	MachineMap				_machines;
	UIntVector				_destroyedAgents;

	EventVector				_events;
	EventVector				_delayedEvents;

	Vector3					_playerPos;
	Vector3					_playerDir;

	AIAgent*				getAgent(unsigned int agentID) const
	{
		AgentMap::const_iterator a(_agents.find(agentID));
		return a == _agents.end() ? 0 : a->second;
	}

	void					deleteDestroyedAgents();
	void					processEvents();
	void					processSingleEvent(const AIEvent& e);

public:

	AISystem();
	~AISystem();

	AIMachine*				getMachine(unsigned int machineID) const;
	unsigned int			getMachineID(const string& name) const;

	unsigned int			createAgent(unsigned int machineID, unsigned int stateID,
		IAICallback* callback);
	void					destroyAgent(unsigned int agentID);

	void					postEvent(AIEvent& e)
	{
		e.setDeliveryTime(_now);
		_events.push_back(e);
	}
	void					postDelayedEvent(AIEvent& e, unsigned int delay)
	{
		e.setDeliveryTime(_now + delay);
		_events.push_back(e);
	}

	unsigned int			getNow() const				{ return _now; }

	void					updateAI(const Vector3& playerPos,
		const Vector3& playerDir, float time);

	const Vector3&			getPlayerPos() const		{ return _playerPos; }
	const Vector3&			getPlayerDir() const		{ return _playerDir; }
};

#endif