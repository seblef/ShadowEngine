
#ifndef _ACTORFILEFORMAT_H_
#define _ACTORFILEFORMAT_H_

#include <Core.h>

namespace File
{
#define ACTOR_MAX_BONES			64

#define ACTOR_HEADER			(('S' << 24) | ('A' << 16) | ('C' << 8) | ('T'))
#define ACTORANIM_HEADER		(('A' << 24) | ('C' << 16) | ('A' << 8) | ('N'))

struct ActorFileHeader
{
	unsigned int			_header;
	int						_vxCount;
	int						_triCount;
	int						_subCount;
	int						_boneCount;
	int						_nullNodeCount;
	Core::BBox3				_box;
	Core::Matrix4			_invSkinMatrix;
	char					_bonesNames[ACTOR_MAX_BONES][16];
};

struct ActorFileSub
{
	int						_triCount;
	int						_triOffset;
	char					_name[16];
};

struct ActorFileNullNode
{
	int						_id;
	char					_name[16];
};

struct ActorAnimFileHeader
{
	unsigned int			_header;
	int						_keyCount;
	int						_trackCount;
};


};

#endif