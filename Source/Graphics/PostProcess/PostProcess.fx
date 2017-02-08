Texture2D inputTexture : register(t0);

struct VOut
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

SamplerState g_samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

VOut VSMain(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{	
	VOut output = (VOut)0;

	output.Pos = inPos;
	output.texCoord = inTexCoord;

	return output;
}

float4 PSMain(VOut input) : SV_Target
{
	return inputTexture.Sample(g_samPoint, input.texCoord);
}
