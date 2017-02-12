#pragma once
#include "../../Core/RS_Engine.h"

namespace RedSt4R
{
	class MeshManager
	{
	private:
		std::vector <ComplexMesh*> vComplexMeshes;
		std::vector <StaticMesh> vStaticMeshes;

		Dx11Engine *m_Engine;

	public:
		MeshManager(Dx11Engine *Engine);
		~MeshManager();

		void AddMesh(char* MeshDir, std::string FolderName);
		void Update();
		void RemoveMesh(UINT index);
		void Clear();
		void AddWithOffset(char* MeshDir, float x_offset);

		inline std::vector <ComplexMesh*> GetMeshVector() { return vComplexMeshes; }
	};

}