
#ifndef _ANIMEXPORTER_H_
#define _ANIMEXPORTER_H_

#include "IExporter.h"
#include "../Game/ActorFileFormat.h"

class AnimExporter : public IExporter
{
protected:

	vector<IGameNode*>			_nodes;
	vector<IGameNode*>			_nullNodes;
	Core::AnimPRSMultiTrack*	_track;
	int							_frameRate;

	bool				isSkinned() const;
	void				collectNodes(IGameNode *n, IGameObject::ObjectTypes type);
	void				processPRS();
	void				save(const string& name);

public:

	AnimExporter();
	~AnimExporter();

	void				doExport(const string& name);
};

#endif