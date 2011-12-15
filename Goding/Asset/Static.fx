float4x4 World;
float4x4 View;
float4x4 Projection;
float3	 Light;

struct VSI
{
	float4 Position : POSITION0;
	float3 Normal : NORMAL0;
	float2 UV : TEXCOORD0;
	float3 Tangent : TANGENT0;
	float3 BiTangent : BINORMAL0;
};

struct VSO
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD0;
	float3 Normal : TEXCOORD1;
};

VSO VS(VSI input)
{
	VSO output;

	//Transform Position
    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);

	//Transform Normal, yes I know it should be WorldIT but its a technicality and it rarely shows
	output.Normal = normalize(mul(input.Normal, World));

	//Pass UV
	output.UV = input.UV;

	return output;
}

float4 PS(VSO input) : COLOR0
{
	//Light direction
	//float3 L = float3(-0.4, -0.3, 0.85);

	//Just output Diffuse
	return abs(dot(input.Normal, Light));
}

technique Default
{
	pass p0
	{
		VertexShader = compile vs_3_0 VS();
		PixelShader = compile ps_3_0 PS();
	}
}
