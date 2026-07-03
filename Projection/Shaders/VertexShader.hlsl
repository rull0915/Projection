#include "Header.hlsli"

PSInput main(VSInput input)
{
	PSInput output = (PSInput) 0;
	
	output.pos = input.pos;
	output.pos = mul(output.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	
	output.normal = input.normal;
	
	return output;
}
