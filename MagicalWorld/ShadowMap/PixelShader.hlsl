cbuffer Matrixes : register(b0)
{
    float4x4 WorldViewProjection;
    float4x4 World;
    float4x4 LightViewProjection; // ライトのView*Projection
};

// ライト
cbuffer Lights : register(b1)
{
    float4 LightDirection; // ライトの向き
    float4 LightDiffuse; // ライトのディフューズ
    float4 LightAmbient; // ライトのアンビエント
};

cbuffer Materials : register(b2)
{
    float4 MaterialDiffuse;
    float4 MaterialAmbient;
};

struct V2P
{
    float4 pos : SV_POSITION;
    float2 depth : DEPTH;
};

struct P2F
{
    float4 fragment : SV_Target;
};


P2F main(V2P pixel)
{
    P2F result;
    float d = pixel.depth.x / pixel.depth.y;

    result.fragment = float4(d, d * d, 0, 1);
    result.fragment = float4(1, 1, 1, 1);
    return result;
}