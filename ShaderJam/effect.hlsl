// Constant buffer for passing time

// Pixel Shader Input Structure
struct PS_INPUT
{
    float4 Pos : SV_POSITION; // Position in clip space
};

// Pixel Shader
float4 main(PS_INPUT input) : SV_Target
{
    // Create a moving gradient based on time and texture coordinates
    float2 color = 0.5 + 0.5 * cos(input.Pos.xy * 0.01);
    return float4(color, 1.0, 1.0);
}
