#pragma once

#include "../../Core/stdafx.h"

using namespace DirectX;

namespace RedSt4R
{
	class Dx12Engine
	{
	private:
		//---------------------- ENGINE DEFINITIONS -----------------------------//
		static const int frameBufferCount = 2;

		HRESULT hr;
		HWND hwnd;
		IDXGIFactory4 *m_DxgiFactor;
		IDXGIAdapter1 *m_Adapter;

		ID3D12Device *m_Device;
		IDXGISwapChain3 *m_SwapChain3;
		ID3D12GraphicsCommandList *m_GraphicsCommandList;
		ID3D12CommandAllocator *m_CommandAllocator[frameBufferCount];
		ID3D12CommandQueue *m_CommandQueue;
		ID3D12Resource *m_RenderTargets[frameBufferCount];
		ID3D12DescriptorHeap *m_DescriptorHeap;
		ID3D12Fence *m_Fence[frameBufferCount];

		HANDLE fenceEvent;
		UINT64 fenceValue[frameBufferCount];

		int m_FrameIndex;
		UINT m_RtvDescriptorSize;
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle;

		//---------------------- RENDERING DEFINITIONS -----------------------------//
		
	
		ID3DBlob *m_VertexShader;
		ID3DBlob *m_PixelShader;
		ID3DBlob *m_ErrorShaderBlob;

		ID3D12PipelineState *m_MainPipelineState;
		ID3D12RootSignature *m_RootSignature;
		D3D12_VIEWPORT m_Viewport;
		D3D12_RECT m_ScissorRect;
		ID3D12Resource *m_VertexBuffer;
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
		ID3D12PipelineState *m_PipelineState;




	public:
		Dx12Engine(HWND a_hwnd);
		~Dx12Engine();

		void InitializeEngine();
		void UpdatePipeline();
		void Render();
		void WaitForPreviousFrame();
		void ShutDown();
		

		inline const HWND GetWindowHandle() { return hwnd; }
		inline const ID3D12Device& GetDevice() { return *m_Device; }

	private:
		void InitBase();
		void TestTraingleInit();
		void TestTraingleRender();
		void RS_CreateDevice();
		void RS_CreateCommandQueue();
		void RS_CreateSwapChain3();
		void RS_CreateRTVDescriptorHeap();
		void RS_CreateRTVs();
		void RS_CreateCmdListAndCmdAllocatorsAndFances();

	};
}