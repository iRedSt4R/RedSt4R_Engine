#pragma once

#include "../Core/RS_Engine.h"

namespace RedSt4R
{
	class GameMode
	{
	private:

		bool bHaveActiveCamera = false;
		bool bHaveMeshManager = false;
		bool bHaveResourceManager = false;
		bool bHaveGuiManager = false;
		bool bHaveCubeMap = false;
		bool bHaveTimeManager = false;
		bool bIsInEditorMode = false;

		CubeMap *m_ActiveCubeMap;
		FirstPersonCamera *m_ActiveFPSCamera;


	public:
		GameMode();
		~GameMode();

		void InitGameMode();
		void UpdateGameMode();
		void Render();

		void SetFPSCamera(FirstPersonCamera *FPSCamera);
		void SetIsInEditorMode(bool bIfIsInEditorMode);
		void SetMeshManager(MeshManager *MeshManager);


	};
}