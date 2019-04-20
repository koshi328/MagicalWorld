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

V2P  main(
float4 pos : SV_POSITION,   // 座標
float3 normal : NORMAL,     // 法線
float4 tangent : TANGENT,   // 接線
float4 color : COLOR,       // 色
float2 uv : TEXCOORD0       // テクスチャ座標
)
{
    V2P result;

    float4 lightPos = pos;
    lightPos = mul(lightPos, World);
    lightPos = mul(lightPos, LightViewProjection);
    result.pos = lightPos;


    result.depth = result.pos.zw;
    return result;
}
