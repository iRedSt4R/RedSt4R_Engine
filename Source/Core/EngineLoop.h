#pragma once
#include "../Graphics/Mesh/StaticMesh.h"
#include "../Graphics/Lights/DireLight.h"
#include "../Graphics/Lights/CubeMap.h"
#include "../Graphics/Mesh/ComplexMesh.h"
#include "../Graphics/Mesh/Mesh_Manager.h"
#include "../Core/D3D11Engine/Dx11Engine.h"
#include "../Core/D3D12Engine/Dx12Engine.h"
#include "../GUI/GuiManager.h"
#include "EngineResources.h"

namespace RedSt4R
{
	class EngineLoop
	{
	private:
		HWND hwnd;
		bool bIsRunning = false;


	public:
		DireLight *DirLightTest;
		CubeMap *CubeTest;
		GuiManager *gui;
		MeshManager *meshManager;
		EngineResources *engineResources;

		RedSt4R::Dx11Engine *dx11Engine;
		EngineLoop(HWND a_hwnd);
		~EngineLoop();

		void Begin();
		void Update();
		void Close();

	};
}
