
#include "../../SceneInfos.hlsl"
#include "../LightCommon.hlsl"

cbuffer LightInfos : register(b2)
{
	float4			cLightColor;
	float4			vLightDir;
	float4			cAmbientLight;
};

struct PS_INPUT
{
	float4 vPos : SV_POSITION;
	float2 vTex : TEXCOORD0;
	float4 vPos2 : SV_Position;
};

float4 PSMain(PS_INPUT psin) : SV_Target
{
	float4 acc = tAccumulation.Sample(sPoint, psin.vTex);

	float3 spec = getSpecularColor(acc.rgb, acc.a);
	float3 light = cAmbientLight.rgb + acc.rgb;
	float3 aldebo = tAldebo.Sample(sPoint, psin.vTex);

	float occlusion = tHDAO.Sample(sPoint, psin.vTex).r;

	return float4(aldebo * light * occlusion, 1.0f);
}