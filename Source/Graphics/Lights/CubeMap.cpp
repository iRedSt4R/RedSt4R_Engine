#include "CubeMap.h"

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		:Position(x, y, z), texCoord(u, v), Normal(nx, ny, nz) {}

	XMFLOAT3 Position;
	XMFLOAT2 texCoord;
	XMFLOAT3 Normal;
};

struct cbCubeMap
{
	XMMATRIX WVP;
	XMMATRIX World;
};

cbCubeMap cbCubeMapObject;

RedSt4R::Graphics::CubeMap::CubeMap(Dx11Engine *Engine, wchar_t* CubeMapTextureDir)
{
	m_Engine = Engine;
	m_DeviceContext = m_Engine->GetDeviceContext();
	//Create Sphere:
	CreateSphere(10,10);

	CubeMapShader = new Shader(Engine->GetDevice(), Engine->GetDeviceContext());
	CubeMapShader->RS_CreateVertexShader(L"Source/Graphics/Lights/CubeMapShader.fx");
	CubeMapShader->RS_CreatePixelShader(L"Source/Graphics/Lights/CubeMapShader.fx");
	CubeMapShader->RS_CreateInputLayout();

	D3D11_BUFFER_DESC cbcubemapDesc = {};
	cbcubemapDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbcubemapDesc.Usage = D3D11_USAGE_DEFAULT;
	cbcubemapDesc.ByteWidth = sizeof(cbCubeMap);
	cbcubemapDesc.CPUAccessFlags = 0;
	cbcubemapDesc.MiscFlags = 0;

	m_Engine->GetDevice()->CreateBuffer(&cbcubemapDesc, nullptr, &sphereConstantBuffer);

	CreateDDSTextureFromFile(m_Engine->GetDevice(), CubeMapTextureDir, &CubeMapResource, nullptr);

	//Changing From ID3D11Resource* To ID3D11Texture2D*
	auto hr = CubeMapResource->QueryInterface(IID_ID3D11Texture2D, (void **)&CubeMapTexture);
	if(FAILED(hr)) RS_WARNING("Cast Failed!")

	D3D11_TEXTURE2D_DESC SMTextureDesc;
	CubeMapTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = SMTextureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	srvDesc.TextureCube.MostDetailedMip = 0;

	m_Engine->GetDevice()->CreateShaderResourceView(CubeMapTexture, &srvDesc, &m_CubeMapShaderResourceView);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	Engine->GetDevice()->CreateSamplerState(&sampDesc, &m_SamplerState);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	m_Engine->GetDevice()->CreateDepthStencilState(&dssDesc, &DSLessEqual);
}

