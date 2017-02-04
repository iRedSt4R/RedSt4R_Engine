#include "Material.h"

RedSt4R::Graphics::Material::Material(Dx11Engine *Engine)
{
	m_Engine = Engine;
	m_Device = Engine->GetDevice();
	m_DeviceContext = Engine->GetDeviceContext();

	cbMaterialObject.bHaveDiffuseTexture = 0;
	cbMaterialObject.bHaveNormalMap = 0;

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
	DiffuseTexture = new RedSt4R::API::Texture();
	DiffuseTexture->LoadTexture(m_Engine->GetEngineResource(), a_materialDesc.diffuseTextureDir, TEXTURE_TYPE_DIFFUSE);
	bHaveDiffuseTexture = true;

	NormalTexture = new RedSt4R::API::Texture();
	NormalTexture->LoadTexture(m_Engine->GetEngineResource(), a_materialDesc.normalTextureDir, TEXTURE_TYPE_NORMAL);
	bHaveNormalTexture = true;

	RougnessTexture = new RedSt4R::API::Texture();
	RougnessTexture->LoadTexture(m_Engine->GetEngineResource(), a_materialDesc.rougnessTextureDir, TEXTURE_TYPE_ROUGHNESS);
	bHaveRougnessTexture = true;

	MetallicTexture = new RedSt4R::API::Texture();
	MetallicTexture->LoadTexture(m_Engine->GetEngineResource(), a_materialDesc.metallicTextureDir, TEXTURE_TYPE_METALLIC);
	bHaveMetallicMap = true;

	/*
	HRESULT hr = CreateWICTextureFromFile(m_Device, a_materialDesc.diffuseTextureDir, NULL, &DiffuseTextureRV);
	if (FAILED(hr))
	{
		RS_ERROR("Failed Creating WicTexture - Diffuse!");
		bHaveDiffuseTexture = false;
	}
	else bHaveDiffuseTexture = true;
	if (a_materialDesc.normalTextureDir)
	{
		HRESULT hhr = CreateWICTextureFromFile(m_Device, a_materialDesc.normalTextureDir, NULL, &NormalTextureRV);
		if (FAILED(hhr))
		{
			RS_ERROR("Failed Creating WicTexture - Normal!");
			bHaveNormalTexture = false;
		}
		else bHaveNormalTexture = true;
	}

	if (a_materialDesc.rougnessTextureDir)
	{
		HRESULT hhr = CreateWICTextureFromFile(m_Device, a_materialDesc.rougnessTextureDir, NULL, &RougnessTextureRV);
		if (FAILED(hhr))
		{
			RS_ERROR("Failed Creating WicTexture - Rougness!");
			bHaveRougnessTexture = false;
		}
		else bHaveRougnessTexture = true;
	}
	if (a_materialDesc.metallicTextureDir)
	{
		HRESULT hhr = CreateWICTextureFromFile(m_Device, a_materialDesc.metallicTextureDir, NULL, &MetallicTextureRV);
		if (FAILED(hhr))
		{
			RS_ERROR("Failed Creating WicTexture - Metallic!");
			bHaveMetallicMap = false;
		}
		else bHaveMetallicMap = true;
	}
	*/

	bIsCreated = true;
}

void RedSt4R::Graphics::Material::PrepareMaterial()
{
	//if (bHaveDiffuseTexture) cbMaterialObject.bHaveDiffuseTexture = 1.0;
	//if (bHaveNormalTexture) cbMaterialObject.bHaveNormalMap = 1.0;
	//if (bHaveRougnessTexture) cbMaterialObject.bHaveRougnessMap = 1.0;
	//if (bHaveMetallicMap) cbMaterialObject.bHaveMetallicMap = 1.0;
	//cbMaterialObject.bHaveGlossMap = 0;

	//cbMaterialObject.mRougness = m_Roughness;
	//cbMaterialObject.mGlossiness = m_Glossiness;
	
	DiffuseTexture->Render();
	NormalTexture->Render();
	RougnessTexture->Render();
	MetallicTexture->Render();
	//m_DeviceContext->UpdateSubresource(m_MaterialConstantBuffer, 0, 0, &cbMaterialObject, 0, 0);
	//m_DeviceContext->PSSetConstantBuffers(2, 1, &m_MaterialConstantBuffer);
	//SetAnistopicFiltering();
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
	delete DiffuseTexture;
}
