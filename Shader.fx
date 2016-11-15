#define GAMMA 2.2f
#define PI 3.14159f
/*
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
*/

//-------------------------------------------------- FROSTBITE PBR [BRDF] ---------------------------------------------//
struct VOut
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float3 Normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float3 Tangent : TANGENT;
};

//---------------------- TEXTURES -----------------------//
Texture2D ObjTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D RoughnessTexture : register(t4);
Texture2D MetallicTexture : register(t5);
TextureCube CubeMapTexture : register(t3);
SamplerState ObjSamplerState : register(s0);
SamplerState g_samPoint
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

//--------------------- CONSTANT BUFFERS ----------------//
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

cbuffer Material : register(b2)
{
	float bHaveDiffuseTexture;
	float bHaveNormalMap;
	float bHaveGlossMap;
	float bHaveRougnessMap;
	float bHaveMetallMap;
	float mRoughness;
	float mGlossiness;
	float p0;
};

//-------------------- HELPER FUNCTIONS -----------------------//

/*
float3 FinalGamma(float3 color)
{
	return pow(color, 1.0f / GAMMA);
}

float4 GammaCorrectTexture(Texture2D t, SamplerState s, float2 uv)
{
	float4 samp = t.Sample(s, uv);
	return float4(pow(samp.rgb, GAMMA), samp.a);
}

float3 GammaCorrectTextureRGB(Texture2D t, SamplerState s, float2 uv)
{
	float4 samp = t.Sample(s, uv);
	return float3(pow(samp.rgb, GAMMA));
}
*/

//------------------- GETTERS FUNCTIONS --------------------//
float4 GetAlbedo(float2 a_TexCoord)
{
	if (bHaveDiffuseTexture == 1.0)
	{
		return ObjTexture.Sample(ObjSamplerState, a_TexCoord);
		//return ObjTexture.SampleLevel(g_samPoint, a_TexCoord, 0);
	}
	else
	{
		return float4(0.662745, 0.662745, 0.662745, 1.0);
		//return float4(1, 0.843137, 0, 1.0);
	}
}
float GetRoughness(float2 a_TexCoord)
{
	//if (bHaveRougnessMap == 1.0)
	//{
		return RoughnessTexture.Sample(ObjSamplerState, a_TexCoord);
	//}
	//else
	//{
	//	return 1;
	//}
}
float GetMetallness(float2 a_TexCoord)
{
	//if (bHaveRougnessMap == 1.0)
	//{
	return MetallicTexture.Sample(ObjSamplerState, a_TexCoord);
	//}
	//else
	//{
	//	return 1;
	//}
}
/*
float3 GetSpecular(Attributes attributes)
{
	return (1.0f - u_UsingSpecularMap) * u_SpecularColor + u_UsingSpecularMap * GammaCorrectTextureRGB(u_SpecularMap, u_SpecularSampler, attributes.uv);
}

float GetGloss(Attributes attributes)
{
	u_GlossColor + u_UsingGlossMap * GammaCorrectTextureRGB(u_GlossMap, u_GlossSampler, attributes.uv).r;
}


*/
//------------------------ BRDF STUFF ----------------------//
float FresnelSchlick(float f0, float fd90, float view)
{
	return f0 + (fd90 - f0) * pow(max(1.0f - view, 0.1f), 5.0f);
}

float chiGGX(float v)
{
	return v > 0 ? 1 : 0;
}

float GGX_Distribution(float3 n, float3 h, float alpha)
{
	float NoH = dot(n, h);
	float alpha2 = alpha * alpha;
	float NoH2 = NoH * NoH;
	float den = NoH2 * alpha2 + (1 - NoH2);
	return (chiGGX(NoH) * alpha2) / (PI * den * den);
}
 
float Disney(float3 N, float3 V, float3 L, float roughness)
{
	float3 halfVector = normalize(L + V);

	float NdotL = saturate(dot(N, L));
	float LdotH = saturate(dot(L, halfVector));
	float NdotV = saturate(dot(N, V));

	float energyBias = lerp(0.0f, 0.5f, roughness);
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughness);
	float fd90 = energyBias + 2.0f * (LdotH * LdotH) * roughness;
	float f0 = 1.0f;

	float lightScatter = FresnelSchlick(f0, fd90, NdotL);
	float viewScatter = FresnelSchlick(f0, fd90, NdotV);

	return lightScatter * viewScatter * energyFactor;
}

