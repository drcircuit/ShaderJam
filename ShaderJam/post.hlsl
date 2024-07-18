float4 main(float2 uv : TEXCOORD0) : SV_Target
{
    float4 color = float4(uv, 0, 1);
    return color;
}