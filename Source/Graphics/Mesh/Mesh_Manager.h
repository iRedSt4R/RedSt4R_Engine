#pragma once
#include "../../Core/RS_Engine.h"
#include "../../GUI/GuiManager.h"

namespace RedSt4R
{
	class MeshManager
	{
	private:
		std::vector <ComplexMesh*> vComplexMeshes;
		std::vector <StaticMesh> vStaticMeshes;

		GuiManager *GUI;
		Dx11Engine *m_Engine;

	public:
		MeshManager(Dx11Engine *Engine, GuiManager *pGUI);
		~MeshManager();

		void AddMesh(char* MeshDir, std::string FolderName);
		void Update();
		void RemoveMesh(UINT index);
		void Clear();
		void AddWithOffset(char* MeshDir, float x_offset);

		//For Gui
		//inline GetMeshPositionX(int meshIndex) { return vComplexMeshes[meshIndex]-> }

		inline std::vector <ComplexMesh*> GetMeshVector() { return vComplexMeshes; }
	};

}