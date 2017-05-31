struct ShadowMapPSOut
{
	float4 depth : SV_TARGET;
};

struct ShadowMapVSOut
{
	float4 positionH : SV_POSITION;
	float2 depth : DEPTHSHADOW;
};

ShadowMapPSOut main(ShadowMapVSOut pin)
{
	ShadowMapPSOut result;
	result.depth = pin.depth.x / pin.depth.y;
	return result;
}