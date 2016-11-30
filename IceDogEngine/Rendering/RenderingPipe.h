#pragma once
#include "PlatformDependenceRenderResource.h"
#include "RenderData.h"
#include "PipeView.h"
#include "../Platform/PlatformWindow.h"

namespace IceDogRendering
{
	class RenderingPipe
	{
	public:
		RenderingPipe(std::ostream& errOS) :s_errorlogOutStream(errOS) {}
		/* set msaa config not enable immediately*/
		virtual void SetMsaaConfig(bool msaaEnable) { c_enableMsaa = msaaEnable;};
		/* init the pipe line */
		virtual void InitPipe(IceDogPlatform::PlatformWindow pfWindow) = 0;
		/* render */
		virtual void Render() = 0;
		/* init the platform dependence data */
		virtual void InitPlatformDepedence(PlatformDependenceRenderResource pdrr) { c_PDRR = pdrr; };
		/* regist the render data */
		virtual void RegistRenderData(std::shared_ptr<RenderData> rd)=0;
	protected:
		/* reset the render target size */
		virtual void Resize(int newWidth, int newHeight) = 0;

	protected:
		// the pipe view
		std::shared_ptr<PipeView> r_mainPipeView;
		std::vector < std::shared_ptr<PipeView> > r_pipeViewList;
		// the render datas
		std::vector<std::shared_ptr<RenderData>> r_renderDatas;
		// the platform window that output the render result
		IceDogPlatform::PlatformWindow c_platformWindow;
		// the platform dependence rendering resource
		PlatformDependenceRenderResource c_PDRR;
		// whether enable the multisample anti al
		bool c_enableMsaa;
		// msaa quality
		unsigned int c_msaaQuility;
		// the refresh rate of swap chain
		unsigned int c_swapChainBufferRefreshRate;
		// back buffer width
		unsigned int c_backBufferWidth;
		// back buffer height
		unsigned int c_backBufferHeight;
		// the error out put log port
		std::ostream& s_errorlogOutStream;
	};
}