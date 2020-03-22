//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
/*
cbuffer cbPerObject : register(b0)
{
    float4 g_vObjectColor : packoffset(c0);
};

cbuffer cbPerFrame : register(b1)
{
    float3 g_vLightDir : packoffset(c0);
    float  g_fAmbient  : packoffset(c0.w);
};
*/

//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------
Texture2D    g_txDiffuse : register(t0);
SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal   : NORMAL;
    float3 vColor    : COLOR;
    float2 vTexcoord : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PSMain(PS_INPUT Input) : SV_TARGET
{
    float4 vDiffuse = g_txDiffuse.Sample(g_samLinear, Input.vTexcoord);
    /*
    Input.vColor.x = Input.vColor.x * 0.75;
    Input.vColor.y = Input.vColor.y * 0.75;
    Input.vColor.z = Input.vColor.z * 0.75;

    vDiffuse.x = (vDiffuse.x + Input.vColor.x) / 2;
    vDiffuse.y = (vDiffuse.z + Input.vColor.y) / 2;
    vDiffuse.z = (vDiffuse.y + Input.vColor.z) / 2;
    */
    // float fLighting = saturate(dot(g_vLightDir, Input.vNormal));
    // fLighting = max(fLighting, g_fAmbient);

    return vDiffuse;
}
