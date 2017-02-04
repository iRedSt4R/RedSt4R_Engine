#include "Texture.h"
#include "../Graphics/Material/WICTextureLoader.h"

RedSt4R::API::Texture::Texture()
{

/*
	//------------------- Creating Sampler State -----------------------//
	D3D11_SAMPLER_DESC afDesc;
	ZeroMemory(&afDesc, sizeof(D3D11_SAMPLER_DESC));

	afDesc.Filter = D3D11_FILTER_MAXIMUM_ANISOTROPIC;
	afDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	afDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	afDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	afDesc.MipLODBias = 0;
	afDesc.MaxAnisotropy = 16;
	afDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	afDesc.MinLOD = 0;
	afDesc.MaxLOD = 0;

	engineResources->GetDevice()->CreateSamplerState(&afDesc, &filteringState);
*/

}

RedSt4R::API::Texture::~Texture()
{

}

void RedSt4R::API::Texture::LoadTexture(EngineResources* pEngineResources, wchar_t* filePath, int a_textureType)
{
	engineResources = pEngineResources;
	textureType = a_textureType;

	CreateWICTextureFromFile(engineResources->GetDevice(), filePath, NULL, &textureSRV);

	//------------------------ Create Buffer For Texture (TODO) --------------------------//

}

void RedSt4R::API::Texture::Render()
{
	engineResources->GetDeviceContext()->PSSetShaderResources(textureType, 1, &textureSRV);
}
