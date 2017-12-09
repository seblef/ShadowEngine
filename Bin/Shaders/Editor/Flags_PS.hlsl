

struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
	float4 cColor : COLOR0;
};

float4 PSMain(VS_OUTPUT psin) : SV_Target
{
	return psin.cColor;
}
