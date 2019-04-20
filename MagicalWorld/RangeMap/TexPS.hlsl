cbuffer Matrixes : register(b0)
{
    float4x4 WorldViewProjection;
    float4x4 AboveWorldViewProj;
};

struct V2P
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

float4 main(V2P pixel) : SV_TARGET
{
    float4 color = pixel.color;
    color = float4(1, 0, 0, 1);

    return color;
}