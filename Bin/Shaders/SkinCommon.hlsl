
#ifndef _SKINCOMMON_H_
#define _SKINCOMMON_H_

struct VS_INPUTSkin
{
	float4 vPos : POSITION;
	float4 fWeights : BLENDWEIGHT;
	float4 fIndices : BLENDINDICES;
	float3 vNorm : NORMAL;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;
	float2 vTex0 : TEXCOORD0;
};

cbuffer BonesInfos : register(b3)
{
	float4x4	mBones[64];
};

float4 SGetLocalPosition(VS_INPUTSkin input, int4 iIndices)
{
	const float4 vPos=mul(input.vPos,mWorld);

	float4 v1=mul(mBones[iIndices[0]],vPos);
	float4 v2=mul(mBones[iIndices[1]],vPos);
	float4 v3=mul(mBones[iIndices[2]],vPos);

	v1*=input.fWeights[2];
	v2*=input.fWeights[1];
	v3*=input.fWeights[0];

	return v1+v2+v3;
}

float3 SGetLocalNormalBinormalTangent(VS_INPUTSkin input, float3 vSrc, float3x3 mRot, int4 iIndices)
{
	const float3 v=mul(mRot,vSrc);

	float3 v1=mul((const float3x3)mBones[iIndices[0]],v);
	float3 v2=mul((const float3x3)mBones[iIndices[1]],v);
	float3 v3=mul((const float3x3)mBones[iIndices[2]],v);

	v1*=input.fWeights[2];
	v2*=input.fWeights[1];
	v3*=input.fWeights[0];

	return normalize(v1+v2+v3);
}

#endif