#include "FXAA.fx"

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

float rand_1_05(in float2 uv)
{
	float2 noise = (frac(sin(dot(uv, float2(12.9898, 78.233)*2.0)) * 43758.5453));
	return abs(noise.x + noise.y) * 0.5;
}


float texelSize = 1.0f / 1366.0f;
//float texelSize = 1.0f / 768.0f;

VOut VSMain(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{	
	VOut output;

	output.Pos = inPos;
	output.texCoord = inTexCoord;

	return output;
}

float4 BlurColor(float2 Coord, Texture2D inputTexture, float MipBias)
{
	float2 sc = float2(1366.0f, 768.0f);
	float2 TexelSize = MipBias / sc.xy;

	float4  Color = inputTexture.Sample(g_samPoint, Coord, MipBias);
	Color += inputTexture.Sample(g_samPoint, Coord + float2(TexelSize.x, 0.0), MipBias);
	Color += inputTexture.Sample(g_samPoint, Coord + float2(-TexelSize.x, 0.0), MipBias);
	Color += inputTexture.Sample(g_samPoint, Coord + float2(0.0, TexelSize.y), MipBias);
	Color += inputTexture.Sample(g_samPoint, Coord + float2(0.0, -TexelSize.y), MipBias);
	Color += inputTexture.Sample(g_samPoint, Coord + float2(TexelSize.x, TexelSize.y), MipBias);
	Color += inputTexture.Sample(g_samPoint, Coord + float2(-TexelSize.x, TexelSize.y), MipBias);
	Color += inputTexture.Sample(g_samPoint, Coord + float2(TexelSize.x, -TexelSize.y), MipBias);
	Color += inputTexture.Sample(g_samPoint, Coord + float2(-TexelSize.x, -TexelSize.y), MipBias);

	return Color / 9.0;
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

	// ----------------------- Chromatic Abberation --------------------//
	float2 Screen = float2(1366.0, 726.0);

	float2 uv = input.texCoord.xy / Screen.xy;

	float amount = 0.00;

	float2 sinvalue = 1.115f;

	amount = (1.0 + sin(sinvalue*6.0)) * 0.5;
	amount *= 1.0 + sin(sinvalue*16.0) * 0.5;
	amount *= 1.0 + sin(sinvalue*19.0) * 0.5;
	amount *= 1.0 + sin(sinvalue*27.0) * 0.5;
	amount = pow(amount, 3.0);

	amount *= 0.06;

	float3 col;
	col.r = inputTexture.Sample(g_samPoint, float2(input.texCoord.x + amount, input.texCoord.y)).r;
	col.g = inputTexture.Sample(g_samPoint, input.texCoord).g;
	col.b = inputTexture.Sample(g_samPoint, float2(input.texCoord.x - amount, input.texCoord.y)).b;

	col *= (1.0 - amount * 0.5);

	float4 fragColor = float4(col, 1.0);
	// ----------------------- END Chromatic Abberation --------------------//

	// -------------------------- Simple Radial Blur -------------------------//
	int nsamples = 20;
	float2 center = float2(0.5,0.5);
	float blurStart = 1.0;
	float blurWidth = 0.025;


	float2 uvv = input.texCoord;

	uvv -= center;
	float precompute = blurWidth * (1.0 / float(nsamples - 1));

	float4 colorr = float4(0,0,0,0);
	for (int i = 0; i < nsamples; i++)
	{
		float scale = blurStart + (i* precompute);
		colorr += inputTexture.Sample(g_samPoint, uvv * scale + center);
	}


	colorr /= float(nsamples);

	//fragColor = color;
	colorr.w = 1.0f;
	// -------------------------- END Simple Radial Blur -------------------------//
	
	float Threshold = 0.2;
	float Intensity = 0.8;
	float BlurSize = 2.5;

	float2 uvvv = input.texCoord;
	float4 ccolor = inputTexture.Sample(g_samPoint, uvvv);
	float4 Highlight = clamp(BlurColor(uvvv, inputTexture, BlurSize) - Threshold, 0.0, 1.0)*1.0 / (1.0 - Threshold);

	float4 ccc = 1.0 - (1.0 - ccolor)*(1.0 - Highlight*Intensity);



	return float4(fxaaColor, 1.0);
}
