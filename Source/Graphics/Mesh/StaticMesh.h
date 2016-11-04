#pragma once

#include "../Material/Material.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h> 
#include <vector>
#include "../../Core/D3D11Engine/Dx11Engine.h"
#include "../Material/WICTextureLoader.h"
#include "../Material/RS_Material_Desc.h"
#include "../Mesh/StaticMesh.h"
#include "../../GUI/GuiManager.h"

using namespace DirectX;
using namespace RedSt4R;
using namespace Graphics;
using namespace Assimp;

struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
		:Position(x, y, z), texCoord(u, v), Normal(nx, ny, nz) {}

	XMFLOAT3 Position;
	XMFLOAT2 texCoord;
	XMFLOAT3 Normal;
};

struct VertexB
{
	VertexB() {}
	VertexB(float x, float y, float z, float u, float v, float nx, float ny, float nz, float tx, float ty, float tz)
		:Position(x, y, z), texCoord(u, v), Normal(nx, ny, nz), Tangent(tx, ty, tz) {}

	XMFLOAT3 Position;
	XMFLOAT2 texCoord;
	XMFLOAT3 Normal;
	XMFLOAT3 Tangent;
};

struct cbPerObject
{
	XMMATRIX WVP;
	XMMATRIX World;
	XMFLOAT3 CameraPos;
	float padd;
};

class Material;

namespace RedSt4R
{
	namespace Graphics
	{
		class StaticMesh
		{
		private:
			//---------------------Mesh Data-----------------//
			bool bCulled = false;
			bool bLastFrameCulled = false;

			int VertexCount = false;
			int VertexVecSize;
			int FaceCount;
			char* DiffuseTextureDir;
			std::vector <aiVector3D> VertexPosVec;
			std::vector <aiVector3D> TexCoordVec;
			std::vector <aiVector3D> NormalsVec;
			std::vector <aiVector3D> TangentsVec;
			cbPerObject m_MeshConstantBufferObject;
			//--------------------Mesh World----------------//
			XMFLOAT3 vPosition;
			XMFLOAT3 vRotation;
			XMFLOAT3 vScale;
			//------------------Mesh Matrices------------------//
			XMMATRIX mPosition;
			XMMATRIX mRotation;
			XMMATRIX mRotationX;
			XMMATRIX mRotationY;
			XMMATRIX mRotationZ;
			XMMATRIX mScale;
			XMMATRIX mWorld;
			XMMATRIX mWVP;

			//-------------Complete Vertex Vector---------------//
			std::vector <VertexB> VertexVec;
			std::vector <VertexB> UniqueVertexVec;
			std::vector <UINT> Indices;
			std::vector <aiVector3D> AABBVector;
			

			//------------------For Debug Lines----------------//
			std::vector<Vertex> DebugLineVertexVec;
			ID3D11Buffer *m_DebugAABBVertexBuffer;
			std::vector<XMFLOAT4> frustumPlanes;

			//------------------Dx11 Engine-------------------//
			Dx11Engine *m_Engine;
			Material *TestMaterial;
			RS_Material_Desc matDesc;

			//------------------DirectX 11 Stuff--------------//
			ID3D11Device *m_Device;
			ID3D11DeviceContext *m_DeviceContext;
			ID3D11Buffer* m_MeshVertexBuffer;
			ID3D11Buffer* m_IndexBuffer;
			ID3D11Buffer* m_MeshConsantBuffer;
			ID3D11ShaderResourceView* MeshTexture;
			ID3D11Resource *m_TextureResource;
			ID3D11SamplerState* MeshSamplerState;

			GuiManager *GUI;
		

		public:
			StaticMesh(Dx11Engine *Engine, GuiManager *pGUI);
			StaticMesh(char* filePath, Dx11Engine *Engine, wchar_t* DiffuseTextureDir, wchar_t* NormalTextureDir = nullptr);
			~StaticMesh();

			void LoadMeshFromFile(char *filePath);
			void LoadMeshFromFileWithIndex(const aiScene *a_aScene, char *filePath, int index, std::string FolderName);
			void AssignMaterial(Material *a_Material);
			void CreateAABB();
			void Draw();
			void Translate(float x, float y, float z);
			void RotateX(float angle);
			void RotateY(float angle);
			void RotateZ(float angle);
			void Scale(float scaleFactor);

			inline RS_Material_Desc* GetMaterialInfo() { return &matDesc; }

		private:
			void InitDebugAABB();
			void DrawAABB();
			void CheckIfCulled();
			void InitBuffers();
			void UpdateMeshBuffers();

		};
	}
}

