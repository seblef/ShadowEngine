
#include "MaterialEd.hlsl"


float4 PSMain(VS_OUTPUT psin) : SV_Target
{
	float4 cAldebo;
	float3 vNormal,cDiffuse,cSpecular,cFinalColor;
	float fShine,fSpecInt,fSpecFact;

	getAldeboAndNormal(psin,cAldebo,vNormal,fShine,fSpecInt);
	fSpecFact=getSpecularFact(vNormal,-vLightDir.xyz,psin.vWorldPos.xyz,fSpecInt,fShine);

	cDiffuse=cAldebo.rgb * saturate(-dot(vNormal,vLightDir.xyz));
	cSpecular=float3(fSpecFact,fSpecFact,fSpecFact);

	cFinalColor=(cDiffuse + cSpecular) * cLightColor.rgb * cLightColor.a;
	return float4(cFinalColor,cAldebo.a);
}
