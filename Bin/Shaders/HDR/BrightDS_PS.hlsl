
#include "HDRCommon.hlsl"

static const int2 i2Offset[16]={
	{0,0},
	{0,1},
	{0,2},
	{0,3},
	{1,0},
	{1,1},
	{1,2},
	{1,3},
	{2,0},
	{2,1},
	{2,2},
	{2,3},
	{3,0},
	{3,1},
	{3,2},
	{3,3}
};


float4 PSMain(PS_INPUT psin) : SV_Target
{
	float4 f4Average=0.0f;

	int3 i3UV=int3((int2)(psin.vPos2.xy * 4.0f),0);

	for(int i=0;i<16;++i)
	{
		f4Average+=tSrc.Load(i3UV,i2Offset[i]);
	}

	f4Average/=16.0f;
	return f4Average;
}