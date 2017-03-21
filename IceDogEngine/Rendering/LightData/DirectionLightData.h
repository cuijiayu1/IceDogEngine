#pragma once
#include "LightBase.h"

namespace IceDogRendering
{
	class DirectionLightData : public LightBase
	{
	public:
		DirectionLightData();

		/* set the intensity of this light */
		virtual void SetIntensity(float intensity) override;

		/* update the shadow texture */
		virtual void UpdateShadowTexture(PlatformDependenceRenderResource pdrr) override;

		/* check whether is dirty or not */
		virtual bool IsDirty() override;

		/* clean the buffer */
		virtual void CleanBuffer(PlatformDependenceRenderResource pdrr) override;

	private:
		// no right, this should be pack into the PID
		bool CreateRenderTargetTexture2D(PlatformDependenceRenderResource pdrr,UINT width, UINT height, ID3D11Texture2D*& texture, ID3D11RenderTargetView*& rt, ID3D11ShaderResourceView*& sr, DXGI_FORMAT format);

		// the width / height of the shadow
		float c_shadowWidth;
		float c_shadowHeight;
	};
}

