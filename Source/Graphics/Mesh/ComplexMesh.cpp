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
	RSMODEL_DESC modelDesc;
	LoadRSModelFile(MeshDir, &modelDesc);

	Importer aImporter;
	const aiScene *aScene = aImporter.ReadFile(modelDesc.fileName, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_GenSmoothNormals);


	for (int m = 0; m < (aScene->mNumMeshes); m++)
	{
		TotalVertices += aScene->mMeshes[m]->mNumFaces * 3;
		//Loading Model
		StaticMesh* tempMesh = new StaticMesh(m_Engine, GUI);
		tempMesh->LoadMeshFromFileWithIndex(aScene, &modelDesc.fileName[0], m, &modelDesc.textureFolder[0], &modelDesc);
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

	this->Scale(35);
}

void RedSt4R::Graphics::ComplexMesh::Update()
{
	if (state[SDL_SCANCODE_KP_1])
	{
		this->RotateX(0.02f);
	}
	if (state[SDL_SCANCODE_KP_3])
	{
		this->RotateX(-0.02f);
	}
	if (state[SDL_SCANCODE_KP_4])
	{
		this->RotateY(0.02f);
	}
	if (state[SDL_SCANCODE_KP_6])
	{
		this->RotateY(-0.02f);
	}
	if (state[SDL_SCANCODE_KP_7])
	{
		this->RotateZ(0.02f);
	}
	if (state[SDL_SCANCODE_KP_9])
	{
		this->RotateZ(-0.02f);
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

void RedSt4R::Graphics::ComplexMesh::LoadRSModelFile(char* filePath, RSMODEL_DESC* a_ModelDesc)
{
	int counter = 0;
	std::ifstream fin;
	fin.open(filePath);

	if (fin.fail())
	{
		std::cout << "Could not open file from desired path!" << std::endl;
	}
	else
	{
		std::cout << "File Opened!" << std::endl;
	}
	while (!fin.eof())
	{
		if (counter = 1) fin >> a_ModelDesc->fileName;
		if (counter = 2) fin >> a_ModelDesc->textureFolder;
		if (counter = 3) fin >> a_ModelDesc->diffuseTexName;
		if (counter = 4) fin >> a_ModelDesc->normalTexName;
		if (counter = 5) fin >> a_ModelDesc->roughnessTexName;
		if (counter = 6) fin >> a_ModelDesc->metalnessTexName;
		//counter++;
	}

	std::cout << a_ModelDesc->fileName << std::endl;
	std::cout << a_ModelDesc->textureFolder << std::endl;
	std::cout << a_ModelDesc->diffuseTexName << std::endl;
	std::cout << a_ModelDesc->normalTexName << std::endl;
	std::cout << a_ModelDesc->roughnessTexName << std::endl;
	std::cout << a_ModelDesc->metalnessTexName << std::endl;

	fin.close();
}


void RedSt4R::Graphics::ComplexMesh::Translate(float x, float y, float z)
{
	for (int i = 0; i < vMeshes.size(); i++)
	{
		vMeshes[i]->Translate(x, y, z);
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