void RedSt4R::Graphics::CubeMap::CreateSphere(int LatLines, int LongLines)
{
		NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
		NumSphereFaces = ((LatLines - 3)*(LongLines) * 2) + (LongLines * 2);

		float sphereYaw = 0.0f;
		float spherePitch = 0.0f;

		std::vector<Vertex> vertices(NumSphereVertices);

		XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		vertices[0].Position.x = 0.0f;
		vertices[0].Position.y = 0.0f;
		vertices[0].Position.z = 1.0f;

		for (DWORD i = 0; i < LatLines - 2; ++i)
		{
			spherePitch = (i + 1) * (3.14 / (LatLines - 1));
			Rotationx = XMMatrixRotationX(spherePitch);
			for (DWORD j = 0; j < LongLines; ++j)
			{
				sphereYaw = j * (6.28 / (LongLines));
				Rotationy = XMMatrixRotationZ(sphereYaw);
				currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
				currVertPos = XMVector3Normalize(currVertPos);
				vertices[i*LongLines + j + 1].Position.x = XMVectorGetX(currVertPos);
				vertices[i*LongLines + j + 1].Position.y = XMVectorGetY(currVertPos);
				vertices[i*LongLines + j + 1].Position.z = XMVectorGetZ(currVertPos);
			}
		}

		vertices[NumSphereVertices - 1].Position.x = 0.0f;
		vertices[NumSphereVertices - 1].Position.y = 0.0f;
		vertices[NumSphereVertices - 1].Position.z = -1.0f;


		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

		vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumSphereVertices;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = 0;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;

		ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
		vertexBufferData.pSysMem = &vertices[0];
		m_Engine->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);


		std::vector<DWORD> indices(NumSphereFaces * 3);

		int k = 0;
		for (DWORD l = 0; l < LongLines - 1; ++l)
		{
			indices[k] = 0;
			indices[k + 1] = l + 1;
			indices[k + 2] = l + 2;
			k += 3;
		}

		indices[k] = 0;
		indices[k + 1] = LongLines;
		indices[k + 2] = 1;
		k += 3;

		for (DWORD i = 0; i < LatLines - 3; ++i)
		{
			for (DWORD j = 0; j < LongLines - 1; ++j)
			{
				indices[k] = i*LongLines + j + 1;
				indices[k + 1] = i*LongLines + j + 2;
				indices[k + 2] = (i + 1)*LongLines + j + 1;

				indices[k + 3] = (i + 1)*LongLines + j + 1;
				indices[k + 4] = i*LongLines + j + 2;
				indices[k + 5] = (i + 1)*LongLines + j + 2;

				k += 6; // next quad
			}

			indices[k] = (i*LongLines) + LongLines;
			indices[k + 1] = (i*LongLines) + 1;
			indices[k + 2] = ((i + 1)*LongLines) + LongLines;

			indices[k + 3] = ((i + 1)*LongLines) + LongLines;
			indices[k + 4] = (i*LongLines) + 1;
			indices[k + 5] = ((i + 1)*LongLines) + 1;

			k += 6;
		}

		for (DWORD l = 0; l < LongLines - 1; ++l)
		{
			indices[k] = NumSphereVertices - 1;
			indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
			indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
			k += 3;
		}

		indices[k] = NumSphereVertices - 1;
		indices[k + 1] = (NumSphereVertices - 1) - LongLines;
		indices[k + 2] = NumSphereVertices - 2;

		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA iinitData;

		iinitData.pSysMem = &indices[0];
		m_Engine->GetDevice()->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);

}

void RedSt4R::Graphics::CubeMap::UpdateCubeMap()
{
	
	sphereWorld = XMMatrixIdentity();

	//Define sphereWorld's world space matrix
	Scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	//Make sure the sphere is always centered around camera
	Translation = XMMatrixTranslation(XMVectorGetX(m_Engine->FPSCamera->GetCameraPosition()), XMVectorGetY(m_Engine->FPSCamera->GetCameraPosition()), XMVectorGetZ(m_Engine->FPSCamera->GetCameraPosition()));
	XMVECTOR rotationDir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX mRotation = XMMatrixRotationAxis(rotationDir, 0);

	sphereWorld = Scale * Translation;

	m_DeviceContext->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);

	WVP = sphereWorld * m_Engine->FPSCamera->GetCameraView() * m_Engine->FPSCamera->GetCameraProjections();
	cbCubeMapObject.WVP = XMMatrixTranspose(WVP);
	cbCubeMapObject.World = XMMatrixTranspose(sphereWorld);
	m_DeviceContext->UpdateSubresource(sphereConstantBuffer, 0, NULL, &cbCubeMapObject, 0, 0);
	m_DeviceContext->VSSetConstantBuffers(2, 1, &sphereConstantBuffer);
	m_DeviceContext->PSSetShaderResources(3, 1, &m_CubeMapShaderResourceView);
	m_DeviceContext->PSSetSamplers(0, 1, &m_SamplerState);
	m_DeviceContext->IASetInputLayout(CubeMapShader->GetInputLayout());
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->VSSetShader(CubeMapShader->GetVertexShader(), 0, 0);
	m_DeviceContext->PSSetShader(CubeMapShader->GetPixelShader(), 0, 0);
	m_DeviceContext->OMSetDepthStencilState(DSLessEqual, 0);
	m_DeviceContext->DrawIndexed(NumSphereFaces * 3, 0, 0);
	m_DeviceContext->OMSetDepthStencilState(NULL, 0);
	
	
}

void RedSt4R::Graphics::CubeMap::InitCubeMap()
{

}

void RedSt4R::Graphics::CubeMap::DrawCubeMap()
{

}

void RedSt4R::Graphics::CubeMap::BindCubeMap(int index)
{
	m_DeviceContext->PSSetShaderResources(3, 1, &m_CubeMapShaderResourceView);
}

RedSt4R::Graphics::CubeMap::~CubeMap()
{

}
