
#include "../SceneInfos.hlsl"
#include "../Common.hlsl"


PS_OUTPUT PSMain(VS_OUTPUT psin)
{
	float4 cColor;
	float3 vNormal;
	float fShine,fSpecInt=vShininess.y;
	float4 cSpec;
	PS_OUTPUT psout;

	cColor=cDiffuse * tDiffuse.Sample(sLinear,psin.vTexBase);

	float4 sTex=tSpecular.Sample(sLinear,psin.vTexBase);
	cSpec.rgb=cSpecular.rgb * sTex.rgb;
	fShine=(sTex.a * 255.0f + vShininess.x) * 0.5f;

	//vNormal=normalize((tNormal.Sample(sLinear,psin.vTexBase).rgb - 0.5f) * 2.0f);
	//vNormal=mul(psin.mTangent,vNormal);

	vNormal=psin.vNormal;

	psout.cColor=cColor;
	psout.cNormal=float4(vNormal.xyz,0.0f) * 0.5f + 0.5f;

	psout.cColor.a=fShine / 255.0f;
	psout.cNormal.a=fSpecInt;

	return psout;
}
