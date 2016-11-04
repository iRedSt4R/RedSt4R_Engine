#include "DireLight.h"

RedSt4R::Graphics::DireLight::DireLight(Dx11Engine *Engine, XMFLOAT3 a_vDirection, XMFLOAT4 a_Ambient, XMFLOAT4 a_vDiffuse)
{
	m_Engine = Engine;
	m_Device = m_Engine->GetDevice();
	m_DeviceContext = m_Engine->GetDeviceContext();

	vDirection = a_vDirection;
	vAmbient = a_Ambient;
	vDiffuse = a_vDiffuse;

	m_Light.ambient = vAmbient;
	m_Light.diffuse = vDiffuse;
	m_Light.dir = vDirection;

	CreateConstantBuffer();
}

void RedSt4R::Graphics::DireLight::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC cbDesc ;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth = sizeof(m_Light);
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.MiscFlags = 0;
	cbDesc.CPUAccessFlags = 0;

	m_Device->CreateBuffer(&cbDesc, 0, &m_LightConstantBuffer);
}

void RedSt4R::Graphics::DireLight::SetLight()
{
	m_DeviceContext->UpdateSubresource(m_LightConstantBuffer, 0, 0, &m_Light, 0, 0);
	m_DeviceContext->PSSetConstantBuffers(1, 1, &m_LightConstantBuffer);
}

RedSt4R::Graphics::DireLight::~DireLight()
{

}
