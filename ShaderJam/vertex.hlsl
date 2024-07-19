// Vertex input structure
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
    float2 IResolution : IRESOLUTION;
};

// Vertex output structure
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float2 IResolution : IRESOLUTION;
};

// Vertex Shader
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = input.Pos; // Pass position directly to rasterizer
    output.Tex = input.Tex; // Pass texture coordinates directly to pixel shader
    output.IResolution = input.IResolution; // Pass resolution directly to pixel shader
    return output;
}
