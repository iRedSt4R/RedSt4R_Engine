#pragma once

#include "../../Core/D3D11Engine/Dx11Engine.h"
#include <d3d11.h>
#include <SDL.h>
#include <vector>
#include "../Shaders/Shader.h"
#include "../Material/DDSTextureLoader.h"
#include "../Material/WICTextureLoader.h"


#pragma comment(lib, "Dxguid.lib")

using namespace DirectX;

namespace RedSt4R
{
	namespace Graphics
	{
		class CubeMap
		{
		private:
			Dx11Engine *m_Engine;
			ID3D11DeviceContext *m_DeviceContext;

			ID3D11Buffer *sphereIndexBuffer;
			ID3D11Buffer *sphereVertBuffer;
			ID3D11Buffer *sphereConstantBuffer;
			Shader *CubeMapShader;
			ID3D11Resource *CubeMapResource;
			ID3D11Texture2D *CubeMapTexture;

			ID3D11ShaderResourceView *m_CubeMapShaderResourceView;
			ID3D11DepthStencilState* DSLessEqual;
			ID3D11RasterizerState* RSCullNone;
			ID3D11SamplerState *m_SamplerState;

			int NumSphereVertices;
			int NumSphereFaces;

			XMMATRIX sphereWorld;
			XMMATRIX Rotationx;
			XMMATRIX Rotationy;
			XMMATRIX Rotationz;
			XMMATRIX Scale;
			XMMATRIX Translation;
			XMMATRIX WVP;

		public:
			CubeMap(Dx11Engine *Engine, wchar_t* CubeMapTextureDir);
			~CubeMap();

			void CreateSphere(int LatLines, int LongLines);
			void UpdateCubeMap();
			void DrawCubeMap();
			void BindCubeMap(int index);

		private:
			void InitCubeMap();

		};
	}
}