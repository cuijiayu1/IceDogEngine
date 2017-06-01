
struct LightVSOut
{
	float4 position : POSITION;
};

struct LightGSOut
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

[maxvertexcount(4)]
void main(point LightVSOut gIn[1], inout TriangleStream<LightGSOut> triStream)
{
	LightGSOut O0;
	LightGSOut O1;
	LightGSOut O2;
	LightGSOut O3;
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
}