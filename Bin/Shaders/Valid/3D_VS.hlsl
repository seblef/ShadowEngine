

struct VS_INPUT
{
	float4 vPos : POSITION;
	float3 vNorm : NORMAL;
	float3 vTangent : TANGENT;
	float3 vBinormal : BINORMAL;
	float2 vTex0 : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 vPos : POSITION;
};



VS_OUTPUT VSMain(VS_INPUT vsin)
{
	VS_OUTPUT vsout;

	vsout.vPos=vsin.vPos;
	return vsout;
}


