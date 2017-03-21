#include "PipeView.h"

using namespace IceDogRendering;

void PipeView::UpdateViewMatrix()
{
#if defined __DIRECTX__
	XMFLOAT4X4 tempMatrix;
	XMMATRIX xmat = XMMatrixLookAtLH(XMLoadFloat3(&XMFLOAT3(c_eyePosition.x, c_eyePosition.y, c_eyePosition.z)), XMLoadFloat3(&XMFLOAT3(c_focusPosition.x, c_focusPosition.y, c_focusPosition.z)), XMLoadFloat3(&XMFLOAT3(c_upDirection.x, c_upDirection.y, c_upDirection.z)));
	XMStoreFloat4x4(&tempMatrix, xmat);
	c_viewMatrix = float4x4(tempMatrix);
#endif
	UpdateViewInverseMatrix();
}

void IceDogRendering::PipeView::UpdateViewMatrix(float4x4 mat)
{
	c_viewMatrix = mat;
	UpdateViewInverseMatrix();
}

void IceDogRendering::PipeView::UpdateViewInverseMatrix()
{
#if defined __DIRECTX__
	XMMATRIX xmat = XMMatrixLookAtLH(XMLoadFloat3(&XMFLOAT3(c_eyePosition.x, c_eyePosition.y, c_eyePosition.z)), XMLoadFloat3(&XMFLOAT3(c_focusPosition.x, c_focusPosition.y, c_focusPosition.z)), XMLoadFloat3(&XMFLOAT3(c_upDirection.x, c_upDirection.y, c_upDirection.z)));
	XMFLOAT4X4 tempMatrix;
	XMStoreFloat4x4(&tempMatrix, xmat);
	XMStoreFloat4x4(&tempMatrix, XMMatrixInverse(&XMMatrixDeterminant(xmat), xmat));
	c_viewInverse = float4x4(tempMatrix);
#endif
}

void PipeView::UpdateProjectionMatrix()
{
#if defined __DIRECTX__
	XMFLOAT4X4 tempMatrix;
	XMStoreFloat4x4(&tempMatrix, XMMatrixPerspectiveFovLH(XM_PI*c_fovDegrees / 360.0, c_aspectRatio, c_nearPlane, c_farPlane));
	c_projMatrix = float4x4(tempMatrix);
#endif
}

PipeView::PipeView(float aspectRatio) :c_eyePosition(0, 0, 0.001),
										c_focusPosition(0, 0, 0),
										c_upDirection(0, 1, 0),
										c_aspectRatio(aspectRatio),
										c_fovDegrees(90),
										c_nearPlane(DEFAULT_NEARZ),
										c_farPlane(DEFAULT_FARZ)
{
	// build up the view matrix
	UpdateViewMatrix();
	// build up the projection matrix
	UpdateProjectionMatrix();
}

void PipeView::SetEyePosition(float3 pos)
{
	c_eyePosition = pos;
}

void PipeView::SetFocusPosition(float3 focusPos)
{
	c_focusPosition = focusPos;
}

void PipeView::SetUpDirection(float3 upDirection)
{
	c_upDirection = upDirection;
}

void PipeView::SetAspectRatio(float ratio)
{
	c_aspectRatio = ratio;
	UpdateProjectionMatrix();
}
void PipeView::SetFOVDegree(float degree)
{
	c_fovDegrees = degree;
	UpdateProjectionMatrix();
}
void PipeView::SetNearPlane(float nearPlane)
{
	c_nearPlane = nearPlane;
	UpdateProjectionMatrix();
}
void PipeView::SetFarPlane(float farPlane)
{
	c_farPlane = farPlane;
	UpdateProjectionMatrix();
}