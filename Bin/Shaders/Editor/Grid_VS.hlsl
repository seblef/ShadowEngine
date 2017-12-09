
#include "../SceneInfos.hlsl"

struct VS_INPUT
{
	float4 vPos : POSITION;
	float4 cColor : COLOR0;
	float2 uv : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
};


VS_OUTPUT VSMain(VS_INPUT vsin)
{
	VS_OUTPUT vsout;

	vsout.vPos=mul(mViewProj,vsin.vPos);
	return vsout;
}


