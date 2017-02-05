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
#define PBR_TEST_5 "Assets/pbr-roughness-scale.dae", ""
#define PBR_TEST_6 "Assets/Crossbow/model.dae", ""
#define PBR_TEST_7 "Assets/Stylized_PBR_Dagger_by_Stephen_Stone/Dagger.obj", ""
#define PBR_TEST_8 "Assets/Cars/j_car.FBX", ""

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

	//Creating Engine Components
	gui = new GuiManager(engineResources);
	meshManager = new MeshManager(dx11Engine, gui);
	DirLightTest = new DireLight(dx11Engine, XMFLOAT3(0.5f, 0.65f, -1.0f), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f), XMFLOAT4(0.881176f, 0.866667f, 0.509804f, 1.0f));
	CubeTest = new CubeMap(dx11Engine, L"Assets/OutputCube2.dds");



	//gui->PrepareGui();
	//gui->SetupImGuiStyle(true, 0.8f);

	meshManager->AddWithOffset("Assets/TestModel.rsmodel", 0);




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
