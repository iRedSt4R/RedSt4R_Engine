#include "EngineLoop.h"
#include "../Graphics/Mesh/StaticMesh.h"
#include "../Graphics/Lights/DireLight.h"
#include "../Graphics/Lights/CubeMap.h"
#include "../Graphics/Mesh/ComplexMesh.h"
#include "../GUI/GuiManager.h"
#include "../Graphics/Mesh/Mesh_Manager.h"

#define PBR_TEST_1 "Assets/PBR_A_BALL/export3dcoat.obj", "PBR_A_BALL"
#define PBR_TEST_2 "Assets/PBR_Madam/girl_complete_03.obj", ""
#define PBR_TEST_3 "Assets/PBR_Stol/Stiratoio1.obj", "PBR_Stol"
#define PBR_TEST_4 "Assets/Cerberus/Cerberus_LP.FBX", ""
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
	dx11Engine = new Dx11Engine(hwnd);
	dx11Engine->InitEngine();
	dx11Engine->PreparePipeline();

	gui = new GuiManager(dx11Engine);
	gui->PrepareGui();
	gui->SetupImGuiStyle(true, 1.0f);

	meshManager = new MeshManager(dx11Engine, gui);
	meshManager->AddMesh(PBR_TEST_1);


	DirLightTest = new DireLight(dx11Engine, XMFLOAT3(0.5f, 0.65f, -1.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f), XMFLOAT4(0.881176f, 0.866667f, 0.509804f, 1.0f));
	CubeTest = new CubeMap(dx11Engine, L"MorzeCubeMap.dds");
//	dx11Engine->SetCubeMap(CubeTest);
	

}
void RedSt4R::EngineLoop::Update()
{
	gui->UpdateGui();
	DirLightTest->SetLight();
	dx11Engine->ClearScreen();
	CubeTest->UpdateCubeMap();
	dx11Engine->UpdatePipeline();


	
	CubeTest->BindCubeMap(3);
	meshManager->Update();
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
