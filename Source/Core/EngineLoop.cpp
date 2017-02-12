#include "EngineLoop.h"
#include "../Graphics/Mesh/StaticMesh.h"
#include "../Graphics/Lights/DireLight.h"
#include "../Graphics/Lights/CubeMap.h"
#include "../Graphics/Mesh/ComplexMesh.h"
#include "../GUI/GuiManager.h"
#include "../Graphics/Mesh/Mesh_Manager.h"

using namespace RedSt4R;
using namespace Graphics;

StaticMesh *StaticMeshTest;
const UINT8 *state = SDL_GetKeyboardState(NULL);


RedSt4R::EngineLoop::EngineLoop(HWND a_hwnd)
{
	hwnd = a_hwnd;
}

void RedSt4R::EngineLoop::Begin()
{
	//Running Dx11 Engine
	dx11Engine = new Dx11Engine(hwnd);
	dx11Engine->InitEngine();
	dx11Engine->PreparePipeline();
	engineResources = dx11Engine->GetEngineResource();

	gui = new GuiManager(engineResources->GetDevice(), engineResources->GetDeviceContext());

	meshManager = new MeshManager(dx11Engine);
	gui->SetMeshManager(meshManager);

	DirLightTest = new DireLight(dx11Engine, XMFLOAT3(0.5f, 0.65f, -1.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f), XMFLOAT4(0.881176f, 0.866667f, 0.509804f, 1.0f));
	CubeTest = new CubeMap(dx11Engine, L"Assets/OutputCube2.dds");


	meshManager->AddWithOffset("Assets/Cer.rsmodel", 0);




}
void RedSt4R::EngineLoop::Update()
{
	gui->UpdateGui();
	DirLightTest->SetLight();
	dx11Engine->ClearScreen();
	dx11Engine->UpdatePipeline();


	
	CubeTest->BindCubeMap(3);
	meshManager->Update();
	CubeTest->UpdateCubeMap();
	gui->RenderGui();
	dx11Engine->Render();

}

void RedSt4R::EngineLoop::Close()
{
	dx11Engine->ShutDown();
}

RedSt4R::EngineLoop::~EngineLoop()
{

}
