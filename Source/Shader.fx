float LightingFuncGGX_D(float dotNH, float roughness)
{
	float alpha = roughness*roughness;
	float alphaSqr = alpha*alpha;
	float pi = 3.14159f;
	float denom = dotNH * dotNH *(alphaSqr - 1.0) + 1.0f;

	float D = alphaSqr / (pi * denom * denom);
	return D;
}

float2 LightingFuncGGX_FV(float dotLH, float roughness)
{
	float alpha = roughness*roughness;

	// F
	float F_a, F_b;
	float dotLH5 = pow(1.0f - dotLH, 5);
	F_a = 1.0f;
	F_b = dotLH5;

	// V
	float vis;
	float k = alpha / 2.0f;
	float k2 = k*k;
	float invK2 = 1.0f - k2;
	vis = rcp(dotLH*dotLH*invK2 + k2);

	return float2(F_a*vis, F_b*vis);
}

float LightingFuncGGX_OPT3(float3 N, float3 V, float3 L, float roughness, float F0)
{
	float3 H = normalize(V + L);

	float dotNL = saturate(dot(N, L));
	float dotLH = saturate(dot(L, H));
	float dotNH = saturate(dot(N, H));

	float D = LightingFuncGGX_D(dotNH, roughness);
	float2 FV_helper = LightingFuncGGX_FV(dotLH, roughness);
	float FV = F0*FV_helper.x + (1.0f - F0)*FV_helper.y;
	float specular = dotNL * D * FV;

	return specular;
}

float LightingFuncGGX_OPT2(float3 N, float3 V, float3 L, float roughness, float F0)
{
	float alpha = roughness*roughness;

	float3 H = normalize(V + L);

	float dotNL = saturate(dot(N, L));

	float dotLH = saturate(dot(L, H));
	float dotNH = saturate(dot(N, H));

	float F, D, vis;

	// D
	float alphaSqr = alpha*alpha;
	float pi = 3.14159f;
	float denom = dotNH * dotNH *(alphaSqr - 1.0) + 1.0f;
	D = alphaSqr / (pi * denom * denom);

	// F
	float dotLH5 = pow(1.0f - dotLH, 5);
	F = F0 + (1.0 - F0)*(dotLH5);

	// V
	float k = alpha / 2.0f;
	float k2 = k*k;
	float invK2 = 1.0f - k2;
	vis = rcp(dotLH*dotLH*invK2 + k2);

	float specular = dotNL * D * F * vis;
	return specular;
}

float G1V(float dotNV, float k)
{
	return 1.0f / (dotNV*(1.0f - k) + k);
}

float LightingFuncGGX_REF(float3 N, float3 V, float3 L, float roughness, float F0)
{
	float alpha = roughness*roughness;

	float3 H = normalize(V + L);

	float dotNL = saturate(dot(N, L));
	float dotNV = saturate(dot(N, V));
	float dotNH = saturate(dot(N, H));
	float dotLH = saturate(dot(L, H));

	float F, D, vis;

	// D
	float alphaSqr = alpha*alpha;
	float pi = 3.14159f;
	float denom = dotNH * dotNH *(alphaSqr - 1.0) + 1.0f;
	D = alphaSqr / (pi * denom * denom);

	// F
	float dotLH5 = pow(1.0f - dotLH, 5);
	F = F0 + (1.0 - F0)*(dotLH5);

	// V
	float k = alpha / 2.0f;
	vis = G1V(dotNL, k)*G1V(dotNV, k);

	float specular = dotNL * D * F * vis;
	return specular;
}







struct VOut
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float3 Normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float3 Tangent : TANGENT;
	float3 Bitangent : BITANGENT;
};

Texture2D ObjTexture : register(t0);
Texture2D NormalTexture : register(t1);
SamplerState ObjSamplerState;

cbuffer cbPerObject : register(b0)
{
	float4x4 WVP;
	float4x4 World;
	float3 CameraPos;
	float padd;
};

cbuffer DirLight : register(b1)
{
	float3 Lightdir;
	float pad;
	float4 Lightambient;
	float4 Lightdiffuse;
};

VOut VSMain(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 inNormal : NORMAL, float3 inTangent : TANGENT, float3 inBitangent : BINORMAL)
{
	VOut output;

	output.Pos = mul(inPos, WVP);
	output.Normal = mul(inNormal, World);
	output.TexCoord = inTexCoord;
	//output.Tangent = mul(inTangent, World);
	//output.Bitangent = mul(inBitangent, World);
	//output.Bitangent = normalize(output.Bitangent);
	//output.Tangent = normalize(output.Tangent);
	output.Normal = normalize(output.Normal);

	float4 worldPosition;
	worldPosition = mul(inPos, World);
	
	float3 Test = CameraPos.xyz;
	output.viewDirection = CameraPos.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);

	return output;
}


float4 PSMain(VOut input) : SV_TARGET
{
	//For Specular
	float3 reflection;
	float4 specular;
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float3 BumpNormal;


	float4 NormalMapSample = NormalTexture.Sample(ObjSamplerState, input.TexCoord * 1);
	NormalMapSample = (2.0f * NormalMapSample) - 1.0f;

	BumpNormal = (NormalMapSample.x * input.Tangent) + (NormalMapSample.y * input.Bitangent) + (NormalMapSample.z * input.Normal);

	//input.Tangent = normalize(input.Tangent);
	//input.Tangent = normalize(input.Tangent - dot(input.Tangent, input.Normal)*input.Normal);
	//input.Bitangent = cross(input.Normal, input.Tangent);

	//float3x3 texSpace = float3x3(input.Tangent, input.Bitangent, input.Normal);

	//Convert normal from normal map to texture space and store in input.normal
	//input.Normal = normalize(mul(NormalMapSample, texSpace));


	//Reading Texture Color
	BumpNormal = normalize(BumpNormal);

	float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);
	diffuse = saturate(diffuse);

	input.Normal = BumpNormal;

	//Calculating Reflection Vector (1 - LightIntensity)
	reflection = normalize(2 * 3 * input.Normal + Lightdir);
	//Calc Specular
	specular = pow(saturate(dot(reflection, input.viewDirection)), 128);

	//Diffuse Color
	float3 finalColor;
	finalColor = diffuse * Lightambient;
	finalColor += saturate(dot(Lightdir, input.Normal) * Lightdiffuse * diffuse);

	float4 ffinalColor = float4(finalColor, diffuse.a);
	//return ffinalColor;

	float specularx;
	//Best 0.05, 0.4
	specularx = LightingFuncGGX_OPT3(input.Normal, input.viewDirection, Lightdir, 0.5, 0.2);
	ffinalColor.x = ffinalColor.x + specularx;
	ffinalColor.y = ffinalColor.y + specularx;
	ffinalColor.z = ffinalColor.z + specularx;

	ffinalColor.rgb = ((ffinalColor.rgb - 0.5f) * max(1.1, 0)) + 0.5f;
	ffinalColor.xyz *= 1.1;

	return ffinalColor;
	//return ObjTexture.Sample(ObjSamplerState, input.TexCoord);
}

