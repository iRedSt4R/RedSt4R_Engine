#pragma once

#include "../../Core/D3D11Engine/Dx11Engine.h"
#include "../../GUI/GuiManager.h"
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
		class ComplexMesh
		{
		private:
			Dx11Engine *m_Engine;
			ID3D11Device *m_Device;
			ID3D11DeviceContext *m_DeviceContext;
			const UINT8 *state = SDL_GetKeyboardState(NULL);

			std::vector <StaticMesh*> vMeshes;
			std::vector <Material*> vMaterials;
			Material *TestMaterial;
			int m_NumberMeshes;
			int m_CurrentMeshIndex;

			int FaceCount;
			int VertexCount;
			int TotalVertices = 0;

			GuiManager *GUI;


		public:
			ComplexMesh(Dx11Engine *Engine, char* MeshDir, std::string FolderName, GuiManager *pGUI);
			~ComplexMesh();

			void LoadMeshFromFile(char* MeshDir, std::string FolderName);
			void Update();
			void Draw();
			void Scale(float scaleFactor);
			void RotateX(float angle);
			void RotateY(float angle);
			void RotateZ(float angle);
		};

	}
}