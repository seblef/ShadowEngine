
#include "../../SceneInfos.hlsl"
#include "../LightCommon.hlsl"
#include "../ShadowCommon.hlsl"


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
	float4 vNormalFull=tNormal.Sample(sPoint,psin.vTex);
	float3 vNormal=vNormalFull.xyz * 2.0f - 1.0f;

	float fDiffuse=-dot(vNormal,vLightDir.xyz);
	float fSpecularVal=0;
	float3 cDiffuse=0;
	float4 cColor=0;
	float shadowFact=1;

	cColor = tAldebo.Sample(sPoint, psin.vTex);

	if (fDiffuse > 0.0f)
	{
		float fDepth = tDepth.Sample(sPoint, psin.vTex).r;

		float fSpec, fSpecInt;
		fSpec = vNormalFull.a * 255.0f;
		fSpecInt = vNormalFull.a * 10.0f;

		float2 vCoord = (psin.vPos2.xy - float2(0.5f, 0.5f)) * vInvScreenSize;
		float4 vPos = GetWorldPosition(vCoord, fDepth);

		fSpecularVal = GetSpecularFact(vNormal, -vLightDir.xyz, vPos.xyz, fSpecInt, fSpec);

		shadowFact = GetShadowFactPoisson(vPos);
	}
	else
		fDiffuse = 0.0f;

	float occlusion = tHDAO.Sample(sPoint, psin.vTex).r;

	float3 lfact = cLightColor.rgb * cLightColor.a * shadowFact;

	float4 acc = tAccumulation.Sample(sPoint, psin.vTex);
	float3 ldiffuse = acc.rgb + lfact * fDiffuse;
	float3 lspec = lfact * fSpecularVal + getSpecularColor(acc.rgb, acc.a);

	float3 cfinal = ((ldiffuse + cAmbientLight.rgb) * cColor) * occlusion;// +lspec;

	return float4(cfinal.rgb + lspec,1.0f);
}