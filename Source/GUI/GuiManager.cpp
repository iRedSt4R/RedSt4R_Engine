#include "GuiManager.h"

std::vector <std::string> strFilesVec;
std::vector<char*>  charFilesVec;
char buf[256];
bool test;


 char* convert(const std::string & s)
 {
 	char *pc = new char[s.size() + 1];
 	std::strcpy(pc, s.c_str());
 	return pc;
 }
 
 void LOL()
 { 
 	DIR *dir;
 	struct dirent *ent;
 	if ((dir = opendir("C:/Users/Dragoner/Documents/Visual Studio 2015/Projects/RedSt4R_Engine/Assets")) != NULL)
 	{
 		while ((ent = readdir(dir)) != NULL)
 		{
 			std::string tempstr;
 			tempstr = ent->d_name;
			strFilesVec.push_back(tempstr);
 		}
 		
 	}
 	else
 	{

	}
 	closedir(dir);
 
 
 	std::transform(strFilesVec.begin(), strFilesVec.end(), std::back_inserter(charFilesVec), convert);
 
 	for (int i = 0; i < strFilesVec.size(); i++)
 	{
 		std::cout<< charFilesVec[i]<<std::endl;
	}
 }


int myChoice;

GuiManager::GuiManager()
{
	
}


GuiManager::GuiManager(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	m_Device = pDevice;
	m_DeviceContext = pDeviceContext;
	PrepareGui();
	SetupImGuiStyle(true, 0.8f);
}

void GuiManager::SetInputToImGui()
{
	ImGuiIO& io = ImGui::GetIO();
	SDL_Event sdl_Event;

	int x, y;
	SDL_GetMouseState(&x, &y);
	io.MousePos.x = (signed short)(x);
	io.MousePos.y = (signed short)(y);

	SDL_PumpEvents();
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
		io.MouseDown[0] = true;
	else
		io.MouseDown[0] = false;

}

void GuiManager::PrepareGui()
{
	ImGui_ImplDX11_Init(GetActiveWindow(), m_Device, m_DeviceContext);
	LOL();
}


void GuiManager::UpdateGui()
{
	SetInputToImGui();
	ImGui_ImplDX11_NewFrame();


	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("MainWindow", nullptr, ImVec2(1377,700), 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) 
			{

			}
			if (ImGui::MenuItem("Load")) 
			{

			}
			if (ImGui::MenuItem("Exit"))
			{
				exit(EXIT_SUCCESS);
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::Text("");
	ImGui::SameLine(LeftPanelWidth);
	ImGui::Text("FPS: ");
	ImGui::End();

	
	//---------Left Page Start---------//
	ImGui::SetNextWindowSize(ImVec2(LeftPanelWidth, 768));
	ImGui::SetNextWindowPos(ImVec2(0, 19));
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
	ImGui::Begin("Left", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	ImGui::Separator();
	ImGui::SliderFloat("", &g_RoughnessValue, 0.0f, 1.0f, "Rougness = %.3f");
	ImGui::SliderFloat("", &g_MetalnessValue, 0.0f, 1.0f, "Metalness = %.3f");
	ImGui::InputFloat3("Position", g_Position, -1, 0);
	ImGui::InputFloat3("Rotation", g_Rotation, -1, 0);
	ImGui::InputFloat3("Scale", g_Scale, -1, 0);

	ImGui::Separator();
	ImGui::Combo("", &myChoice, (const char**)&charFilesVec[0], charFilesVec.size(), -1);
	if (ImGui::Button("Load Model"))
	{
		std::string tempstr(charFilesVec[myChoice]);
		std::string finalDir = "Assets/" + tempstr;
		RS_WARNING(finalDir);
		meshManager->AddWithOffset(convert(finalDir),0);
	}
	ImGui::Separator();
	ImGui::InputText("String", buf, 256);
	ImGui::End();
	//--------Left Page End------------//

}

void GuiManager::RenderGui()
{
	ImGui::Render();
}

GuiManager::~GuiManager()
{

}
