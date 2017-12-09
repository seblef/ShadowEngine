
#include "../SceneInfos.hlsl"

struct VS_INPUT
{
	float4 pos :		POSITION;
	float4 color :		COLOR0;
	float2 uv :			TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 pos :		SV_POSITION;
	float4 color :		COLOR0;
	float2 uv :			TEXCOORD0;
};




VS_OUTPUT VSMain(VS_INPUT vsin)
{
	VS_OUTPUT vsout;

	vsout.pos=mul(mViewProj,vsin.pos);
	vsout.uv=vsin.uv;
	vsout.color=vsin.color;

	return vsout;
}

