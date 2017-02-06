#include "Mesh_Manager.h"

RedSt4R::MeshManager::MeshManager(Dx11Engine *Engine)
{
	m_Engine = Engine;
}

void RedSt4R::MeshManager::AddMesh(char* MeshDir, std::string FolderName)
{
	ComplexMesh *tempMesh = new ComplexMesh(m_Engine, MeshDir, FolderName);
	vComplexMeshes.push_back(tempMesh);
}

void RedSt4R::MeshManager::Update()
{
	for (int i = 0; i < vComplexMeshes.size(); i++)
	{
		vComplexMeshes[i]->Update();
	}
}

void RedSt4R::MeshManager::RemoveMesh(UINT index)
{

}

void RedSt4R::MeshManager::Clear()
{

}

void RedSt4R::MeshManager::AddWithOffset(char* MeshDir, float x_offset)
{
	ComplexMesh *tempMesh = new ComplexMesh(m_Engine, MeshDir, "");
	tempMesh->Translate(x_offset, 0, 0);
	vComplexMeshes.push_back(tempMesh);
}

RedSt4R::MeshManager::~MeshManager()
{

}
