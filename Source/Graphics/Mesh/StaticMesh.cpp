#include "StaticMesh.h"
#include "../Material/Material.h"

using namespace RedSt4R;
using namespace Graphics;

wchar_t *GetWC(char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

RedSt4R::Graphics::StaticMesh::StaticMesh(Dx11Engine *Engine)
{
	m_Engine = Engine;
	m_Device = m_Engine->GetDevice();
	m_DeviceContext = m_Engine->GetDeviceContext();

	vPosition = XMFLOAT3(0, 0, 0);
	vRotation = XMFLOAT3(0, 0, 0);
	vScale = XMFLOAT3(0.2, 0.2, 0.2);

	DebugLineVertexVec.resize(2);
	frustumPlanes.resize(6);
}

RedSt4R::Graphics::StaticMesh::StaticMesh(char *filePath, Dx11Engine *Engine, wchar_t* DiffuseTextureDir, wchar_t* NormalTextureDir)
{
	m_Engine = Engine;
	m_Device = m_Engine->GetDevice();
	m_DeviceContext = m_Engine->GetDeviceContext();

	vPosition = XMFLOAT3(0, 0, 0);
	vRotation = XMFLOAT3(0, 0, 0);
	vScale = XMFLOAT3(0.2, 0.2, 0.2);

	LoadMeshFromFile(filePath);

	//--------------------Material Info--------------------//
	TestMaterial = new Material(m_Engine);
	matDesc.diffuseTextureDir = DiffuseTextureDir;
	matDesc.normalTextureDir = NormalTextureDir;
	TestMaterial->CreateMaterial(matDesc);
	
}

void RedSt4R::Graphics::StaticMesh::LoadMeshFromFile(char *filePath)
{

	Importer aImporter;
	const aiScene *aScene = aImporter.ReadFile(filePath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_GenSmoothNormals);

	RS_WARNING("Number Meshes:" << aScene->mNumMeshes)

	for (int m = 0; m < (aScene->mNumMeshes); m++)
	{
		aiMesh* aMesh = aScene->mMeshes[m];
		FaceCount = aMesh->mNumFaces;
		VertexCount = FaceCount * 3;

		for (unsigned int f = 0; f < FaceCount; f++)
		{
			const aiFace &aFace = aMesh->mFaces[f];

			for (int i = 0; i < 3; i++)
			{

				aiVector3D vertexPos = aMesh->mVertices[aFace.mIndices[i]];
				aiVector3D textCoord = aMesh->mTextureCoords[0][aFace.mIndices[i]];
				aiVector3D normal = aMesh->HasNormals() ? aMesh->mNormals[aFace.mIndices[i]] : aiVector3D(1.0f, 1.0f, 1.0f);
				aiVector3D tangent = aMesh->HasTangentsAndBitangents() ? aMesh->mTangents[aFace.mIndices[i]] : aiVector3D(1.0f, 1.0f, 1.0f);

				VertexPosVec.push_back(vertexPos);
				TexCoordVec.push_back(textCoord);
				NormalsVec.push_back(normal);
				TangentsVec.push_back(tangent);
			}
		}
	}

		for (int i = 0; i < VertexPosVec.size(); i++)
		{
			VertexB tempVertexT;

			//position and textCoord
			tempVertexT.Position.x = VertexPosVec[i].x;
			tempVertexT.Position.y = VertexPosVec[i].y;
			tempVertexT.Position.z = VertexPosVec[i].z;
			tempVertexT.texCoord.x = TexCoordVec[i].x;
			tempVertexT.texCoord.y = (1.0f - TexCoordVec[i].y);
			tempVertexT.Normal.x = NormalsVec[i].x;
			tempVertexT.Normal.y = NormalsVec[i].y;
			tempVertexT.Normal.z = NormalsVec[i].z;
			tempVertexT.Tangent.x = TangentsVec[i].x;
			tempVertexT.Tangent.y = TangentsVec[i].y;
			tempVertexT.Tangent.z = TangentsVec[i].z;

			VertexVec.push_back(tempVertexT);
		}
		//----------------Calculating Tangent and BiTagnent-------------------//

	RS_WARNING(VertexVec.size())
	InitBuffers();
	CreateAABB();
}

void RedSt4R::Graphics::StaticMesh::LoadMeshFromFileWithIndex(const aiScene *a_aScene, char * filePath, int index, std::string FolderName, RSMODEL_DESC* a_modelDesc)
{
	

	const aiScene *aScene = a_aScene;

	RS_WARNING("Number Meshes:" << aScene->mNumMeshes)

		RS_LOG("Number Vertices: " << aScene->mMeshes[index]->mNumVertices)

		aiMesh* aMesh = aScene->mMeshes[index];
		FaceCount = aMesh->mNumFaces;
		VertexCount = FaceCount * 3;
		//VertexCount = aMesh->mNumVertices;

		for (unsigned int i = 0; i < aMesh->mNumVertices; i++)
		{
			aiVector3D vertexPos = aMesh->mVertices[i];
			aiVector3D textCoord = aMesh->HasTextureCoords(0) ? aMesh->mTextureCoords[0][i] : aiVector3D(1, 1, 1);
			aiVector3D normal = aMesh->HasNormals() ? aMesh->mNormals[i] : aiVector3D(1.0f, 1.0f, 1.0f);
			//aiVector3D tangent = aMesh->HasTangentsAndBitangents() ? aMesh->mTangents[i] : aiVector3D(1.0f, 1.0f, 1.0f);
			aiVector3D tangent = aMesh->mTangents[i];

			VertexPosVec.push_back(vertexPos);
			TexCoordVec.push_back(textCoord);
			NormalsVec.push_back(normal);
			TangentsVec.push_back(tangent);
		}

		Indices.resize(VertexCount);
		for (unsigned int f = 0; f < FaceCount; f++)
		{
			const aiFace &aFace = aMesh->mFaces[f];

			Indices.push_back(aFace.mIndices[0]);
			Indices.push_back(aFace.mIndices[1]);
			Indices.push_back(aFace.mIndices[2]);
		}

			const aiMaterial* material = aScene->mMaterials[aMesh->mMaterialIndex];
			int a = 5;
			int texIndex = 0;
			aiString path;  // filename
			std::string currentPath = "Assets/" + FolderName + "/";
			
			if (material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path) == AI_SUCCESS && a_modelDesc->flag == "use_def")
			{
				std::string finalPath = currentPath + path.data;
				matDesc.diffuseTextureDir = GetWC(&finalPath[0]);
			}
			else
			{
				matDesc.diffuseTextureDir = GetWC(&a_modelDesc->diffuseTexName[0]);
			}

			if (material->GetTexture(aiTextureType_HEIGHT, texIndex, &path) == AI_SUCCESS && a_modelDesc->flag == "use_def")
			{
				std::string finalPath = currentPath + path.data;
				matDesc.normalTextureDir = GetWC(&finalPath[0]);
			}
			else
			{
				matDesc.normalTextureDir = GetWC(&a_modelDesc->normalTexName[0]);
			}

			if (material->GetTexture(aiTextureType_SPECULAR, texIndex, &path) == AI_SUCCESS)
			{
				std::string finalPath = currentPath + path.data;
				matDesc.rougnessTextureDir = GetWC(&finalPath[0]);
			}
			else
			{
				matDesc.rougnessTextureDir = GetWC(&a_modelDesc->roughnessTexName[0]);
			}


			for (int i = 0; i < VertexPosVec.size(); i++)
			{
				VertexB tempVertexT;

				//position and textCoord
				tempVertexT.Position.x = VertexPosVec[i].x;
				tempVertexT.Position.y = VertexPosVec[i].y;
				tempVertexT.Position.z = VertexPosVec[i].z;
				tempVertexT.texCoord.x = TexCoordVec[i].x;
				tempVertexT.texCoord.y = (1.0f - TexCoordVec[i].y);
				tempVertexT.Normal.x = NormalsVec[i].x;
				tempVertexT.Normal.y = NormalsVec[i].y;
				tempVertexT.Normal.z = NormalsVec[i].z;
				tempVertexT.Tangent.x = TangentsVec[i].x;
				tempVertexT.Tangent.y = TangentsVec[i].y;
				tempVertexT.Tangent.z = TangentsVec[i].z;

				VertexVec.push_back(tempVertexT);
			}
			
			if (a_modelDesc->flag != "use_def")
			{
				matDesc.metallicTextureDir = GetWC(&a_modelDesc->metalnessTexName[0]);
			}

			InitBuffers();


}


