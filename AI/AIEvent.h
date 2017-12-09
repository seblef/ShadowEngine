
#ifndef _AIEVENT_H_
#define _AIEVENT_H_

enum AIEventType
{
	AIEVT_ENTER=0,
	AIEVT_EXIT,
	AIEVT_UPDATE,

	AIEVT_SEEPLAYER,
	AIEVT_LOSTPLAYER,

	AIEVT_ENDMOVE,

	AIEVT_HIT,
	AIEVT_DEAD,

	AIEVT_COUNT
};


class AIEvent
{
protected:

	AIEventType			_name;
	unsigned int		_senderID;
	unsigned int		_recieverID;
	unsigned int		_deliveryTime;
	unsigned int		_stateID;

	int					_intParam;
	void*				_pParam;

public:

	AIEvent(AIEventType type, unsigned int senderID,
		unsigned int recieverID, unsigned int stateID=0xffffffff) : _name(type), _senderID(senderID), _recieverID(recieverID),
		_deliveryTime(0), _stateID(stateID), _intParam(0), _pParam(0)				{}
	AIEvent(AIEventType type, unsigned int senderID,
		unsigned int recieverID,
		int intParam,
		void *pParam, unsigned int stateID = 0xffffffff) : _name(type), _senderID(senderID), _recieverID(recieverID),
		_deliveryTime(0), _intParam(intParam), _pParam(pParam), _stateID(stateID)		{}
	AIEvent(const AIEvent& e) : _name(e._name), _senderID(e._senderID), _recieverID(e._recieverID),
		_deliveryTime(e._deliveryTime), _intParam(e._intParam), _pParam(e._pParam), _stateID(e._stateID)		{}
	~AIEvent()																	{}

	AIEventType			getName() const					{ return _name; }
	unsigned int		getSenderID() const				{ return _senderID; }
	unsigned int		getRecieverID() const			{ return _recieverID; }
	unsigned int		getDeliveryTime() const			{ return _deliveryTime; }
	unsigned int		getStateID() const				{ return _stateID; }

	int					getIntParam() const				{ return _intParam; }
	void*				getVoidParam() const			{ return _pParam; }

	void				setType(AIEventType type)		{ _name = type; }
	void				setDeliveryTime(unsigned int t)	{ _deliveryTime = t; }
};

#endif