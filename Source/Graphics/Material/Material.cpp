#include "Material.h"

RedSt4R::Graphics::Material::Material(Dx11Engine *Engine)
{
	m_Engine = Engine;
	m_Device = Engine->GetDevice();
	m_DeviceContext = Engine->GetDeviceContext();

	bHaveDiffuseTexture = false;
	bHaveNormalTexture = false;
	bHaveRougnessTexture = false;
	bHaveMetallicMap = false;

	cbMaterialObject.bHaveDiffuseTexture = 0.0f;
	cbMaterialObject.bHaveNormalMap = 0.0f;
	cbMaterialObject.bHaveRougnessMap = 0.0f;
	cbMaterialObject.bHaveMetallicMap = 0.0f;

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

	m_Device->CreateSamplerState(&afDesc, &FilteringState);

	CreateBuffers();
}

void RedSt4R::Graphics::Material::CreateMaterial(RS_Material_Desc &a_materialDesc)
{	
	bool r;

	DiffuseTexture = new RedSt4R::API::Texture();
	r = DiffuseTexture->LoadTexture(m_Engine->GetEngineResource(), a_materialDesc.diffuseTextureDir, TEXTURE_TYPE_DIFFUSE);
	if(r) bHaveDiffuseTexture = true;

	NormalTexture = new RedSt4R::API::Texture();
	r = NormalTexture->LoadTexture(m_Engine->GetEngineResource(), a_materialDesc.normalTextureDir, TEXTURE_TYPE_NORMAL);
	if (r) bHaveNormalTexture = true;

	RougnessTexture = new RedSt4R::API::Texture();
	r = RougnessTexture->LoadTexture(m_Engine->GetEngineResource(), a_materialDesc.rougnessTextureDir, TEXTURE_TYPE_ROUGHNESS);
	if (r) bHaveRougnessTexture = true;

	MetallicTexture = new RedSt4R::API::Texture();
	r = MetallicTexture->LoadTexture(m_Engine->GetEngineResource(), a_materialDesc.metallicTextureDir, TEXTURE_TYPE_METALLIC);
	if (r) bHaveMetallicMap	 = true;

	if (!bHaveDiffuseTexture) cbMaterialObject.mDiffuseColor = a_materialDesc.diffuseColor;

	bIsCreated = true;
}

void RedSt4R::Graphics::Material::PrepareMaterial()
{
	if (bHaveDiffuseTexture) cbMaterialObject.bHaveDiffuseTexture = 1.0;
	if (bHaveNormalTexture) cbMaterialObject.bHaveNormalMap = 1.0;
	if (bHaveRougnessTexture) cbMaterialObject.bHaveRougnessMap = 1.0;
	if (bHaveMetallicMap) cbMaterialObject.bHaveMetallicMap = 1.0;

	cbMaterialObject.mRougness = 1;
	cbMaterialObject.mMetalness = 0;
	
	DiffuseTexture->Render();
	NormalTexture->Render();
	RougnessTexture->Render();
	MetallicTexture->Render();
	m_DeviceContext->UpdateSubresource(m_MaterialConstantBuffer, 0, 0, &cbMaterialObject, 0, 0);
	m_DeviceContext->PSSetConstantBuffers(2, 1, &m_MaterialConstantBuffer);
}

void RedSt4R::Graphics::Material::ClearMaterial()
{
}

void RedSt4R::Graphics::Material::SetAnistopicFiltering()
{
	m_DeviceContext->PSSetSamplers(0, 1, &FilteringState);
}

void RedSt4R::Graphics::Material::SetRoughness(float value)
{
	m_Roughness = value;
}

void RedSt4R::Graphics::Material::SetGlossiness(float value)
{
	m_Glossiness = value;
}

void RedSt4R::Graphics::Material::CreateBuffers()
{
	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth = sizeof(cbMaterial);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	m_Device->CreateBuffer(&cbDesc, nullptr, &m_MaterialConstantBuffer);

}

RedSt4R::Graphics::Material::~Material()
{
	//delete DiffuseTexture;
}
