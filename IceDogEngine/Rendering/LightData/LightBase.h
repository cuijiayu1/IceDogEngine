#pragma once

#include "../../Utils/Common/UtilBasic.h"
#include "../PlatformDependenceRenderResource.h"
#include "../PDRenderingDef.h"

namespace IceDogRendering
{
	class LightBase
	{
	public:
		LightBase();
		virtual ~LightBase();

		/* call to close this resource */
		virtual void Close();

		/* set the light definition of this light, for example, direction, intensity, etc.. */
		void SetLightDef(std::shared_ptr<LightDef> lightDef);

		/* return the light def */
		std::shared_ptr<LightDef> GetLightDef();

		/* set the intensity of this light */
		virtual void SetIntensity(float intensity)=0;

		/* update the shadow texture */
		virtual void UpdateShadowTexture(PlatformDependenceRenderResource pdrr) = 0;

		/* check whether the resource is dirty */
		virtual bool IsDirty() = 0;
		
		/* get the shadow map RTV */
		PIDRenderTargetView GetShadowMapRTV();

		/* get the shadow map SRV */
		PIDShaderResourceView GetShadowMapSRV();

		/* get the depth stencil view */
		PIDDepthStencilView GetDepthStencilView();

		/* get the projection matrix */
		IceDogRendering::float4x4 GetProjectionMatrix();

		/* get the view matrix */
		IceDogRendering::float4x4 GetViewMatrix();

		/* get the view port */
		PIDViewport& GetViewport();

		/* get the shadow map size */
		float GetShadowMapSize();

		/* clean the buffer with some color */
		virtual void CleanBuffer(PlatformDependenceRenderResource pdrr) = 0;

	protected:
		// indicate whether the light texture has created or not
		bool c_lightTextureDirty = true;

		// the size of the shadow map
		float c_shadowMapSize = 1024;

		// the projection matrix
		IceDogRendering::float4x4 c_projectionMatrix;

		// the view matrix
		IceDogRendering::float4x4 c_viewMatrix;

		// the view port of the light when render shadow map
		PIDViewport r_viewport;

		// the shadow map texture
		PIDTexture2D r_shadowTexture2d;
		// the shadow map shader resource view
		PIDShaderResourceView r_shadowMapSRV;
		// the shadow map render target view
		PIDRenderTargetView r_shadowMapRTV;
		// the depthstencil texture
		PIDTexture2D r_depthStencilTexture2d;
		// the depthstencil view
		PIDDepthStencilView r_depthStencilView;

		// the light def
		std::shared_ptr<LightDef> r_lightDef;
	};
}

