
#include "stdafx.h"
#include "SUtils.h"

void validMatrix(Core::Matrix4& m)
{
	for(int i=0;i<4;++i)
		for(int j=0;j<4;++j)
		{
			if(FloatMakePositive(m(i,j)) <= SEPSILON)			m(i,j)=0.0f;
			else if(FloatMakePositive(m(i,j) - 1.0f) <= SEPSILON) m(i,j)=1.0f;
			else if(FloatMakePositive(m(i,j) + 1.0f) <= SEPSILON) m(i,j)=-1.0f;
		}
}

void offsetMatrix(IGameNode *pNode, Core::Matrix4& mSOffset, Core::Matrix4& mSOffsetNoRot)
{
	GMatrix mObject(pNode->GetObjectTM());
	GMatrix mWorld(pNode->GetWorldTM());

	GMatrix mIWorld(mWorld.Inverse());
	GMatrix mOffset;

	//if(mIWorld.IsIdentity())
	//	mOffset.SetIdentity();
	//else
		mOffset=mObject * mIWorld;

	copyMatrix(mOffset,mSOffset);
	validMatrix(mSOffset);

	mSOffsetNoRot=mSOffset;
	mSOffsetNoRot(3,0)=0.0f;
	mSOffsetNoRot(3,1)=0.0f;
	mSOffsetNoRot(3,2)=0.0f;
} 