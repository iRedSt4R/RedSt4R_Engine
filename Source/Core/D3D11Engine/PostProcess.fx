Texture2D inputTexture : register(t0);

float VSMain(float inPos : test)
{
	return test;
}

float4 PSMain(VOut input) : SV_Target
{
	return float4(0.4, 0.6, 0.5, 1);
}