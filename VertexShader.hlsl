//--------------------------------------------------------------------------------------
// Globals
//--------------------------------------------------------------------------------------
cbuffer cbPerObject : register(b0)
{
    matrix g_mWorld;
    matrix g_mView;
    matrix g_mProj;
    matrix g_mWorldViewProj;
};

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 vPosition : POSITION;
    float3 vNormal   : NORMAL;
    float3 vColor    : COLOR;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal   : NORMAL;
    float3 vColor    : COLOR;
    float2 vTexcoord : TEXCOORD0;
};

float4 calcPost(float3 inPos)
{
    float4 pos = float4(inPos, 1.0);

    pos = mul(pos, g_mWorld);
    pos = mul(pos, g_mView);
    pos = mul(pos, g_mProj);

    return pos;
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VSMain(VS_INPUT Input)
{
    VS_OUTPUT Output;
    Output.vPosition = calcPost(Input.vPosition);
    Output.vNormal = mul(Input.vNormal, (float3x3)g_mWorld);
    Output.vColor = Input.vColor;
    Output.vTexcoord = Input.vTexcoord;
    return Output;
}
