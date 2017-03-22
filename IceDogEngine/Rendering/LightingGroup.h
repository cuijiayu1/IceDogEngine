#pragma once

#include "../Utils/Common/UtilBasic.h"
#include "LightData/DirectionLightData.h"
#include "PDRenderingDef.h"

// the lighting group is a group that hold 3 kinds of light and each type of light has at least one.
// one lighting group would be used in once (pre obj drawing). but the light more than a lighting group could hold would
// cause much more drawing call.
// ie: if we have just one active lighting group,the draw call would be: for each obj draw.
// however if we have more than one lighting group, the draw call would be for each lighting group for each obj draw.
// the best performance would be just hold one active lighting group.
// this class only used in forward rendering

namespace IceDogRendering
{

	template<int perLightCount = 1>
	class LightingGroup
	{
	public:
		LightingGroup()
		{
			for (size_t i = 0; i < perLightCount; i++)
			{
				c_holdDirectionalLights[i] = false;
				c_holdPointLights[i] = false;
				c_holdSpotLights[i] = false;
			}
		}

		// return how many light can this group hold per type
		constexpr int GetPerLightCount()
		{
			return perLightCount;
		}

		// return the directional light that this group hold
		std::shared_ptr<IceDogRendering::DirectionLightData>* GetDirectionalLight()
		{
			return r_directionalLights;
		}

		// return the spot light that this group hold
		IceDogRendering::SpotLight* GetSpotLight()
		{
			return r_SpotLights;
		}

		// return the point light that this group hold
		IceDogRendering::PointLight* GetPointLight()
		{
			return r_PointLights;
		}

		int HasDirectionalLightsSpace()
		{
			int totSpace = perLightCount;
			for (size_t i = 0; i < perLightCount; i++)
			{
				if (c_holdDirectionalLights[i] == true) totSpace--;
			}
			return totSpace;
		}
		int HasSpotLightsSpace()
		{
			int totSpace = perLightCount;
			for (size_t i = 0; i < perLightCount; i++)
			{
				if (c_holdSpotLights[i] == true) totSpace--;
			}
			return totSpace;
		}
		int HasPointLightsSpace()
		{
			int totSpace = perLightCount;
			for (size_t i = 0; i < perLightCount; i++)
			{
				if (c_holdPointLights[i] == true) totSpace--;
			}
			return totSpace;
		}

		// add a direction light to group
		bool AddDirectionalLight(std::shared_ptr<IceDogRendering::DirectionLightData> dl)
		{
			int place = HasDirectionalLightsSpace();
			if (place == 0) { return false; }
			r_directionalLights[perLightCount - place] = dl;
			c_holdDirectionalLights[perLightCount - place] = true;
			return true;
		}

		// add a spot light to group
		bool AddSpotLight(IceDogRendering::SpotLight& sl)
		{
			int place = HasSpotLightsSpace();
			if (place == 0) { return false; }
			r_SpotLights[perLightCount - place] = sl;
			c_holdSpotLights[perLightCount - place] = true;
			return true;
		}

		// add a point light to group
		bool AddPointLight(IceDogRendering::PointLight& pl)
		{
			int place = HasSpotLightsSpace();
			if (place == 0) { return false; }
			r_PointLights[perLightCount - place] = pl;
			c_holdPointLights[perLightCount - place] = true;
			return true;
		}

	private:
		// indicate which one is used
		bool c_holdDirectionalLights[perLightCount];
		bool c_holdSpotLights[perLightCount];
		bool c_holdPointLights[perLightCount];

		// hold the lights
		std::shared_ptr<IceDogRendering::DirectionLightData> r_directionalLights[perLightCount];
		IceDogRendering::SpotLight r_SpotLights[perLightCount];
		IceDogRendering::PointLight r_PointLights[perLightCount];
	};

}