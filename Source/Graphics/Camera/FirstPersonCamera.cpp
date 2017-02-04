#include "FirstPersonCamera.h"

using namespace DirectX;

RedSt4R::Graphics::FirstPersonCamera::FirstPersonCamera()
{
	
	SDL_GetGlobalMouseState(&prev_mx, &prev_my);

	mViewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	mProjectionMatrix = XMMatrixPerspectiveFovLH(0.4f*3.14f, 1366.0f / 768.0f, 1.0f, 1000.f);
	
}

void RedSt4R::Graphics::FirstPersonCamera::UpdateInput()
{
	if (state[SDL_SCANCODE_W])
	{
		moveBackForward += 0.5f;
	}
	if (state[SDL_SCANCODE_S])
	{
		moveBackForward -= 0.5f;
	}
	if (state[SDL_SCANCODE_D])
	{
		moveLeftRight += 0.5f;
	}
	if (state[SDL_SCANCODE_A])
	{
		moveLeftRight -= 0.5f;
	}

	SDL_GetGlobalMouseState(&mx, &my);
	
	camYaw += dx * 0.0018f;
	camPitch += dy * 0.0018f;
		
	prev_mx = mx;
	prev_my = my;
}

void RedSt4R::Graphics::FirstPersonCamera::UpdateFPSCamera()
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camPitch);

	camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camUp = XMVector3Cross(camForward, camRight);

	camPosition += moveLeftRight*camRight;
	camPosition += moveBackForward*camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;

	mViewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	mProjectionMatrix = XMMatrixPerspectiveFovLH(0.4f*3.14f, 1000.0f / 600.0f, 1.0f, 1000.f);

}

std::vector<XMFLOAT4> RedSt4R::Graphics::FirstPersonCamera::GetFrustrumPlanes(XMMATRIX ViewMatrix, XMMATRIX ProjectionMatrix)
{
	std::vector<XMFLOAT4> FrustumPlane(6);

	//float zMinimum, r;
	//XMMATRIX matrix;


	// Calculate the minimum Z distance in the frustum.
	//zMinimum = -ProjectionMatrix._43 / ProjectionMatrix._33;
	//r = 1000.0f / (1000.0f - zMinimum);
//	ProjectionMatrix._33 = r;
//	ProjectionMatrix._43 = -r * zMinimum;

	// Create the frustum matrix from the view matrix and updated projection matrix.
	XMMATRIX mviewProj = XMMatrixMultiply(ViewMatrix, ProjectionMatrix);
	XMFLOAT4X4 viewProj;
	XMStoreFloat4x4(&viewProj, mviewProj);

	// Left Frustum Plane
	// Add first column of the matrix to the fourth column
	FrustumPlane[0].x = viewProj._14 + viewProj._11;
	FrustumPlane[0].y = viewProj._24 + viewProj._21;
	FrustumPlane[0].z = viewProj._34 + viewProj._31;
	FrustumPlane[0].w = viewProj._44 + viewProj._41;

	// Right Frustum Plane
	// Subtract first column of matrix from the fourth column
	FrustumPlane[1].x = viewProj._14 - viewProj._11;
	FrustumPlane[1].y = viewProj._24 - viewProj._21;
	FrustumPlane[1].z = viewProj._34 - viewProj._31;
	FrustumPlane[1].w = viewProj._44 - viewProj._41;

	// Top Frustum Plane
	// Subtract second column of matrix from the fourth column
	FrustumPlane[2].x = viewProj._14 - viewProj._12;
	FrustumPlane[2].y = viewProj._24 - viewProj._22;
	FrustumPlane[2].z = viewProj._34 - viewProj._32;
	FrustumPlane[2].w = viewProj._44 - viewProj._42;

	// Bottom Frustum Plane
	// Add second column of the matrix to the fourth column
	FrustumPlane[3].x = viewProj._14 + viewProj._12;
	FrustumPlane[3].y = viewProj._24 + viewProj._22;
	FrustumPlane[3].z = viewProj._34 + viewProj._32;
	FrustumPlane[3].w = viewProj._44 + viewProj._42;

	// Near Frustum Plane
	// We could add the third column to the fourth column to get the near plane,
	// but we don't have to do this because the third column IS the near plane
	FrustumPlane[4].x = viewProj._13;
	FrustumPlane[4].y = viewProj._23;
	FrustumPlane[4].z = viewProj._33;
	FrustumPlane[4].w = viewProj._43;

	// Far Frustum Plane
	// Subtract third column of matrix from the fourth column
	FrustumPlane[5].x = viewProj._14 - viewProj._13;
	FrustumPlane[5].y = viewProj._24 - viewProj._23;
	FrustumPlane[5].z = viewProj._34 - viewProj._33;
	FrustumPlane[5].w = viewProj._44 - viewProj._43;

	// Normalize plane normals (A, B and C (xyz))
	// Also take note that planes face inward
	for (int i = 0; i < 6; ++i)
	{
		float length = sqrt((FrustumPlane[i].x * FrustumPlane[i].x) + (FrustumPlane[i].y * FrustumPlane[i].y) + (FrustumPlane[i].z * FrustumPlane[i].z));
		//FrustumPlane[i].x /= length;
		//FrustumPlane[i].y /= length;
		//FrustumPlane[i].z /= length;
		FrustumPlane[i].w /= length;
	}
	return FrustumPlane;
}

bool RedSt4R::Graphics::FirstPersonCamera::CheckFrustumPoint(float x, float y, float z)
{
	
}

RedSt4R::Graphics::FirstPersonCamera::~FirstPersonCamera()
{

}
