#pragma once

#include "../../Rendering/PDRenderingDef.h"
#include "../../Rendering/RenderData.h"


namespace IceDogResources
{
	namespace IceDogGeometry
	{
		class GeometryGenerator
		{
		public:
			///<summary>
			/// Creates teapot.
			///</summary>
			static void CreateTeapot(int tess,float size, bool rhcoords, std::shared_ptr<IceDogRendering::RenderData> rd);

			///<summary>
			/// Creates a box centered at the origin with the given dimensions.
			///</summary>
			static void CreateBox(float width, float height, float depth, std::shared_ptr<IceDogRendering::RenderData> rd);

			///<summary>
			/// Creates an mxn grid in the xz-plane with m rows and n columns, centered
			/// at the origin with the specified width and depth.
			///</summary>
			static void CreateGrid(float width, float depth, unsigned int m, unsigned int n, std::shared_ptr<IceDogRendering::RenderData> rd);

			///<summary>
			/// Creates an hill grid based on Grid
			/// at the origin with the specified width and depth.
			///</summary>
			static void CreateHill(float width, float depth, unsigned int m, unsigned int n, std::shared_ptr<IceDogRendering::RenderData> rd);

			///<summary>
			/// Creates a geosphere centered at the origin with the given radius.  The
			/// depth controls the level of tessellation.
			///</summary>
			static void CreateGeosphere(float radius, unsigned int numSubdivisions, std::shared_ptr<IceDogRendering::RenderData> rd);

			///<summary>
			/// Creates a sphere centered at the origin with the given radius.  The
			/// slices and stacks parameters control the degree of tessellation.
			///</summary>
			static void CreateSphere(float radius, UINT sliceCount, UINT stackCount, std::shared_ptr<IceDogRendering::RenderData> rd);

		private:
			static void Subdivide(std::shared_ptr<IceDogRendering::RenderData> rd);
		};
	}
}