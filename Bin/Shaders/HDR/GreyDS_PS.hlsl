
#include "HDRCommon.hlsl"

static const int2 i2Offset[9]={
	{-1,-1},
	{ 0,-1},
	{ 1,-1},
	{-1, 0},
	{ 0, 0},
	{ 1, 0},
	{-1, 1},
	{ 0, 1},
	{ 1, 1}
};


float4 PSMain(PS_INPUT psin) : SV_Target
{
	float fAverage=0.0f;
	float4 cColor;
	float fMax=-1e20f;

	int3 i3UV=int3((int2)(psin.vPos2.xy * 3.0f),0);

	for(int i=0;i<9;++i)
	{
		cColor=tSrc.Load(i3UV,i2Offset[i]);
		fAverage+=cColor.r;
		fMax=max(fMax,cColor.g);
	}

	fAverage/=9.0f;
	return float4(fAverage,fMax,0.0f,0.0f);
}