struct DirectionalLight
{
	float4 direction;
	float3 diffuse;
	float intensity;
};

struct PointLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float range;
	float3 att;
	float pad;
};

struct SpotLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float range;
	float3 direction;
	float spot;
	float3 att;
	float pad;
};