
#include "ShadowsCommon.hlsl"

float4 PSMain(VS_OUTPUT psin) : SV_Target
{
	return float4(psin.vZ.x / psin.vZ.y,0.0f,0.0f,1.0f);
}
