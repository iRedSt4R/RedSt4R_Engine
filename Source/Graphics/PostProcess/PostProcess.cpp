#include "PostProcess.h"

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768 
#define MSAA_QUALITY 1

// Calculate the screen coordinates of the left side of the window.
float left = -1;

// Calculate the screen coordinates of the right side of the window.
float right = 1;

// Calculate the screen coordinates of the top of the window.
float top = 1;

// Calculate the screen coordinates of the bottom of the window.
float bottom = -1;

VertexPP v[] =
{
	VertexPP(left, top, 0.5f, 0, 0),
	VertexPP(left,  bottom, 0.5f, 0, 1),
	VertexPP(right, bottom, 0.5f, 1,  1),
	VertexPP(right, top, 0.5f, 1, 0),
	VertexPP(right, bottom, 0.5f, 1, 1),
	VertexPP(left, top, 0.5f, 0, 0),
};

RedSt4R::PostProcess::PostProcess()
{

}

RedSt4R::PostProcess::~PostProcess()
{

}

void RedSt4R::PostProcess::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	m_Device = pDevice;
	m_DeviceContext = pDeviceContext;

	PostProcessBaseShader = new Graphics::Shader(pDevice, pDeviceContext);
	PostProcessBaseShader->RS_CreateVertexShader(L"PostProcess.fx");
	PostProcessBaseShader->RS_CreatePixelShader(L"PostProcess.fx");
	PostProcessBaseShader->RS_CreateInputLayoutC();

	//------------------- Vertex Buffer For Quad -------------------//
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexPP) * 6;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA VBData;
	ZeroMemory(&VBData, sizeof(D3D11_SUBRESOURCE_DATA));

	VBData.pSysMem = v;
	auto hrrr = m_Device->CreateBuffer(&vertexBufferDesc, &VBData, &vertexBuffer);
	if (FAILED(hrrr)) RS_WARNING("Failed Creating VERTEX BUFFER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");


	

}

void RedSt4R::PostProcess::Render(ID3D11Texture2D *pRenderedTexture, ID3D11RenderTargetView* pTextureRTV, ID3D11ShaderResourceView* pTextureSRV)
{
	renderedTexture = pRenderedTexture;
	textureRTV = pTextureRTV;
	textureSRV = pTextureSRV;

	m_DeviceContext->PSSetShaderResources(0, 1, &pTextureSRV);
	m_DeviceContext->IASetInputLayout(PostProcessBaseShader->GetInputLayoutC());
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT stride = sizeof(VertexPP);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	m_DeviceContext->VSSetShader(PostProcessBaseShader->GetVertexShader(), 0, 0);
	m_DeviceContext->PSSetShader(PostProcessBaseShader->GetPixelShader(), 0, 0);
	m_DeviceContext->Draw(6, 0);


}
