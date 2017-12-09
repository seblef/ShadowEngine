

struct VS_INPUT
{
	float4 vPos : POSITION;
	float4 cColor : COLOR0;
	float2 vUV : TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float2 vUV : TEXCOORD0;

};


VS_OUTPUT VSMain(VS_INPUT vsin)
{
	VS_OUTPUT vsout;

	vsout.vPos=vsin.vPos;
	vsout.vUV=vsin.vUV;

	return vsout;
}

