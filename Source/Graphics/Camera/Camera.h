#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <windows.h>
#include "../../Debug/DebugMacros.h"

using namespace DirectX;

namespace RedSt4R
{
	namespace Graphics
	{
		class Camera
		{
		private:
			ID3D11Device *m_Device;
			ID3D11DeviceContext *m_DeviceContext;
			XMFLOAT3 m_Position;



		public:
			Camera();
			~Camera();


		};
	}
}