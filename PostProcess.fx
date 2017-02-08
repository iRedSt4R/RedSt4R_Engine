
struct VOut
{
	float4 Pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};

Texture2D inputTexture : register(t0);
SamplerState g_samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

float texelSize = 1.0f / 1366.0f/2.0f;
//float texelSize = 1.0f / 768.0f;

VOut VSMain(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{	
	VOut output;

	output.Pos = inPos;
	output.texCoord = inTexCoord;
	output.texCoord1 = inTexCoord + float2(texelSize * -4.0f, 0.0f);
	output.texCoord2 = inTexCoord + float2(texelSize * -3.0f, 0.0f);
	output.texCoord3 = inTexCoord + float2(texelSize * -2.0f, 0.0f);
	output.texCoord4 = inTexCoord + float2(texelSize * -1.0f, 0.0f);
	output.texCoord5 = inTexCoord + float2(texelSize *  0.0f, 0.0f);
	output.texCoord6 = inTexCoord + float2(texelSize *  1.0f, 0.0f);
	output.texCoord7 = inTexCoord + float2(texelSize *  2.0f, 0.0f);
	output.texCoord8 = inTexCoord + float2(texelSize *  3.0f, 0.0f);
	output.texCoord9 = inTexCoord + float2(texelSize *  4.0f, 0.0f);

	return output;
}

float4 PSMain(VOut input) : SV_Target
{
	//float4 result = inputTexture.Sample(g_samPoint, input.texCoord);

	float weight0, weight1, weight2, weight3, weight4;
	float normalization;
	float4 color;

	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine horizontal pixels to the color by the specific weight of each.
	color += inputTexture.Sample(g_samPoint, input.texCoord1) * weight4;
	color += inputTexture.Sample(g_samPoint, input.texCoord2) * weight3;
	color += inputTexture.Sample(g_samPoint, input.texCoord3) * weight2;
	color += inputTexture.Sample(g_samPoint, input.texCoord4) * weight1;
	color += inputTexture.Sample(g_samPoint, input.texCoord5) * weight0;
	color += inputTexture.Sample(g_samPoint, input.texCoord6) * weight1;
	color += inputTexture.Sample(g_samPoint, input.texCoord7) * weight2;
	color += inputTexture.Sample(g_samPoint, input.texCoord8) * weight3;
	color += inputTexture.Sample(g_samPoint, input.texCoord9) * weight4;

	color.a = 1.0f;

	return color;
}
