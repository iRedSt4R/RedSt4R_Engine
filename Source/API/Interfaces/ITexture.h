#pragma once

#include <d3d11.h>
#include "../../Core/EngineResources.h"
#include <string>

namespace RedSt4R
{
	namespace API
	{
		class ITexture
		{
		protected:
			ID3D11ShaderResourceView *textureSRV;
			ID3D11SamplerState *filteringState;
			EngineResources* engineResources;
			ID3D11Buffer *m_MaterialConstantBuffer;
			int textureType;

		public:

			virtual void LoadTexture(EngineResources* pEngineResources, wchar_t* filePath, int a_textureType) = 0;
			virtual void Render() = 0;
		};
	}
}