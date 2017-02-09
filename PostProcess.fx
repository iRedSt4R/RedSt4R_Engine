#include "FXAA.fx"

float GridValue(float2 fragCoord)
{
	float2 pixelCount = vec2(16.0, 16.0);
	float2 coord = fragCoord.xy;
	float2 gridCoord = frac(coord / pixelCount);
	float2 pixCoord = gridCoord * pixelCount;
	float2 lineVal = step(pixCoord, float2(1.0, 1.0));
	float val = max(lineVal.x, lineVal.y);
	return val;
}

struct VOut
{
	float4 Pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

Texture2D inputTexture : register(t0);
SamplerState samp : register(s0);
SamplerState g_samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

float texelSize = 1.0f / 1366.0f;
//float texelSize = 1.0f / 768.0f;

VOut VSMain(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{	
	VOut output;

	output.Pos = inPos;
	output.texCoord = inTexCoord;

	return output;
}

float4 PSMain(VOut input) : SV_Target
{
	float4 Color = inputTexture.Sample(g_samPoint, input.texCoord);
	// ----------------------- FXAA -------------------------------//
	FxaaTex textaa;
	textaa.smpl = g_samPoint;
	textaa.tex = inputTexture;
	float2 sc = float2(1 / 1366.0f, 1 / 726.0f);
	float3 fxaaColor = FxaaPixelShader(input.texCoord, textaa, sc);
	fxaaColor = pow(fxaaColor, 1 / 1.3);
	// ----------------------- END FXAA ---------------------------//

	
	return float4(fxaaColor, 1.0);
}
