// テクスチャ
Texture2D texture1 : register(t0);
Texture2D rangeTexture : register(t1);
SamplerState textureSampler : register(s0);
SamplerState rangeSampler : register(s1);

cbuffer Matrixes : register(b0)
{
    float4x4 WorldViewProjection;
    float4x4 AboveWorldViewProj;
};

struct V2P
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float4 lightPos : LIGHTPOS;
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

    	// ライト空間プロジェクション座標
    float4 lightPos = pos;
    lightPos = mul(lightPos, AboveWorldViewProj);
    result.lightPos = lightPos;

    return result; // ピクセルシェーダへ結果を送る
}