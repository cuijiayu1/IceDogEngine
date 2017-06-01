
struct LightVSIn
{
	float3 position : POSITION;
};
struct LightVSOut
{
	float4 position : POSITION;
};

LightVSOut main(LightVSIn vin)
{
	LightVSOut a;
	a.position = float4(0.5, 0.5, 0, 1);
	return a;
}