#include "ComplexMesh.h"

RedSt4R::Graphics::ComplexMesh::ComplexMesh(Dx11Engine *Engine, char* MeshDir, std::string FolderName, GuiManager *pGUI)
{
	m_Engine = Engine;

	RS_Material_Desc matDesc;

	LoadMeshFromFile(MeshDir, FolderName);
	RS_ERROR("CREATED!!!")

	GUI = pGUI;
}

void RedSt4R::Graphics::ComplexMesh::LoadMeshFromFile(char* MeshDir, std::string FolderName)
{
	Importer aImporter;
	const aiScene *aScene = aImporter.ReadFile(MeshDir, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_GenSmoothNormals);


	for (int m = 0; m < (aScene->mNumMeshes); m++)
	{
		TotalVertices += aScene->mMeshes[m]->mNumFaces * 3;
		//Loading Model
		StaticMesh* tempMesh = new StaticMesh(m_Engine, GUI);
		tempMesh->LoadMeshFromFileWithIndex(aScene, MeshDir, m, FolderName);
		//tempMesh->CreateAABB();
		vMeshes.push_back(tempMesh);

		//Loading Material
		RS_Material_Desc *materialDesc = tempMesh->GetMaterialInfo();
		Material *tempMaterial = new Material(m_Engine);
		tempMaterial->CreateMaterial(*materialDesc);
		vMaterials.push_back(tempMaterial);
		vMeshes[m]->AssignMaterial(vMaterials[m]);
	}
	RS_LOG("Total Vertices: "<<TotalVertices)

	this->Scale(3);
}

void RedSt4R::Graphics::ComplexMesh::Update()
{
	if (state[SDL_SCANCODE_KP_1])
	{
		this->RotateX(0.008f);
	}
	if (state[SDL_SCANCODE_KP_3])
	{
		this->RotateX(-0.008f);
	}
	if (state[SDL_SCANCODE_KP_4])
	{
		this->RotateY(0.008f);
	}
	if (state[SDL_SCANCODE_KP_6])
	{
		this->RotateY(-0.008f);
	}
	if (state[SDL_SCANCODE_KP_7])
	{
		this->RotateZ(0.008f);
	}
	if (state[SDL_SCANCODE_KP_9])
	{
		this->RotateZ(-0.008f);
	}
}

void RedSt4R::Graphics::ComplexMesh::Draw()
{
	//Update();
//	m_Engine->GetDeviceContext()->RSSetState(m_Engine->GetBackCullingRasterizationState());

	for (int i = 0; i < vMeshes.size(); i++)
	{
		vMaterials[i]->SetRoughness(GUI->g_RoughnessValue);
		vMaterials[i]->SetGlossiness(GUI->g_GlossinessValue);
		vMeshes[i]->Draw();
	}
}

void RedSt4R::Graphics::ComplexMesh::Scale(float scaleFactor)
{
	for (int i = 0; i < vMeshes.size(); i++)
	{
		vMeshes[i]->Scale(scaleFactor);
	}
}

void RedSt4R::Graphics::ComplexMesh::RotateX(float angle)
{
	for (int i = 0; i < vMeshes.size(); i++)
	{
		vMeshes[i]->RotateX(angle);
	}
}

void RedSt4R::Graphics::ComplexMesh::RotateY(float angle)
{
	for (int i = 0; i < vMeshes.size(); i++)
	{
		vMeshes[i]->RotateY(angle);
	}
}

void RedSt4R::Graphics::ComplexMesh::RotateZ(float angle)
{
	for (int i = 0; i < vMeshes.size(); i++)
	{
		vMeshes[i]->RotateZ(angle);
	}
}

RedSt4R::Graphics::ComplexMesh::~ComplexMesh()
{

}
