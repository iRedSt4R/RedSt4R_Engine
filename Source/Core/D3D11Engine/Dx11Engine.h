#pragma once

#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <SDL.h>
#include "../stdafx.h"
#include <DirectXMath.h>
#include <windows.h>
#include "../../Debug/DebugMacros.h"
#include "../../Graphics/Shaders/Shader.h"
#include "../../Graphics/Camera/FirstPersonCamera.h"
#include "../EngineResources.h"
#include "../../Graphics/PostProcess/PostProcess.h"

using namespace DirectX;
using namespace RedSt4R;
using namespace Graphics;


namespace RedSt4R 
{
	class Dx11Engine
	{
	public:

		FirstPersonCamera *FPSCamera;
	private:
		bool bIsRunning = false;
		//---------WinApi Stuff---------//
		HWND hwnd;

		//-------- DirectX 11 Stuff-----------//
		ID3D11Device *m_Device;
		ID3D11DeviceContext *m_DeviceContext;
		IDXGISwapChain *m_SwapChain;
		ID3D11RenderTargetView *m_RenderTargetView;
		RedSt4R::Graphics::Shader *m_MainShader;
		ID3D11Buffer *m_VertexBuffer;
		D3D11_VIEWPORT m_Viewport;
		//-------Depth|Stencil View---------------//
		ID3D11Texture2D *m_DepthStencilTexture;
		ID3D11DepthStencilView *m_DepthStencilView;
		ID3D11RasterizerState *m_DefaultRasterizationState;
		//--------Post_Process Texture-------------//
		ID3D11Texture2D* postProcessTexture;
		ID3D11RenderTargetView* renderTargetViewMap;
		ID3D11ShaderResourceView* shaderResourceViewMap;
		PostProcess* pp;
		//-----------Static Camera [Temporal]-------------//
		XMMATRIX mCameraView;
		XMMATRIX mCameraProjection;
		ID3D11Buffer *m_CameraConstantBuffer;
		XMMATRIX World;
		XMVECTOR camPosition;
		XMVECTOR camTarget;
		XMVECTOR camUp;

	public:
		Dx11Engine(HWND a_hwnd);
		~Dx11Engine();

		void InitEngine();
		void PreparePipeline();
		void UpdatePipeline();
		void ClearScreen();
		void Render();
		void ShutDown();

		void SetFPSMouse();
		void SetEditorMouse();


		inline ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext; }
		inline ID3D11Device* GetDevice() const { return m_Device; }
		inline XMMATRIX GetCameraView() const { return FPSCamera->GetCameraView(); }
		inline XMMATRIX GetCameraProjection() const { return FPSCamera->GetCameraProjections(); }
		inline ID3D11InputLayout* GetInputLayout() const { return m_MainShader->GetInputLayout(); }
		inline ID3D11InputLayout* GetInputLayoutB() const { return m_MainShader->GetInputLayoutB(); }
		inline ID3D11RasterizerState* GetBackCullingRasterizationState() { return m_DefaultRasterizationState; }
		inline ID3D11Texture2D* GetTexture2DForPostProcess() { return postProcessTexture; }
		inline ID3D11RenderTargetView* GetRTVForPostProcess() { return renderTargetViewMap; }
		inline ID3D11ShaderResourceView* GetSRV2DForPostProcess() { return shaderResourceViewMap; }
		inline EngineResources* GetEngineResource() 
		{ 
			EngineResources* eg = new EngineResources;
			eg->SetDevice(m_Device);
			eg->SetDeviceContext(m_DeviceContext);

			return eg;
		}

	private:
		void CreateStaticCamera();
		
	};
}