void RedSt4R::Graphics::StaticMesh::AssignMaterial(Material *a_Material)
{
	TestMaterial = a_Material;
}

void RedSt4R::Graphics::StaticMesh::CreateAABB()
{
	aiVector3D minVertex = aiVector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	aiVector3D maxVertex = aiVector3D(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (UINT i = 0; i < VertexVec.size(); i++)
	{
		// The minVertex and maxVertex will most likely not be actual vertices in the model, but vertices
		// that use the smallest and largest x, y, and z values from the model to be sure ALL vertices are
		// covered by the bounding volume

		//Get the smallest vertex 
		minVertex.x = min(minVertex.x, VertexVec[i].Position.x);    // Find smallest x value in model
		minVertex.y = min(minVertex.y, VertexVec[i].Position.y);    // Find smallest y value in model
		minVertex.z = min(minVertex.z, VertexVec[i].Position.z);    // Find smallest z value in model

															  //Get the largest vertex 
		maxVertex.x = max(maxVertex.x, VertexVec[i].Position.x);    // Find largest x value in model
		maxVertex.y = max(maxVertex.y, VertexVec[i].Position.y);    // Find largest y value in model
		maxVertex.z = max(maxVertex.z, VertexVec[i].Position.z);    // Find largest z value in model
	}
	// Our AABB [0] is the min vertex and [1] is the max
	AABBVector.push_back(minVertex);
	AABBVector.push_back(maxVertex);

	RS_LOG("Lowest AABB.x: " << minVertex.x <<" Lowest AABB.y: "<<minVertex.y << " Lowest AABB.z: " << minVertex.z)
	RS_LOG("Highest AABB.x: " << maxVertex.x << " Highest AABB.y: " << maxVertex.y << " Highest AABB.z: " << maxVertex.z)
}

void RedSt4R::Graphics::StaticMesh::InitBuffers()
{
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(UINT) * Indices.size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA IBData;
	ZeroMemory(&IBData, sizeof(D3D11_SUBRESOURCE_DATA));

	IBData.pSysMem = &Indices[0];

	auto hrr = m_Device->CreateBuffer(&indexBufferDesc, &IBData, &m_IndexBuffer);
	if (FAILED(hrr)) RS_WARNING("Failed Creating Index BUFFER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexB) * VertexVec.size();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA VBData;
	ZeroMemory(&VBData, sizeof(D3D11_SUBRESOURCE_DATA));

	VBData.pSysMem = &VertexVec[0];
	//VBData.pSysMem = VertexVec.data();
	auto hrrr = m_Device->CreateBuffer(&vertexBufferDesc, &VBData, &m_MeshVertexBuffer);
	if(FAILED(hrrr)) RS_WARNING("Failed Creating VERTEX BUFFER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

	UINT stride = sizeof(VertexB);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, &m_MeshVertexBuffer, &stride, &offset);

	//Constant Buffer
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(D3D11_BUFFER_DESC));

	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(cbPerObject);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	cbd.MiscFlags = 0;

	m_Device->CreateBuffer(&cbd, NULL, &m_MeshConsantBuffer);


	VertexVecSize = VertexVec.size();
}

void RedSt4R::Graphics::StaticMesh::UpdateMeshBuffers()
{
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(VertexB);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, &m_MeshVertexBuffer, &stride, &offset);

	mPosition = XMMatrixTranslation(vPosition.x, vPosition.y, vPosition.z);
	mRotationX = XMMatrixRotationX(vRotation.x);
	mRotationY = XMMatrixRotationY(vRotation.y);
	mRotationZ = XMMatrixRotationZ(vRotation.z);

	mRotation = mRotationX * mRotationY * mRotationZ;
	
	mScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);

	mWorld = XMMatrixIdentity();
	mWorld = mPosition * mRotation * mScale;
	mWVP = XMMatrixIdentity();
	mWVP = mWorld * m_Engine->GetCameraView() * m_Engine->GetCameraProjection();

	//Filling constant buffer for shaders
	m_MeshConstantBufferObject.WVP = XMMatrixTranspose(mWVP);
	m_MeshConstantBufferObject.World = XMMatrixTranspose(mWorld);
	m_MeshConstantBufferObject.CameraPos.x = XMVectorGetX(m_Engine->FPSCamera->GetCameraPosition());
	m_MeshConstantBufferObject.CameraPos.y = XMVectorGetY(m_Engine->FPSCamera->GetCameraPosition());
	m_MeshConstantBufferObject.CameraPos.z = XMVectorGetZ(m_Engine->FPSCamera->GetCameraPosition());

	m_DeviceContext->UpdateSubresource(m_MeshConsantBuffer, 0, 0, &m_MeshConstantBufferObject, 0, 0);
	m_DeviceContext->VSSetConstantBuffers(0, 1, &m_MeshConsantBuffer);
}

