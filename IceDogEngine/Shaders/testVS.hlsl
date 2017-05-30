
struct MCVsIn
{
	float3 pos : POSITION;
	float4 val_f : WEIGHT0;
	float4 val_b : WEIGHT1;
};

MCVsIn main(MCVsIn vin)
{
	return vin;
}