#pragma once 

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include "../Shaders/Shader.h"

using namespace DirectX;

struct VertexPP
{
	VertexPP() {}
	VertexPP(float x, float y, float z, float u, float v)
		:Position(x, y, z), texCoord(u, v) {}

	XMFLOAT3 Position;
	XMFLOAT2 texCoord;
};

namespace RedSt4R
{
	class PostProcess
	{
		private:
			ID3D11Device* m_Device;
			ID3D11DeviceContext* m_DeviceContext;

			ID3D11Texture2D* renderedTexture;
			ID3D11RenderTargetView* textureRTV;
			ID3D11ShaderResourceView* textureSRV;
			Graphics::Shader* PostProcessBaseShader;
			ID3D11Buffer* vertexBuffer;
			ID3D11Buffer* indexBuffer;
			ID3D11SamplerState* m__SamplerState;


		public:
			PostProcess();
			~PostProcess();

			void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
			void Render(ID3D11Texture2D *pRenderedTexture, ID3D11RenderTargetView* pTextureRTV, ID3D11ShaderResourceView* pTextureSRV);
	};
}
