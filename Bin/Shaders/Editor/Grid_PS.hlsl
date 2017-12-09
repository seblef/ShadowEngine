

struct VS_OUTPUT
{
	float4 vPos : SV_POSITION;
};

float4 PSMain(VS_OUTPUT psin) : SV_Target
{
	return float4(0.2f,0,0,1);
}
