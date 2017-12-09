
#include "stdafx.h"
#include "MaterialUVAnimation.h"

MaterialUVAnimation::MaterialUVAnimation(Material* m, ScriptFile& sf) : MaterialAnimation(m),
	_offsetPerSec(Vector2::NullVector), _rotPerSec(0)
{
	string token(sf.getToken());

	while(sf.good() && token!=_T("end_anim"))
	{
		if(token=="rotation")
			_rotPerSec=stof(sf.getToken());
		else if(token=="offset")
		{
			_offsetPerSec.x=stof(sf.getToken());
			_offsetPerSec.y=stof(sf.getToken());
		}

		token=sf.getToken();
	}
}