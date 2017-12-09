
#include "HDRCommon.hlsl"

static const int2 i2Offset[9]={
	{0,-4},
	{0,-3},
	{0,-2},
	{0,-1},
	{0, 0},
	{0, 1},
	{0, 2},
	{0, 3},
	{0, 4}
};


float4 PSMain(PS_INPUT psin) : SV_Target
{
	float4 cColor=0.0f;

	int3 i3UV=int3((int2)(psin.vPos2.xy),0);

	for(int i=0;i<9;++i)
	{
		cColor+=tSrc.Load(i3UV,i2Offset[i]) * g_fGaussianWeights[i];
	}

	return cColor;
}