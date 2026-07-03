#include "Header.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	float3 normal = input.normal;
	float3 light = lightDir;
	normal = normalize(normal);
	light = normalize(light);
	
	float d = dot(normal, light);
	
	return float4(d, d, d, 1.0f);
}
