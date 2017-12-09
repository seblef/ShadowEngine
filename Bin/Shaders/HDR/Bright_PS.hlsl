
#include "HDRCommon.hlsl"

static const int2 i2Offset[4] = {
	{ 0, 0 },
	{ 0, 1 },
	{ 1, 0 },
	{ 1, 1 }
};


float4 PSMain(PS_INPUT psin) : SV_Target
{
	int3 i3UV = int3((int2)(psin.vPos2.xy * 2.0f), 0);
	float3 color=0;
	float grey;

	for (int i = 0; i < 4; ++i)
		color += tSrc.Load(i3UV, i2Offset[i]);

	color *= 0.25f;
	color=max(color - float4(g_fBrightThreshold,g_fBrightThreshold,g_fBrightThreshold,1.0f),0.0f);
	return float4(color,1);
}
