#pragma once

#include <d3d11.h>
#include <string>

namespace RedSt4R
{
	class EngineResources 
	{
	private:
		ID3D11Device* device;
		ID3D11DeviceContext * deviceContext;

	public:
		EngineResources();
		~EngineResources();

		void SetDevice(ID3D11Device* pDevice);
		void SetDeviceContext(ID3D11DeviceContext* pDeviceContext);

		inline ID3D11Device* GetDevice() { if(device != nullptr) return device; }
		inline ID3D11DeviceContext* GetDeviceContext() { if (deviceContext != nullptr) return deviceContext; }
	};
	
}