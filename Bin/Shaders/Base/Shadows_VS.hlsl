
#include "../SceneInfos.hlsl"
#include "ShadowsCommon.hlsl"

struct VS_INPUT
{
	float4 vPos : POSITION;
	float3 vNorm : NORMAL;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;
	float2 vTex0 : TEXCOORD0;
};


VS_OUTPUT VSMain(VS_INPUT vsin)
{
	VS_OUTPUT vsout;

	vsout.vPos=mul(mViewProj,mul(mWorld,vsin.vPos));
	vsout.vZ.x=vsout.vPos.z;
	vsout.vZ.y=vsout.vPos.w;

	return vsout;
}


