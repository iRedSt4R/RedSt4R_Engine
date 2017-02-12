#include "EngineResources.h"

RedSt4R::EngineResources::EngineResources()
{

}

RedSt4R::EngineResources::~EngineResources()
{

}

void RedSt4R::EngineResources::SetDevice(ID3D11Device* pDevice)
{
	device = pDevice;
}

void RedSt4R::EngineResources::SetDeviceContext(ID3D11DeviceContext* pDeviceContext)
{
	deviceContext = pDeviceContext;
}