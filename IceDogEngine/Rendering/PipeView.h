#pragma once
#include "../Utils/Common/UtilBasic.h"
#include "PDRenderingDef.h"

namespace IceDogRendering
{
	// pipe view is some kind of camera that offer a view to look at the Rendering pipe
	class PipeView
	{
	public:
		PipeView(float aspectRatio);
		/* get the view matrix */
		float4x4 GetViewMatrix() { return c_viewMatrix; }
		/* get the view inverse matrix */
		float4x4 GetViewInverse() { return c_viewInverse; }
		/* get the eye position in world space */
		float3 GetEyePosition() { return c_eyePosition; }
		/* get the near and the far plane */
		float2 GetNearFarPlane() { return float2(c_nearPlane, c_farPlane); }
		/* set eye position */
		void SetEyePosition(float3);
		/* set eye focus position */
		void SetFocusPosition(float3);
		/* set eye up direction */
		void SetUpDirection(float3);
		/* get the projection matrix */
		float4x4 GetProjectionMatrix() { return c_projMatrix; };
		/* set the aspect ratio */
		void SetAspectRatio(float ratio);
		/* set the FOV angle in degree */
		void SetFOVDegree(float degree);
		/* set the near plane */
		void SetNearPlane(float nearPlane);
		/* set the far plane */
		void SetFarPlane(float farPlane);
		/* use for update the view matrix, called after value change */
		void UpdateViewMatrix();
		void UpdateViewMatrix(float4x4 mat);
		/* use for update the view inverse matrix, called after value change */
		void UpdateViewInverseMatrix();

	private:
		/* use for update the projection matrix, called after value change */
		void UpdateProjectionMatrix();

	private:
		// the view matrix
		float4x4 c_viewMatrix;
		float4x4 c_viewInverse;
		float3 c_eyePosition;
		float3 c_focusPosition;
		float3 c_upDirection;

		// for projection matrix
		float c_aspectRatio;
		float c_fovDegrees;
		float c_nearPlane;
		float c_farPlane;
		float4x4 c_projMatrix;
	};
}

