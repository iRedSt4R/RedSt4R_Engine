#include "Shader.h"
#define D3D_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)

RedSt4R::Graphics::Shader::Shader(ID3D11Device *a_Device, ID3D11DeviceContext *a_DeviceContext)
{
	m_Device = a_Device;
	m_DeviceContext = a_DeviceContext;
}

void RedSt4R::Graphics::Shader::RS_CreateVertexShader(const wchar_t* a_VertexShaderDir)
{
	hr = D3DCompileFromFile((LPCWSTR)a_VertexShaderDir, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0", 0, 0, &m_VertexShaderByteCode, 0);
	if(FAILED(hr)) RS_ERROR("Failed Compiling Vertex Shader!")

	hr = m_Device->CreateVertexShader(m_VertexShaderByteCode->GetBufferPointer(), m_VertexShaderByteCode->GetBufferSize(), nullptr, &m_VertexShader);
	if (FAILED(hr)) RS_ERROR("Failed Creating Vertex Shader!")
	
}

void RedSt4R::Graphics::Shader::RS_CreatePixelShader(const wchar_t* a_PixelShaderDir)
{
	hr = D3DCompileFromFile((LPCWSTR)a_PixelShaderDir, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_5_0", 0, 0, &m_PixelShaderByteCode, 0);
	if (FAILED(hr)) RS_ERROR("Failed Compiling Pixel Shader!")

	hr = m_Device->CreatePixelShader(m_PixelShaderByteCode->GetBufferPointer(), m_PixelShaderByteCode->GetBufferSize(), nullptr, &m_PixelShader);
	if (FAILED(hr)) RS_ERROR("Failed Creating Pixel Shader!")
}

void RedSt4R::Graphics::Shader::RS_CreateInputLayout()
{
	
	D3D11_INPUT_ELEMENT_DESC m_InputLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = m_Device->CreateInputLayout(m_InputLayoutDesc, ARRAYSIZE(m_InputLayoutDesc), m_VertexShaderByteCode->GetBufferPointer(), m_VertexShaderByteCode->GetBufferSize(), &m_InputLayout);
	if (FAILED(hr)) RS_ERROR("Failed Creating Input Layout!")
}

void RedSt4R::Graphics::Shader::RS_CreateInputLayoutB()
{
	D3D11_INPUT_ELEMENT_DESC m_InputLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = m_Device->CreateInputLayout(m_InputLayoutDesc, ARRAYSIZE(m_InputLayoutDesc), m_VertexShaderByteCode->GetBufferPointer(), m_VertexShaderByteCode->GetBufferSize(), &m_InputLayoutB);
	if (FAILED(hr)) RS_ERROR("Failed Creating Input Layout!")
}

void RedSt4R::Graphics::Shader::RS_CreateInputLayoutC()
{
	D3D11_INPUT_ELEMENT_DESC m_InputLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = m_Device->CreateInputLayout(m_InputLayoutDesc, ARRAYSIZE(m_InputLayoutDesc), m_VertexShaderByteCode->GetBufferPointer(), m_VertexShaderByteCode->GetBufferSize(), &m_InputLayoutC);
	if (FAILED(hr)) RS_ERROR("Failed Creating Input Layout!")
}

RedSt4R::Graphics::Shader::~Shader()
{

}
