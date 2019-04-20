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

V2P main(
float4 pos : SV_POSITION, // 座標
float3 normal : NORMAL, // 法線
float4 tangent : TANGENT, // 接線
float4 color : COLOR, // 色
float2 uv : TEXCOORD0 // テクスチャ座標
)
{
    V2P result; // 結果を代入する変数
    result.pos = mul(pos, WorldViewProjection);
    result.normal = normal;
    result.uv = uv;
    result.color = color;

    return result; // ピクセルシェーダへ結果を送る
}