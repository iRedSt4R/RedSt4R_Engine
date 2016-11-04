#include "Mesh_Manager.h"

RedSt4R::MeshManager::MeshManager(Dx11Engine *Engine, GuiManager *pGUI)
{
	m_Engine = Engine;

	GUI = pGUI;
}

void RedSt4R::MeshManager::AddMesh(char* MeshDir, std::string FolderName)
{
	ComplexMesh *tempMesh = new ComplexMesh(m_Engine, MeshDir, FolderName, GUI);
	vComplexMeshes.push_back(tempMesh);
}

void RedSt4R::MeshManager::Update()
{
	for (int i = 0; i < vComplexMeshes.size(); i++)
	{
		vComplexMeshes[i]->Update();
		vComplexMeshes[i]->Draw();
	}
}

void RedSt4R::MeshManager::RemoveMesh(UINT index)
{

}

void RedSt4R::MeshManager::Clear()
{

}

RedSt4R::MeshManager::~MeshManager()
{

}