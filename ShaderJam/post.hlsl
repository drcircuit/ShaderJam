// Assume these register bindings match your setup in C++ code
Texture2D currentFrame : register(t0); // Texture output from effect shader

SamplerState samplerState : register(s0); // Sampler for texture sampling

// Constant buffer for passing time
cbuffer TimeBuffer : register(b0)
{
    float Time; // Time value that updates each frame
    float3 padding; // Padding for correct alignment
}
// Input structure from vertex shader
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0; // Texture coordinates passed from vertex shader
    float2 IResolution : IRESOLUTION; // Screen resolution
};

// Pixel shader main function
float4 main(PixelInputType input) : SV_TARGET
{
    // Normalize texture coordinates to [0, 1] range
    float2 uv = input.tex / input.IResolution;
    // Simply sample the texture at the uv coordinates
    
    float4 color = currentFrame.Sample(samplerState, uv);
    //color += sin(uv.x * 100. + Time) * 0.1;
    // simple vignette
    float2 center = float2(0.5, 0.5);
    float vignette = 1.0 - length(uv - center) * 1.5;
    vignette = pow(vignette, 0.9) * (0.5 + 0.5 * sin(Time*10.));
    color.rgb -= 1.0-vignette;
    
    return color;
}
