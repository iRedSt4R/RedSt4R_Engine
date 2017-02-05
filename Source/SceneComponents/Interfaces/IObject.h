#pragma once
#include <DirectXMath.h>

using namespace DirectX;

namespace RedSt4R
{
	class IObject
	{
	protected:
		//--------------------Mesh World----------------//
		XMFLOAT3 vPosition;
		XMFLOAT3 vRotation;
		XMFLOAT3 vScale;
		//------------------Mesh Matrices------------------//
		XMMATRIX mPosition;
		XMMATRIX mRotation;
		XMMATRIX mRotationX;
		XMMATRIX mRotationY;
		XMMATRIX mRotationZ;
		XMMATRIX mScale;
		XMMATRIX mWorld;
		XMMATRIX mWVP;

	public:
		virtual void Begin() = 0;
		virtual void Render() = 0;
		virtual void End() = 0;

		virtual void BuildMatrices() = 0;

		virtual void SetObjectPosition(XMFLOAT3 position) = 0;
		virtual void SetObjectScale(XMFLOAT3 scale) = 0;
		virtual void RotateX(float angle) = 0;
		virtual void RotateY(float angle) = 0;
		virtual void RotateZ(float angle) = 0;



		//Getters
		virtual XMFLOAT3 GetObjectPosition() = 0;
		virtual float GetObjectPositionX() = 0;
		virtual float GetObjectPositionY() = 0;
		virtual float GetObjectPositionZ() = 0;
		virtual XMFLOAT3 GetObjectRotation() = 0;
		virtual float GetObjectRotationX() = 0;
		virtual float GetObjectRotationY() = 0;
		virtual float GetObjectRotationZ() = 0;
		virtual XMFLOAT3 GetObjectScale() = 0;
		virtual float GetObjectScaleX() = 0;
		virtual float GetObjectScaleY() = 0;
		virtual float GetObjectScaleZ() = 0;
	};
}
