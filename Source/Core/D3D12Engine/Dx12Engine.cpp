#include "Dx12Engine.h"

const int RedSt4R::Dx12Engine::frameBufferCount;

struct Vertex { XMFLOAT3 Pos; };


RedSt4R::Dx12Engine::Dx12Engine(HWND a_hwnd)
{
	hwnd = a_hwnd;
}

void RedSt4R::Dx12Engine::InitializeEngine()
{
	std::cout << frameBufferCount << std::endl;
	InitBase();
	TestTraingleInit();
}

void RedSt4R::Dx12Engine::InitBase()
{
	RS_CreateDevice();
	RS_CreateCommandQueue();
	RS_CreateSwapChain3();
	RS_CreateRTVDescriptorHeap();
	RS_CreateRTVs();
	RS_CreateCmdListAndCmdAllocatorsAndFances();
}

void RedSt4R::Dx12Engine::TestTraingleInit()
{
	//-----Root Signature------
	CD3DX12_ROOT_SIGNATURE_DESC rsDesc;
	rsDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob *signature;
	hr = D3D12SerializeRootSignature(&rsDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr);
	if (FAILED(hr)) RS_ERROR("Failed Serializing Root Signature!")

	hr = m_Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature));
	if (FAILED(hr)) RS_ERROR("Failed Creating Root Signature!")

	//-------Shaders---------
	hr = D3DCompileFromFile(L"VertexShader.hlsl",nullptr, nullptr,
		"main","vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,&m_VertexShader,&m_ErrorShaderBlob);
	if (FAILED(hr)) OutputDebugStringA((char*)m_ErrorShaderBlob->GetBufferPointer());

	D3D12_SHADER_BYTECODE vertexShaderByteCode;
	vertexShaderByteCode.pShaderBytecode = m_VertexShader->GetBufferPointer();
	vertexShaderByteCode.BytecodeLength = m_VertexShader->GetBufferSize();

	hr = D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr,
		"main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0, &m_PixelShader, &m_ErrorShaderBlob);
	if (FAILED(hr)) OutputDebugStringA((char*)m_ErrorShaderBlob->GetBufferPointer());

	D3D12_SHADER_BYTECODE pixelShaderByteCode;
	pixelShaderByteCode.pShaderBytecode = m_PixelShader->GetBufferPointer();
	pixelShaderByteCode.BytecodeLength = m_PixelShader->GetBufferSize();

	//-----Creating Input Element Desc (Input Layout)--------
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{ 
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
	inputLayoutDesc.NumElements = sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	inputLayoutDesc.pInputElementDescs = inputLayout;

	//--------Creating Pipeline State Object----------
	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineDesc = {};
	graphicsPipelineDesc.InputLayout = inputLayoutDesc; // the structure describing our input layout
	graphicsPipelineDesc.pRootSignature = m_RootSignature; // the root signature that describes the input data this pso needs
	graphicsPipelineDesc.VS = vertexShaderByteCode; // structure describing where to find the vertex shader bytecode and how large it is
	graphicsPipelineDesc.PS = pixelShaderByteCode; // same as VS but for pixel shader
	graphicsPipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // type of topology we are drawing
	graphicsPipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the render target
	graphicsPipelineDesc.SampleDesc = sampleDesc; // must be the same sample description as the swapchain and depth/stencil buffer
	graphicsPipelineDesc.SampleMask = 0xffffffff; // sample mask has to do with multi-sampling. 0xffffffff means point sampling is done
	graphicsPipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); // a default rasterizer state.
	graphicsPipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT); // a default blent state.
	graphicsPipelineDesc.NumRenderTargets = 1; // we are only binding one render target

	hr = m_Device->CreateGraphicsPipelineState(&graphicsPipelineDesc, IID_PPV_ARGS(&m_PipelineState));
	if(FAILED(hr)) RS_ERROR("Failed Creating Graphics Pipeline!")


	//Vertex List (Array)
		Vertex vList[] = {
			{ { 0.0f, 0.5f, 0.5f } },
			{ { 0.5f, -0.5f, 0.5f } },
			{ { -0.5f, -0.5f, 0.5f } },
	};

	int vertexBufferSize = sizeof(vList);

	//Creating Default Heap (Heap Only Available On GPU)
	m_Device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize), D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_VertexBuffer));
	m_VertexBuffer->SetName(L"Vertex Buffer(GPU: Default Heap)");

	//Creating Upload Heap (To Upload Vertex Data From CPU to GPU, because we can't directly copy data from cpu to gpu default heap)
	ID3D12Resource *m_VertexBufferUploadHeap;
	m_Device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_VertexBufferUploadHeap));

	D3D12_SUBRESOURCE_DATA vertexDataToUpload = {};
	vertexDataToUpload.pData = reinterpret_cast<BYTE*>(vList);
	vertexDataToUpload.RowPitch = vertexBufferSize;
	vertexDataToUpload.SlicePitch = vertexBufferSize;

	UpdateSubresources(m_GraphicsCommandList, m_VertexBuffer, m_VertexBufferUploadHeap, 0, 0, 1, &vertexDataToUpload);
	
	m_GraphicsCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
	m_GraphicsCommandList->Close();

	ID3D12CommandList* ppCommandLists[] = { m_GraphicsCommandList };
	m_CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	
	fenceValue[m_FrameIndex]++;

	hr = m_CommandQueue->Signal(m_Fence[m_FrameIndex], fenceValue[m_FrameIndex]);
	if(FAILED(hr)) RS_ERROR("Failed Singlaling After Executing Command List For Updating Vertex Buffer")
	
	m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.StrideInBytes = sizeof(Vertex);
	m_VertexBufferView.SizeInBytes = vertexBufferSize;

	//Viewport and Scissors
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = 800;
	m_Viewport.Height = 600;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	m_ScissorRect.left = 0;
	m_ScissorRect.top = 0;
	m_ScissorRect.right = 800;
	m_ScissorRect.bottom = 600;

}

