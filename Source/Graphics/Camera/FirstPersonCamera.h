#pragma once

#include <d3d11.h>
#include "../../Debug/DebugMacros.h"
#include <DirectXMath.h>
#include <Windows.h>
#include <SDL.h>
#include <vector>

using namespace DirectX;

namespace RedSt4R
{
	namespace Graphics
	{
		class FirstPersonCamera
		{
		public:
			
		private:
			//RedSt4R::Dx11Engine *m_Engine;
			ID3D11Device *m_Device;
			ID3D11DeviceContext *m_DeviceContext;
			const UINT8 *state = SDL_GetKeyboardState(NULL);
			int mx, my;
			int prev_mx, prev_my;

			//--------------Camera Matrices-----------//
			XMMATRIX mViewMatrix;
			XMMATRIX mProjectionMatrix;
			XMMATRIX mViewProjection;

			XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR camPosition = XMVectorSet(0.0f, 0.0f, -30.0f, 0.0f);
			XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
			XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

			XMMATRIX camRotationMatrix;
			XMMATRIX groundWorld;

			float moveLeftRight = 0.0f;
			float moveBackForward = 0.0f;

			float camYaw = 0.0f;
			float camPitch = 0.0f;


		public:
			float dx, dy;
			FirstPersonCamera();
			~FirstPersonCamera();

			void UpdateInput();
			void UpdateFPSCamera();
			std::vector<XMFLOAT4> GetFrustrumPlanes(XMMATRIX ViewMatrix, XMMATRIX ProjectionMatrix);
			bool CheckFrustumPoint(float x, float y, float z);

			inline XMMATRIX GetCameraView() { return mViewMatrix; }
			inline XMMATRIX GetCameraProjections() { return mProjectionMatrix; }
			inline XMVECTOR GetCameraPosition() { return camPosition; }
			//inline std::vector<XMFLOAT4>& GetFrustumPlanes() { return FrustumPlane; }
		};
	}
}