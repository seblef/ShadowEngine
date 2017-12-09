
#ifndef _LIGHTINFOS_H_
#define _LIGHTINFOS_H_

cbuffer LightInfos : register(b2)
{
	float4		cLightColor;
	float4		vLightPos;
	float4		vLightDir;
	float		fNearAngle;
	float		fFarAngle;
	float		fRange;
	float		fInvRange;
	float4x4	mAreaViewProj;		//		For area lights
};

float GetRangeFact(float fDistance)
{
	float fAtten=(max(fRange - fDistance,0.0f)) * fInvRange;
	fAtten=fAtten*fAtten*(3.0f - 2.0f * fAtten);

	return fAtten;
}

#endif