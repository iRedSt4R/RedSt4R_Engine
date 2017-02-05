#include "Object.h"

DirectX::XMFLOAT3 RedSt4R::Object::GetObjectPosition()
{
	return vPosition;
}

float RedSt4R::Object::GetObjectPositionX()
{
	return vPosition.x;
}

float RedSt4R::Object::GetObjectPositionY()
{
	return vPosition.y;
}

float RedSt4R::Object::GetObjectPositionZ()
{
	return vPosition.z;
}

DirectX::XMFLOAT3 RedSt4R::Object::GetObjectRotation()
{
	return vRotation;
}

float RedSt4R::Object::GetObjectRotationX()
{
	return vRotation.x;
}

float RedSt4R::Object::GetObjectRotationY()
{
	return vRotation.y;
}

float RedSt4R::Object::GetObjectRotationZ()
{
	return vRotation.z;
}

DirectX::XMFLOAT3 RedSt4R::Object::GetObjectScale()
{
	return vScale;
}

float RedSt4R::Object::GetObjectScaleX()
{
	return vScale.x;
}

float RedSt4R::Object::GetObjectScaleY()
{
	return vScale.y;
}

float RedSt4R::Object::GetObjectScaleZ()
{
	return vScale.z;
}

void RedSt4R::Object::Begin()
{
	
}

void RedSt4R::Object::Render()
{
	
}

void RedSt4R::Object::End()
{
	
}

void RedSt4R::Object::BuildMatrices()
{
	mPosition = XMMatrixTranslation(vPosition.x, vPosition.y, vPosition.z);
	mRotationX = XMMatrixRotationX(vRotation.x);
	mRotationY = XMMatrixRotationY(vRotation.y);
	mRotationZ = XMMatrixRotationZ(vRotation.z);

	mRotation = mRotationX * mRotationY * mRotationZ;

	mScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);

	mWorld = XMMatrixIdentity();
	mWorld = mPosition * mRotation * mScale;
}

void RedSt4R::Object::SetObjectPosition(XMFLOAT3 position)
{
	vPosition = position;
}

void RedSt4R::Object::SetObjectScale(XMFLOAT3 scale)
{
	vScale = scale;
}

void RedSt4R::Object::RotateX(float angle)
{
	vRotation.x += angle;
}

void RedSt4R::Object::RotateY(float angle)
{
	vRotation.y += angle;
}

void RedSt4R::Object::RotateZ(float angle)
{
	vRotation.z += angle;
}