void RedSt4R::Dx12Engine::TestTraingleRender()
{


	
}

void RedSt4R::Dx12Engine::UpdatePipeline()
{

}

void RedSt4R::Dx12Engine::Render()
{
	WaitForPreviousFrame();

	hr = m_CommandAllocator[m_FrameIndex]->Reset();
	if (FAILED(hr)) RS_ERROR("Failed Resetting Command Allocator!")

	hr = m_GraphicsCommandList->Reset(m_CommandAllocator[m_FrameIndex], m_PipelineState);
	if (FAILED(hr)) RS_ERROR("Failed Resetting Command List!")


	m_GraphicsCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle (m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_FrameIndex, m_RtvDescriptorSize);
	m_GraphicsCommandList->OMSetRenderTargets(1, &rtvHandle, 0, nullptr);
	const float clearColor[] = { 0.2f, 0.8f, 0.4f, 1.0f };
	m_GraphicsCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, &m_ScissorRect);

	m_GraphicsCommandList->SetGraphicsRootSignature(m_RootSignature); // set the root signature
	m_GraphicsCommandList->SetPipelineState(m_PipelineState);
	m_GraphicsCommandList->RSSetViewports(1, &m_Viewport); // set the viewports
	m_GraphicsCommandList->RSSetScissorRects(1, &m_ScissorRect); // set the scissor rects
	m_GraphicsCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // set the primitive topology
	m_GraphicsCommandList->IASetVertexBuffers(0, 1, &m_VertexBufferView); // set the vertex buffer (using the vertex buffer view)
	m_GraphicsCommandList->DrawInstanced(3, 1, 0, 0); // finally draw 3 vertices (draw the triangle)
	
	m_GraphicsCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_RenderTargets[m_FrameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	
	hr = m_GraphicsCommandList->Close();
	if (FAILED(hr)) RS_ERROR("Failed Closing Graphics Command List!")

	ID3D12CommandList *ppCommandList[] = { m_GraphicsCommandList };
	m_CommandQueue->ExecuteCommandLists(_countof(ppCommandList), ppCommandList);
	
	//fenceValue[m_FrameIndex]++;
	hr = m_CommandQueue->Signal(m_Fence[m_FrameIndex], fenceValue[m_FrameIndex]);
	if (FAILED(hr)) RS_ERROR("Failed Creating a Signal after executing Command List!")

	hr = m_SwapChain3->Present(0, 0);
	RS_LOG("Presented buffer nr: "<<m_FrameIndex)

	if(FAILED(hr)) RS_ERROR("Failed Presenting SwapChain3!")
}

void RedSt4R::Dx12Engine::WaitForPreviousFrame()
{
	// swap the current rtv buffer index so we draw on the correct buffer
	m_FrameIndex = m_SwapChain3->GetCurrentBackBufferIndex();

	// if the current fence value is still less than "fenceValue", then we know the GPU has not finished executing
	// the command queue since it has not reached the "commandQueue->Signal(fence, fenceValue)" command
	if (m_Fence[m_FrameIndex]->GetCompletedValue() < fenceValue[m_FrameIndex])
	{
		// we have the fence create an event which is signaled once the fence's current value is "fenceValue"
		hr = m_Fence[m_FrameIndex]->SetEventOnCompletion(fenceValue[m_FrameIndex], fenceEvent);
		if (FAILED(hr)) RS_ERROR ("Failed Setting an Event On Completion on Fence!")

		// We will wait until the fence has triggered the event that it's current value has reached "fenceValue". once it's value
		// has reached "fenceValue", we know the command queue has finished executing
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	// increment fenceValue for next frame
	fenceValue[m_FrameIndex]++;
}

void RedSt4R::Dx12Engine::RS_CreateDevice()
{
	hr = CreateDXGIFactory(IID_PPV_ARGS(&m_DxgiFactor));
	if (FAILED(hr))
	{
		RS_ERROR("Failed Creating dxgiFactor!");
	}

	int adapterIndex = 0;
	bool bFoundAdapter = false;

	while (m_DxgiFactor->EnumAdapters1(adapterIndex, &m_Adapter) != DXGI_ERROR_NOT_FOUND)
	{
		RS_LOG("Checked Device nr:" << adapterIndex)
			DXGI_ADAPTER_DESC1 aDesc;
		m_Adapter->GetDesc1(&aDesc);
		RS_LOG(aDesc.DedicatedVideoMemory)
			if (aDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				RS_WARNING(adapterIndex << " is Software Device!")
					adapterIndex++;
				continue;
			}
		auto hr = D3D12CreateDevice(m_Adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
		if (SUCCEEDED(hr))
		{
			RS_LOG("Found a compatible DX12 GPU!")
				adapterIndex++;
		}
	}

	m_DxgiFactor->EnumAdapters1(0, &m_Adapter);

	hr = D3D12CreateDevice(m_Adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));
	if (FAILED(hr))
	{
		RS_ERROR("Failed Created D3D12Device!")
	}
}

void RedSt4R::Dx12Engine::RS_CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC cqDesc = {};
	cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cqDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	cqDesc.NodeMask = 0;

	hr = m_Device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&m_CommandQueue));
	if(FAILED(hr)) RS_ERROR("Failed Creating Command Queue!")
}