void RedSt4R::Graphics::StaticMesh::LoadRSModelFile(char* filePath, RSMODEL_DESC* a_ModelDesc)
{
	int counter = 0;
	std::ifstream fin;
	fin.open("Assets/TestModel.rsmodel");

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

void RedSt4R::Graphics::StaticMesh::InitDebugAABB()
{
	frustumPlanes = m_Engine->FPSCamera->GetFrustrumPlanes(m_Engine->GetCameraView(), m_Engine->GetCameraProjection());

	DebugLineVertexVec[0].Position.x = frustumPlanes[0].x;
	DebugLineVertexVec[0].Position.y = frustumPlanes[0].y;
	DebugLineVertexVec[0].Position.z = frustumPlanes[0].z;

	DebugLineVertexVec[1].Position.x = frustumPlanes[1].x;
	DebugLineVertexVec[1].Position.y = frustumPlanes[1].y;
	DebugLineVertexVec[1].Position.z = frustumPlanes[1].z;

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 2;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA VBData;
	ZeroMemory(&VBData, sizeof(D3D11_SUBRESOURCE_DATA));

	VBData.pSysMem = &DebugLineVertexVec[0];
	m_Device->CreateBuffer(&vertexBufferDesc, &VBData, &m_DebugAABBVertexBuffer);
//	if (FAILED(hrrr)) RS_WARNING("Failed Creating Debug Line Vertex Buffer!!!!!!")
}

void RedSt4R::Graphics::StaticMesh::DrawAABB()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, &m_DebugAABBVertexBuffer, &stride, &offset);
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_DeviceContext->IASetInputLayout(m_Engine->GetInputLayout());

	m_DeviceContext->Draw(2, 0);
}
//
void RedSt4R::Graphics::StaticMesh::CheckIfCulled()
{
	bCulled = false;
	for (int planeID = 0; planeID < 6; ++planeID)
	{
		XMVECTOR planeNormal = XMVectorSet(frustumPlanes[planeID].x, frustumPlanes[planeID].y, frustumPlanes[planeID].z, 0.0f);
		float planeConstant = frustumPlanes[planeID].w;

		// Check each axis (x,y,z) to get the AABB vertex furthest away from the direction the plane is facing (plane normal)
		XMFLOAT3 axisVert;
		//XMFLOAT3 axisVert2;

		// x-axis
		if (frustumPlanes[planeID].x < 0.0f)
			axisVert.x = AABBVector[0].x + vPosition.x; // min x plus tree positions x
		else
			axisVert.x = AABBVector[1].x + vPosition.x; // max x plus tree positions x

																	// y-axis
		//if 
			//frustumPlanes[planeID].y < 0.0f    // Which AABB vertex is furthest down (plane normals direction) the y axis
			axisVert.y = AABBVector[0].y + vPosition.y; // min y plus tree positions y
		//else
			axisVert.y = AABBVector[1].y + vPosition.y; // max y plus tree positions y

																	// z-axis
		//if 
			//frustumPlanes[planeID].z < 0.0f    // Which AABB vertex is furthest down (plane normals direction) the z axis
			axisVert.z = AABBVector[0].z + vPosition.z; // min z plus tree positions z
		//else
			axisVert.z = AABBVector[1].z + vPosition.z; // max z plus tree positions z

																	// Now we get the signed distance from the AABB vertex that's furthest down the frustum planes normal,
																	// and if the signed distance is negative, then the entire bounding box is behind the frustum plane, which means
			
																	// that it should be culled
																	// && XMVectorGetX(XMVector3Dot(planeNormal, XMLoadFloat3(&axisVert2))) + planeConstant < 0.0f
		if (XMVectorGetX(XMVector3Dot(planeNormal, XMLoadFloat3(&axisVert))) + planeConstant < 0.0f)
		{
			bCulled = true;
			// Skip remaining planes to check and move on to next tree
			break;
		}
	}
}