float3 GGX(float3 N, float3 V, float3 L, float roughness, float3 specular)
{
	float3 h = normalize(L + V);
	float NdotH = saturate(dot(N, h));

	float rough2 = max(roughness * roughness, 2.0e-3f); // capped so spec highlights don't disappear
	float rough4 = rough2 * rough2;

	float d = (NdotH * rough4 - NdotH) * NdotH + 1.0f;
	float D = rough4 / (3.1415926535897932384626433832795f * (d * d)); //PI TO BE ADDED!!!!

	// Fresnel
	float3 reflectivity = specular;
	float fresnel = 1;
	float NdotL = saturate(dot(N, L));
	float LdotH = saturate(dot(L, h));
	float NdotV = saturate(dot(N, V));
	float3 F = reflectivity + (fresnel - fresnel * reflectivity) * exp2((-5.55473f * LdotH - 6.98316f) * LdotH);

	// geometric / visibility
	float k = rough2 * 0.5f;
	float G_SmithL = NdotL * (1.0f - k) + k;
	float G_SmithV = NdotV * (1.0f - k) + k;
	float G = 0.25f / (G_SmithL * G_SmithV);

	return G * D * F;
}


float3 BRDF(float3 L, float3 V, float3 N, float3 cAlbedo, float pMetallic, float pRoughness)
{
	float3 base_color = cAlbedo;
	base_color = pow(base_color, 1 / 2.5f);
	float metallic = pMetallic;
	//metallic = pow(metallic, 1 / 1.3f);
	float roughness = pRoughness;
	//roughness = pow(roughness, 1 / 1.3f);

	float3  H = normalize(L + V);
	float dot_n_l = dot(N, L);
	float dot_l_h = dot(L, H);
	float dot_n_h = dot(N, H);
	float dot_n_v = dot(N, V);

	float alpha = roughness * roughness;

	float3 diffuse_color = base_color * (1 - metallic);
	float3 diffuse_brdf = diffuse_color;
	diffuse_brdf *= dot_n_l * Disney(N, V, L, alpha);

	//Reflections
	float3 reflectionVector = normalize(reflect(-V, N));
	float smoothness = 1 - roughness;
	float mipLevel = (1.0f - smoothness * smoothness) * 10.0f;
	float4 cs = CubeMapTexture.SampleLevel(g_samPoint, reflectionVector, mipLevel);
	cs *= dot_n_l;


	diffuse_brdf = saturate(lerp(diffuse_brdf, cs.rgb , metallic));
	diffuse_brdf *= base_color.rgb;
	
	float specPow = pow((1 - roughness), 4);
	float3 specular_brdf = GGX(N, V, L, roughness, float3(specPow, specPow, specPow));
	
	float3 final_brdf = (diffuse_brdf + specular_brdf);

	return final_brdf;
	}



//----------------------------------------------------- VERTEX SHADER-------------------------------------------//

VOut VSMain(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 inNormal : NORMAL, float3 inTangent : TANGENT)
{
	VOut output;

	output.Pos = mul(inPos, WVP);
	output.Normal = mul(inNormal, World);
	output.TexCoord = inTexCoord;
	output.Tangent = mul(inTangent, (float3x3)World);

	float4 worldPosition;
	worldPosition = mul(inPos, World);
	
	float3 Test = CameraPos.xyz;
	output.viewDirection = CameraPos.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);

	return output;
}



//----------------------------------------------------- PIXEL SHADER-------------------------------------------//
float4 PSMain(VOut input) : SV_TARGET
{
	float3 Normal = normalize(input.Normal);

	float3 Tangent = normalize(input.Tangent);
	Tangent = normalize(Tangent - dot(Tangent, input.TexCoord) * Normal);
	float3 Binormal = cross(Tangent, Normal);
	float3 BumpMapNormal = NormalTexture.Sample(ObjSamplerState, input.TexCoord);
	BumpMapNormal = (2.0f * BumpMapNormal) - 1.0f;
	float3 NewNormal;
	float3x3 TBN = float3x3(Tangent, Binormal, Normal);
	NewNormal = mul(BumpMapNormal, TBN);
	NewNormal = normalize(NewNormal);


	///if (bHaveNormalMap == 1.0)
	//{
		input.Normal = NewNormal;
	//}
	//else
	//{
		//input.Normal = normalize(input.Normal);
	//}

	float4 diffuse = GetAlbedo(input.TexCoord);
	
	//float3 finalColor = BRDF(Lightdir, input.viewDirection, input.Normal, diffuse.rgb, GetMetallness(input.TexCoord), GetRoughness(input.TexCoord));
	float3 finalColor = BRDF(Lightdir, input.viewDirection, input.Normal, diffuse.rgb, mRoughness, 1 - GetRoughness(input.TexCoord));
	finalColor.rgb = ((finalColor.rgb - 0.5f) * max(1.2f, 0)) + 0.5f;
	//finalColor.rgb *= 1.2f;

	//------------- GREY SCALE ------------//
	//float grayscale = dot(finalColor.rgb, float3(0.3, 0.59, 0.11));
	//finalColor.rgb = grayscale;


	return float4(finalColor, 1.0f);
}

