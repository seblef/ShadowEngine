
#include "stdafx.h"
#include "resource.h"
#include "MatAnimDlg.h"

int CMatAnimDlg::getMaterialAnimation(const string& animName) const
{
	for(int i=0;i<_material->getAnimationCount();++i)
		if(animName==_material->getAnimation(i)->getAnimationName())
			return i;

	return -1;
}
