// Vertex input structure
struct VS_INPUT
{
    float4 Pos : POSITION;
};

// Vertex output structure
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};

// Vertex Shader
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = input.Pos; // Pass position directly to rasterizer
    return output;
}
