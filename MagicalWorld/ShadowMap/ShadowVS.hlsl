// テクスチャ
Texture2D texture1 : register(t0); // ディフューズテクスチャ
Texture2D depthTexture : register(t1); // シャドウマップ（深度マップ）
SamplerState textureSampler : register(s0); // ディフューズテクスチャ用サンプラ
SamplerState depthSampler : register(s1); // シャドウマップのサンプラ

// 行列
cbuffer Matrixes : register(b0)
{
    float4x4 WorldViewProjection; // World*View*Projection
    float4x4 World; // World
    float4x4 LightViewProjection; // ライトのView*Projection
};

// ライト
cbuffer Lights : register(b1)
{
    float4 LightDirection; // ライトの向き
    float4 LightDiffuse; // ライトのディフューズ
    float4 LightAmbient; // ライトのアンビエント
};

// マテリアル
cbuffer Materials : register(b2)
{
    float4 MaterialDiffuse; // マテリアルのディフューズ
    float4 MaterialAmbient; // マテリアルのアンビエント
};

// ピクセルシェーダに送る情報
struct V2P
{
    float4 pos : SV_POSITION; // 座標
    float3 normal : NORMAL; // 法線
    float4 color : COLOR; // 色
    float2 uv : TEXCOORD0; // テクスチャの座標
    float4 lightPos : LIGHTPOS; // ライト空間プロジェクション座標
};

// 最終結果
struct P2F
{
    float4 fragment : SV_TARGET; // 色
};

// メイン関数
V2P main(
	float4 pos : SV_POSITION, // 座標
	float3 normal : NORMAL, // 法線
	float4 tangent : TANGENT, // 接線
	float4 color : COLOR, // 色
	float2 uv : TEXCOORD0 // テクスチャ座標
	)
{
    V2P result;

	// 座標変換
    result.pos = mul(pos, WorldViewProjection);
	// ワールド法線
    result.normal = mul(normal, (float3x3) World);
	// 色
    result.color = color;
	// テクスチャ座標
    result.uv = uv;

	// ライト空間プロジェクション座標
    float4 lightPos = pos;
    lightPos = mul(lightPos, World);
    lightPos = mul(lightPos, LightViewProjection);
    result.lightPos = lightPos;

    return result;
}