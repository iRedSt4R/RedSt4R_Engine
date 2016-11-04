#pragma once

#include "../../Core/D3D11Engine/Dx11Engine.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "SDirectionalLight.h"

namespace RedSt4R
{
	namespace Graphics
	{
		class DireLight
		{
		private:
			DirLight m_Light;

			Dx11Engine *m_Engine;
			ID3D11Device *m_Device;
			ID3D11DeviceContext *m_DeviceContext;
			ID3D11Buffer *m_LightConstantBuffer;

			XMFLOAT3 vDirection;
			XMFLOAT4 vAmbient;
			XMFLOAT4 vDiffuse;
			bool bIsActive = false;

		public:
			DireLight(Dx11Engine *Engine, XMFLOAT3 a_vDirection, XMFLOAT4 a_Ambient, XMFLOAT4 a_vDiffuse);
			~DireLight();

			void SetLight();

		private:
			void CreateConstantBuffer();
		};
	}
}