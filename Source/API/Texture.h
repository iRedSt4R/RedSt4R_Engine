#pragma once

#include "Interfaces/ITexture.h"
#include "../Core/EngineResources.h"
#include "../Graphics/Material/WICTextureLoader.h"
#include "../Graphics/Material/Material.h"

#define TEXTURE_TYPE_DIFFUSE 0 
#define TEXTURE_TYPE_NORMAL 1 
#define TEXTURE_TYPE_ROUGHNESS 4 
#define TEXTURE_TYPE_METALLIC 5 

namespace RedSt4R
{
	namespace API
	{
		class Texture : ITexture
		{
		public:
			Texture();
			~Texture();

			virtual void LoadTexture(EngineResources* pEngineResources, wchar_t* filePath, int a_textureType) override;
			virtual void Render() override;

		};

	}
}