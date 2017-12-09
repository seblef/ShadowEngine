
#ifndef _SCENEINFOS_H_
#define _SCENEINFOS_H_

cbuffer SceneInfos : register(b0)
{
	float4x4			mViewProj;
	float4x4			mInvViewProj;
	float4x4			mInvProj;
	float4x4			mProj;
	float4x4			mView;
	float4				vEyePos;
	float2				vInvScreenSize;
	float				fTime;
	float				fSIPack;
};

cbuffer ObjectInfos : register(b1)
{
	float4x4			mWorld;
};

float4 GetWorldPosition(float2 vCoord, float fDepth)
{
	const float4 H=float4(vCoord.x * 2.0f - 1.0f,(1.0f - vCoord.y)*2.0f - 1.0f, fDepth,1.0f);
	const float4 D=mul(mInvViewProj,H);
	return D / D.w;
}

#endif