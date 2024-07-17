// Vertex input structure
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

// Vertex output structure
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

// Vertex Shader
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = input.Pos; // Pass position directly to rasterizer
    output.Tex = input.Tex; // Pass texcoord to pixel shader
    return output;
}
