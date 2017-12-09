
#include "HDRCommon.hlsl"

static const int2 i2Offset[4] = {
	{ 0, 0 },
	{ 0, 1 },
	{ 1, 0 },
	{ 1, 1 }
};

float4 PSMain(PS_INPUT psin) : SV_Target
{
	float fMax=-1e20f;
	float4 cColor;

	float2 f2UV=psin.vPos2.xy / 243.0f;// * g_vGreyScaleUV;
	float fGray=0.0f;
	float grey_val;

	for (int i = 0; i < 4; ++i)
	{
		cColor = tSrc.Sample(sPoint, f2UV, i2Offset[i]);
		grey_val = dot(cColor.rgb, g_vLuminanceVector);
		fGray += grey_val;

		fMax = max(fMax, grey_val);
	}

	return float4(fGray*0.25f,fMax,0.0f,0.0f);
}