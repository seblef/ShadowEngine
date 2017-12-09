
#include "HDRCommon.hlsl"


SamplerState sLinear : register(s2);

Texture2D tLum : register(t1);
Texture2D tBloom : register(t2);

float4 PSMain(PS_INPUT psin) : SV_Target
{
	float4 cColor=tSrc.Load(int3((int2)psin.vPos,0));
	float4 fLuminance=tLum.Load(int3(0,0,0));
	float4 fBloom=tBloom.Sample(sLinear,psin.vTex) * 0.5f;

	float fLP=(g_fExposure / fLuminance.r) * dot(cColor.rgb,g_vLuminanceVector);
	float fLmSqr=fLuminance.g * (1.0f + g_fGaussianScalar);
	fLmSqr*=fLmSqr;

	float fToneScalar=(fLP * (1.0f + (fLP / (fLmSqr + 0.001f)))) / (1.0f + fLP);

	return (cColor * fToneScalar) + fBloom;
}