void RedSt4R::Graphics::StaticMesh::Draw()
{
	//frustumPlanes = m_Engine->FPSCamera->GetFrustrumPlanes(m_Engine->GetCameraView(), m_Engine->GetCameraProjection());
	//CheckIfCulled();
	//DrawAABB();

	//if (bLastFrameCulled != bCulled)
	//{
	//	RS_WARNING("--------------CULLED CHANGED!!!!!!!!-----------")
	//}

	if (!bCulled)
	{
		UpdateMeshBuffers();

		TestMaterial->PrepareMaterial();

		m_DeviceContext->RSSetState(m_Engine->GetBackCullingRasterizationState());
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_DeviceContext->IASetInputLayout(m_Engine->GetInputLayoutB());

		m_DeviceContext->DrawIndexed(Indices.size(), 0, 0);
	}
//	bLastFrameCulled = bCulled;
}

void RedSt4R::Graphics::StaticMesh::UpdatePositionWithParent(XMFLOAT3 parentPosition)
{
	XMFLOAT3 desiredPosition = parentPosition;
	parentPosition.x += vPosition.x;
	parentPosition.y += vPosition.y;
	parentPosition.z += vPosition.z;

	this->SetObjectPosition(desiredPosition);
}

void RedSt4R::Graphics::StaticMesh::Begin()
{
	
}


void RedSt4R::Graphics::StaticMesh::Update()
{
	Draw();
}


void RedSt4R::Graphics::StaticMesh::End()
{

}

RedSt4R::Graphics::StaticMesh::~StaticMesh()
{

}