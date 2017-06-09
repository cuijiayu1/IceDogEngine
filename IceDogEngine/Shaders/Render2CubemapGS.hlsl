
struct LightVSOut
{
	float4 position : POSITION;
};

struct PrefilterCubemapOut
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	uint RTindex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(24)]
void main(point LightVSOut gIn[1],
	inout TriangleStream<PrefilterCubemapOut> triStream)
{
	for (int i = 0; i < 6; ++i)
	{
		PrefilterCubemapOut O0;
		PrefilterCubemapOut O1;
		PrefilterCubemapOut O2;
		PrefilterCubemapOut O3;
		O0.RTindex = i;
		O1.RTindex = i;
		O2.RTindex = i;
		O3.RTindex = i;
		O0.uv = float2(0, 1);   //------->u
		O1.uv = float2(0, 0);	//|
		O2.uv = float2(1, 1);	//|
		O3.uv = float2(1, 0);	//v
		O0.position = float4(-1, -1, 1, 1);		//y
		O1.position = float4(-1, 1, 1, 1);		//|
		O2.position = float4(1, -1, 1, 1);		//|
		O3.position = float4(1, 1, 1, 1);		//----->x
		triStream.Append(O0);
		triStream.Append(O1);
		triStream.Append(O2);
		triStream.Append(O3);
		triStream.RestartStrip();
	}
}