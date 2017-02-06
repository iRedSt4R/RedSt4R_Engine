#include "ComplexMesh.h"

RedSt4R::Graphics::ComplexMesh::ComplexMesh(Dx11Engine *Engine, char* MeshDir, std::string FolderName)
{
	m_Engine = Engine;

	LoadMeshFromFile(MeshDir, FolderName);
}

void RedSt4R::Graphics::ComplexMesh::Begin()
{
	vPosition = XMFLOAT3(0, 0, 0);
	vRotation = XMFLOAT3(0, 0, 0);
	vScale = XMFLOAT3(1, 1, 1);
}

void RedSt4R::Graphics::ComplexMesh::Update()
{
	for (int i = 0; i < vMeshes.size(); i++)
	{
		vMeshes[i]->Update();
	}

	//For fast access to rotate mesh with numpad keys
	TestRotation();
}


void RedSt4R::Graphics::ComplexMesh::End()
{

}

RedSt4R::Graphics::ComplexMesh::~ComplexMesh()
{

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
		StaticMesh* tempMesh = new StaticMesh(m_Engine);
		tempMesh->LoadMeshFromFileWithIndex(aScene, &modelDesc.fileName[0], m, &modelDesc.textureFolder[0], &modelDesc);
		vMeshes.push_back(tempMesh);

		//Loading Material
		RS_Material_Desc *materialDesc = tempMesh->GetMaterialInfo();
		Material *tempMaterial = new Material(m_Engine);
		tempMaterial->CreateMaterial(*materialDesc);
		vMaterials.push_back(tempMaterial);
		vMeshes[m]->AssignMaterial(vMaterials[m]);
	}
	RS_LOG("Total Vertices: "<<TotalVertices)

	this->Scale(15);
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

void RedSt4R::Graphics::ComplexMesh::TestRotation()
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

void RedSt4R::Graphics::ComplexMesh::SetObjectPosition(XMFLOAT3 position)
{
	for (int i = 0; i < vMeshes.size(); i++)
	{
		vMeshes[i]->SetObjectPosition(position);
	}

}

void RedSt4R::Graphics::ComplexMesh::SetObjectScale(XMFLOAT3 scale)
{
	for (int i = 0; i < vMeshes.size(); i++)
	{
		vMeshes[i]->SetObjectScale(scale);
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

void RedSt4R::Graphics::ComplexMesh::Translate(float x, float y, float z)
{
	for (int i = 0; i < vMeshes.size(); i++)
	{
		vMeshes[i]->Translate(x, y, z);
	}
}
