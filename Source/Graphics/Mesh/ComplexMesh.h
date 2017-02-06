#pragma once

#include "../../Core/D3D11Engine/Dx11Engine.h"
#include "../../GUI/GuiManager.h"
#include "../../SceneComponents/Interfaces/IObject.h"
#include "StaticMesh.h"
#include <DirectXMath.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h> 

namespace RedSt4R
{
	namespace Graphics
	{
		class ComplexMesh : public Object
		{
		private:
			Dx11Engine *m_Engine;
			ID3D11Device *m_Device;
			ID3D11DeviceContext *m_DeviceContext;
			const UINT8 *state = SDL_GetKeyboardState(NULL);

		public:
			std::vector <StaticMesh*> vMeshes;
			std::vector <Material*> vMaterials;


		private:

			int m_NumberMeshes;
			int m_CurrentMeshIndex;

			int FaceCount;
			int VertexCount;
			int TotalVertices = 0;

		public:
			ComplexMesh(Dx11Engine *Engine, char* MeshDir, std::string FolderName);
			~ComplexMesh();

			void LoadMeshFromFile(char* MeshDir, std::string FolderName);
			
			virtual void Begin() override;


			virtual void Update() override;


			virtual void End() override;


			virtual void SetObjectPosition(XMFLOAT3 position) override;


			virtual void SetObjectScale(XMFLOAT3 scale) override;


			virtual void Scale(float scaleFactor) override;


			virtual void RotateX(float angle) override;


			virtual void RotateY(float angle) override;


			virtual void RotateZ(float angle) override;


			virtual void Translate(float x, float y, float z) override;

		private:
			void LoadRSModelFile(char* filePath, RSMODEL_DESC* a_ModelDesc);
			void TestRotation();

		};

	}
}