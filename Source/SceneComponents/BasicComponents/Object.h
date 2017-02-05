#pragma once

#include "../Interfaces/IObject.h"

namespace RedSt4R
{
	class Object : IObject
	{

	public:
		virtual XMFLOAT3 GetObjectPosition() override;
		virtual float GetObjectPositionX() override;
		virtual float GetObjectPositionY() override;
		virtual float GetObjectPositionZ() override;
		virtual XMFLOAT3 GetObjectRotation() override;
		virtual float GetObjectRotationX() override;
		virtual float GetObjectRotationY() override;
		virtual float GetObjectRotationZ() override;
		virtual XMFLOAT3 GetObjectScale() override;
		virtual float GetObjectScaleX() override;
		virtual float GetObjectScaleY() override;
		virtual float GetObjectScaleZ() override;

	};
}