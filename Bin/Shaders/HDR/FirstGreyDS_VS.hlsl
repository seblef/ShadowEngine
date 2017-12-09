
#include "HDRCommon.hlsl"


VS_OUTPUT VSMain(VS_INPUT vsin)
{
	VS_OUTPUT vsout;
	vsout.vPos=vsin.vPos;
	vsout.vTex=vsin.vUV;

	return vsout;
}

