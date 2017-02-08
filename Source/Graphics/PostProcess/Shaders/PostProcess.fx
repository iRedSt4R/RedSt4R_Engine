
struct VOut
{
	float4 Pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

//Texture2D inputTexture : register(t0);

VOut VSMain(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{	
	VOut output;

	output.Pos = inPos;
	output.texCoord = inTexCoord;

	return output;
}

float4 PSMain(VOut input) : SV_Target
{
	//return inputTexture.Sample(g_samPoint, input.texCoord);
	return float4(0.8f, 0.6f, 0.4f, 1.0f);
}
