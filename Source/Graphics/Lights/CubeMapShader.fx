struct VOut
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

TextureCube CubeMapTexture : register(t3);
SamplerState ObjSamplerState;

cbuffer cbCubeMap: register(b2)
{
	float4x4 WVP;
	float4x4 World;
};

VOut VSMain(float3 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut output = (VOut)0;

	output.Pos = mul(float4(inPos, 1.0f), WVP).xyww;

	output.texCoord = inPos;

	return output;
}

float4 PSMain(VOut input) : SV_Target
{
	return CubeMapTexture.Sample(ObjSamplerState, input.texCoord);
}