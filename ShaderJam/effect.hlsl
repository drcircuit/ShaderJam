// Constant buffer for passing time
cbuffer TimeBuffer : register(b0)
{
    float Time; // Time value that updates each frame
    float3 padding; // Padding for correct alignment
}
// Pixel Shader Input Structure
struct PS_INPUT
{
    float4 Pos : SV_POSITION; // Position in clip space
    float2 Tex : TEXCOORD0;   // Texture coordinates
    float2 IResolution : IRESOLUTION; // Screen resolution
};
struct myvars
{

    float4 litID;
    float svLitID;
    float2 v2Rnd;
    float2 svV2Rnd;
    float gLitID;
};
#define FAR 50.0


float hash21(float2 p)
{
    float n = dot(p, float2(7.163, 157.247));
    return frac(sin(n) * 43758.5453);
}
float hash31(float3 p)
{
    float n = dot(p, float3(13.163, 157.247, 7.951));
    return frac(sin(n) * 43758.5453);
}

float smax(float a, float b, float k)
{
    float f = max(0.0, 1.0 - abs(b - a) / k);
    return max(a, b) - f * f * k * 0.25;
}

float noise3d(in float3 p)
{
    const float3 s = float3(113, 157, 1);
    float3 id = floor(p);
    float4 h = float4(0, s.yz, s.y + s.z) + dot(id, s);
    p -= id;
    p = p * p*(3 - 2 * p);
    h = lerp(frac(sin(h)*43758.5453), frac(sin(h + s.x) * 43758.5453), p.x);
    h.xy = lerp(h.xz, h.yw, p.y);
    float n = lerp(h.x, h.y, p.z);
    return n;
}

float fbm(in float3 p)
{
    return 0.5333 * noise3d(p) + 0.2666 * noise3d(p*2.02) + 0.1333 *noise3d(p*4.04) + 0.0666 * noise3d(p*8.08);
}

float2 path(in float z)
{
    return float2(sin(z*.15)*2.4,0);
}

const float2 s = float2(.866025, 1);

float hexSdf(float2 p)
{
    p = abs(p);
    return max(dot(p, s), p.y);
}

float hexPylonSdf(float2 p2, float pz, float r, float ht)
{
    float3 p = float3(p2.x, pz, p2.y);
    float3 b = float3(r, ht, r);
    p.xz = abs(p.xz);
    p2 = p.xz * 0.8660254 + p.xz * .5;
    p.xz = float2(max(p2.x, p2.y), max(p.z, p.x));
    return length(max(abs(p)-b+.015,0)) - .015;
}



float objDist(float2 p, float pH, float r, float ht, inout float id, float dir)
{
    const float s = 1. / 16.;

    float h1 = hexPylonSdf(p, pH, r, ht);
    h1 = max(h1, -hexPylonSdf(p, pH + ht, r - .06, s / 4.));
    float h2 = hexPylonSdf(p, pH + ht - s, r + .01, s / 3.);
    
    id = h1 < h2 ? 0. : 1.0;
    return min(h1, h2);
}

float hexHeight(float2 p)
{
    return dot(sin(p * 2. - cos(p.yx * 1.4)), float2(.25, .25)) + .5;
}

float4 hexGridCell(float2 p, float pH, inout myvars globals)
{
    float4 hC = floor(float4(p, p - float2(0, .5)) / s.xyxy) + float4(0, 0, 0, .5);
    float4 hC2 = floor(float4(p - float2(.5, .25), p - float2(.5, .75)) / s.xyxy) + float4(.5, .25, .5, .75);
    
    float4 h = float4(p - (hC.xy+.5)*s, p-(hC.zw + .5)*s);
    float4 h2 = float4(p - (hC2.xy + .5)*s, p - (hC2.zw + .5)*s);
    
    float4 ht = float4(hexHeight(hC.xy), hexHeight(hC.zw), hexHeight(hC2.xy), hexHeight(hC2.zw));
    ht = floor(ht * 4.99) / 4. / 2. + .02;
    const float r = .25;
    float4 obj = float4(objDist(h.xy, pH, r, ht.x, globals.litID.x, 1.),
                        objDist(h.zw, pH, r, ht.y, globals.litID.y, 1.),
                        objDist(h2.xy, pH, r, ht.z, globals.litID.z, -1.),
                        objDist(h2.zw, pH, r, ht.w, globals.litID.w, -1.));
    h = obj.x<obj.y ? float4(h.xy, hC.xy) : float4(h.zw, hC.zw);
    h2 = obj.z<obj.w ? float4(h2.xy, hC2.xy) : float4(h2.zw, hC2.zw);
    
    float2 oh = obj.x<obj.y ? float2(obj.x, globals.litID.x) : float2(obj.y, globals.litID.y);
    float2 oh2 = obj.z<obj.w ? float2(obj.z, globals.litID.z) : float2(obj.w, globals.litID.w);
    return oh.x < oh2.x ? float4(oh, h.zw) : float4(oh2, h2.zw);
}



float heightMap(in float3 p, inout myvars globals)
{
    const float sc = 1.;
    float4 h = hexGridCell(p.xz * sc, -p.y * sc, globals);
    globals.v2Rnd = h.zw;
    globals.gLitID = h.y;
    return h.x / sc;
}

float map(float3 p, inout myvars globals)
{
    float c = heightMap(p, globals);
    return c * .7;
}

float3 glow;

float getRndID(float2 p)
{
    return hash21(p) - .75;

}

float march(float3 ro, float3 rd)
{
    return 0;
}
// Pixel Shader
float4 main(PS_INPUT input) : SV_Target
{
    myvars globals = {float4(0,0,0,0), 0, float2(0,0), float2(0,0), 0};
    
    // Create a moving gradient based on time and texture coordinates
    float2 uv = input.Tex / input.IResolution;
    float3 col = 0.5 + 0.5 * cos(Time + uv.xyx + float3(0, 2, 4));
    return float4(col, 1);
}
