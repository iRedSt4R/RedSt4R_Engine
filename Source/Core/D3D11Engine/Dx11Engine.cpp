#include "Dx11Engine.h"

#define WINDOW_WIDTH 1366
#define WINDOW_HEIGHT 768 
#define MSAA_QUALITY 1


HRESULT hr;


RedSt4R::Dx11Engine::Dx11Engine(HWND a_hwnd)
{
	hwnd = a_hwnd;
}

void RedSt4R::Dx11Engine::InitEngine()
{
	//-------------------------------Creating(Swap Chain|Device|Device Context)-----------------------------//
	DXGI_MODE_DESC modeDesc;
	ZeroMemory(&modeDesc, sizeof(DXGI_MODE_DESC));

	modeDesc.Height = WINDOW_HEIGHT;
	modeDesc.Width = WINDOW_WIDTH;
	modeDesc.RefreshRate.Numerator = 0;
	modeDesc.RefreshRate.Denominator = 0;
	modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapchainDesc;
	ZeroMemory(&swapchainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapchainDesc.BufferDesc = modeDesc;
	swapchainDesc.BufferCount = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	swapchainDesc.SampleDesc.Count = MSAA_QUALITY;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.OutputWindow = GetActiveWindow();
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapchainDesc.Windowed = TRUE;


	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapchainDesc, &m_SwapChain, &m_Device, NULL, &m_DeviceContext);
	if (FAILED(hr)) RS_ERROR("Failed Creating D3D11 Device and Swap Chain and Device Context!");

	//m_SwapChain->SetFullscreenState(TRUE, NULL);

	//--------------------------------Creating Render Target View----------------------------//
	ID3D11Texture2D *m_BackBufferTexture;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_BackBufferTexture);
	if (FAILED(hr)) RS_ERROR("Failed Getting Back Buffer From Swap Chain!")

	hr = m_Device->CreateRenderTargetView(m_BackBufferTexture, nullptr, &m_RenderTargetView);
	if (FAILED(hr)) RS_ERROR("Failed Creating Render Target View!")
		m_BackBufferTexture->Release();

	//--------------------------------Creating Viewport---------------------------------//
	
	ZeroMemory(&m_Viewport, sizeof(D3D11_VIEWPORT));
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = WINDOW_WIDTH;
	m_Viewport.Height = WINDOW_HEIGHT;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	//-----------------------------------Creating Shaders------------------------------------//
	

	m_MainShader = new RedSt4R::Graphics::Shader(m_Device, m_DeviceContext);
	m_MainShader->RS_CreateVertexShader(L"Shader.fx");
	m_MainShader->RS_CreatePixelShader(L"Shader.fx");
	m_MainShader->RS_CreateInputLayoutB();
	
	//-------------Creating Depth Stencil View-------------------
	D3D11_TEXTURE2D_DESC depthtextureDesc = {};
	depthtextureDesc.Height = WINDOW_HEIGHT;
	depthtextureDesc.Width = WINDOW_WIDTH;
	depthtextureDesc.MipLevels = 1;
	depthtextureDesc.ArraySize = 1;
	depthtextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthtextureDesc.SampleDesc.Count = MSAA_QUALITY;
	depthtextureDesc.SampleDesc.Quality = 0;
	depthtextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthtextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthtextureDesc.CPUAccessFlags = 0;
	depthtextureDesc.MiscFlags = 0;

	m_Device->CreateTexture2D(&depthtextureDesc, nullptr, &m_DepthStencilTexture);
	m_Device->CreateDepthStencilView(m_DepthStencilTexture, nullptr, &m_DepthStencilView);
	
	FPSCamera = new RedSt4R::Graphics::FirstPersonCamera();
	FPSCamera->UpdateFPSCamera();
	

	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;

	m_Device->CreateRasterizerState(&rsDesc, &m_DefaultRasterizationState);
	m_DeviceContext->RSSetState(m_DefaultRasterizationState);

	//------------- Creating Texture2D For Post Process ------------------//
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	///////////////////////// Map's Texture
	// Initialize the  texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the texture description.
	// We will have our map be a square
	// We will need to have this texture bound as a render target AND a shader resource
	textureDesc.Width = WINDOW_WIDTH;
	textureDesc.Height = WINDOW_HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the texture
	m_Device->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureMap);
	

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	m_Device->CreateRenderTargetView(renderTargetTextureMap, &renderTargetViewDesc, &renderTargetViewMap);
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	m_Device->CreateShaderResourceView(renderTargetTextureMap, &shaderResourceViewDesc, &shaderResourceViewMap);
}

void RedSt4R::Dx11Engine::PreparePipeline()
{
	m_DeviceContext->RSSetViewports(1, &m_Viewport);
	m_DeviceContext->VSSetShader(m_MainShader->GetVertexShader(), 0, 0);
	m_DeviceContext->PSSetShader(m_MainShader->GetPixelShader(), 0, 0);
	m_DeviceContext->IASetInputLayout(m_MainShader->GetInputLayoutB());
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);


}

void RedSt4R::Dx11Engine::UpdatePipeline()
{
	m_DeviceContext->VSSetShader(m_MainShader->GetVertexShader(), 0, 0);
	m_DeviceContext->PSSetShader(m_MainShader->GetPixelShader(), 0, 0);
	//m_DeviceContext->IASetInputLayout(m_MainShader->GetInputLayout());
	const UINT8 *state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_F1])
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
	if (state[SDL_SCANCODE_F2])
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

void RedSt4R::Dx11Engine::ClearScreen()
{
	float clearColor[4];
	clearColor[0] = 1.0f;
	clearColor[1] = 0.6f;
	clearColor[2] = 0.6f;
	clearColor[3] = 1.0f;


	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clearColor);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	FPSCamera->UpdateInput();
	FPSCamera->UpdateFPSCamera();
}

void RedSt4R::Dx11Engine::Render()
{
	hr = m_SwapChain->Present(1, 0);
	if(FAILED(hr)) RS_ERROR("Failed Presenting the scene!")
}

void RedSt4R::Dx11Engine::ShutDown()
{
	SAFE_RELEASE(m_DepthStencilTexture);
	SAFE_RELEASE(m_DepthStencilView);
	SAFE_RELEASE(m_VertexBuffer);
	SAFE_RELEASE(m_DepthStencilTexture);
	SAFE_RELEASE(m_RenderTargetView);
	SAFE_RELEASE(m_SwapChain);
	SAFE_RELEASE(m_DeviceContext);
	SAFE_RELEASE(m_Device);
}

void RedSt4R::Dx11Engine::SetFPSMouse()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void RedSt4R::Dx11Engine::SetEditorMouse()
{
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

//void RedSt4R::Dx11Engine::SetCubeMap(CubeMap *pCubeMap)
//{
	//m_CubeMap = pCubeMap;
//}

void RedSt4R::Dx11Engine::CreateStaticCamera()
{
	//camPosition = XMVectorSet(0.0f, 0.0f, -30.0f, 0.0f);
	//camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//mCameraView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	//mCameraProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, 800.0f / 600.0f, 1.0f, 1000.0f);
}

RedSt4R::Dx11Engine::~Dx11Engine()
{

}