void RedSt4R::Dx12Engine::RS_CreateSwapChain3()
{
	DXGI_SWAP_CHAIN_DESC scDesc = {};
	DXGI_MODE_DESC modeDesc = {};
	DXGI_SAMPLE_DESC sampleDesc = {};
	DXGI_USAGE usageType = {};
	DXGI_RATIONAL refresRate = {};

	//BackBuffer Desc
	modeDesc.Width = 800;
	modeDesc.Height = 600;
	modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

	scDesc.BufferCount = frameBufferCount;
	scDesc.BufferDesc = modeDesc;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.SampleDesc = sampleDesc;
	scDesc.OutputWindow = GetActiveWindow();
	scDesc.Windowed = true;

	IDXGISwapChain *tempSwapChain;

	hr = m_DxgiFactor->CreateSwapChain(m_CommandQueue, &scDesc, &tempSwapChain);
	if(FAILED(hr)) RS_ERROR("Failed Creating SwapChain!!!!")

	m_SwapChain3 = static_cast<IDXGISwapChain3*>(tempSwapChain);
	m_FrameIndex = m_SwapChain3->GetCurrentBackBufferIndex();

	RS_LOG("Current Back Buffer Index: "<<m_FrameIndex)
}

void RedSt4R::Dx12Engine::RS_CreateRTVDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC dhDesc = {};
	dhDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	dhDesc.NumDescriptors = frameBufferCount;
	dhDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	hr = m_Device->CreateDescriptorHeap(&dhDesc, IID_PPV_ARGS(&m_DescriptorHeap));
	if (FAILED(hr)) RS_ERROR("Failed Creating RTVs Descriptor Heap")

	m_RtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	RS_LOG("RTV Descriptor Size: "<<m_RtvDescriptorSize)
}

void RedSt4R::Dx12Engine::RS_CreateRTVs()
{
	rtvHandle = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < frameBufferCount; i++)
	{
		hr = m_SwapChain3->GetBuffer(i, IID_PPV_ARGS(&m_RenderTargets[i]));
		if (FAILED(hr)) RS_ERROR("Failed getting buffer nr." << i << " from swap chain3!")

		m_Device->CreateRenderTargetView(m_RenderTargets[i], nullptr, rtvHandle);

		rtvHandle.Offset(1, m_RtvDescriptorSize);
	}
}

void RedSt4R::Dx12Engine::RS_CreateCmdListAndCmdAllocatorsAndFances()
{
	for (int i = 0; i < frameBufferCount; i++)
	{	
		hr = m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator[i]));
		if(FAILED(hr)) RS_ERROR("Failed Creating Command Allocator!")
	}

	hr = m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator[0], NULL, IID_PPV_ARGS(&m_GraphicsCommandList));
	if (FAILED(hr)) RS_ERROR("Failed Creating Graphics Command List!")

	m_GraphicsCommandList->Close();

	//Creating 3 Fences and with 0 initial value
	for (int i = 0; i < frameBufferCount; i++)
	{
		hr = m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence[i]));
		if (FAILED(hr)) RS_ERROR("Failed Creating a Fence!")

		fenceValue[i] = 0;
	}

	//Creating Fence Event
	fenceEvent = CreateEvent(nullptr, false, false, nullptr);
	if(fenceEvent == nullptr) RS_ERROR	("Failed Creating Fence Event!")
}

void RedSt4R::Dx12Engine::ShutDown()
{
	if (m_DescriptorHeap) m_DescriptorHeap->Release();
	if (m_CommandQueue) m_CommandQueue->Release();
	if (m_SwapChain3) m_SwapChain3->Release();
	if (m_Device) m_Device->Release();
}

RedSt4R::Dx12Engine::~Dx12Engine()
{

}
