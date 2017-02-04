#pragma once

#include <d3d11.h>
#include <iostream>
#include "../Mesh/StaticMesh.h"
#include "../../Core/D3D11Engine/Dx11Engine.h"
#include "../../Debug/DebugMacros.h"
#include "RS_Material_Desc.h"
#include "../Shaders/Shader.h"
#include "WICTextureLoader.h"
#include "../../API/Texture.h"

struct cbMaterial
{
	float bHaveDiffuseTexture;
	float bHaveNormalMap;
	float bHaveGlossMap;
	float bHaveRougnessMap;
	float bHaveMetallicMap;
	float mRougness;
	float mGlossiness;
	float p0;
};

//Forward declaration to avoid errors
namespace RedSt4R{namespace API{class Texture; } }

namespace RedSt4R
{
	namespace Graphics
	{
		class Material
		{
		private:
			Dx11Engine *m_Engine;
			ID3D11Device *m_Device;
			ID3D11DeviceContext *m_DeviceContext;
			ID3D11Buffer *m_MaterialConstantBuffer;

			bool bIsCreated = false;
			cbMaterial cbMaterialObject;

			//-------------Material Values-------------//
			bool bHaveDiffuseTexture = false;
			bool bHaveNormalTexture = false;
			bool bHaveRougnessTexture = false;
			bool bHaveMetallicMap = false;

			float m_Albedo;
			float m_Roughness;
			float m_Glossiness;
			//----------Material Textures-------------//
			//ID3D11ShaderResourceView *DiffuseTextureRV;
			RedSt4R::API::Texture* DiffuseTexture;
			RedSt4R::API::Texture* NormalTexture;
			RedSt4R::API::Texture* RougnessTexture;
			RedSt4R::API::Texture* MetallicTexture;

			//ID3D11ShaderResourceView *NormalTextureRV;
			//ID3D11ShaderResourceView *RougnessTextureRV;
			//ID3D11ShaderResourceView *MetallicTextureRV;

			ID3D11SamplerState *FilteringState;


		public:
			Material(Dx11Engine *Engine);
			~Material();

			void CreateMaterial(RS_Material_Desc &a_materialDesc);
			void PrepareMaterial();
			void ClearMaterial();
			void SetAnistopicFiltering();
			void SetRoughness(float value);
			void SetGlossiness(float value);
		private:
			void CreateBuffers();
		};
	}
}
