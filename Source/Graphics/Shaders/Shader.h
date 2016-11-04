#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include "../../Debug/DebugMacros.h"
namespace RedSt4R
{
	namespace Graphics
	{
		class Shader
		{
		private:
			ID3D11Device *m_Device;
			ID3D11DeviceContext *m_DeviceContext;
			ID3D10Blob *m_VertexShaderByteCode, *m_PixelShaderByteCode;
			ID3D11VertexShader *m_VertexShader;
			ID3D11PixelShader *m_PixelShader;
			ID3D11InputLayout *m_InputLayout;
			ID3D11InputLayout *m_InputLayoutB;
			HRESULT hr;


		public:
			Shader(ID3D11Device *a_Device, ID3D11DeviceContext *a_DeviceContext);
			~Shader();

			void RS_CreateVertexShader(const wchar_t* a_VertexShaderDir);
			void RS_CreatePixelShader(const wchar_t* a_PixelShaderDir);
			void RS_CreateInputLayout();
			void RS_CreateInputLayoutB();

			inline ID3D10Blob* GetVertexShaderByteCode() const { return m_VertexShaderByteCode; }
			inline ID3D10Blob* GetPixelShaderByteCode() const { return m_PixelShaderByteCode; }
			inline ID3D11VertexShader* GetVertexShader() { return m_VertexShader; }
			inline ID3D11PixelShader* GetPixelShader() { return m_PixelShader; }
			inline ID3D11InputLayout* GetInputLayout() const { return m_InputLayout; }
			inline ID3D11InputLayout* GetInputLayoutB() const { return m_InputLayoutB; }

		};
	}
}