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
P2F main(V2P pixel)
{
    P2F result;

	// ライトの向き
    float3 lightDir = normalize(-LightDirection.xyz);
	// 法線の向き
    float3 normal = normalize(pixel.normal);

	// ライトの強度を求める
    float light = dot(lightDir, normal);

	// 色を求める
    float3 color;

	// ライトの影響を求める（影か、影ではないか）
    float LightAmount = 1.0f;
    {
		// シャドウマップの座標を求める
        float2 shadowTexUV = (pixel.lightPos.xy / pixel.lightPos.w) * 0.5f + 0.5f;
		// Y座標を反転
        shadowTexUV.y = 1.0f - shadowTexUV.y;
		// 深さの係数を求める
        float depth = pixel.lightPos.z / pixel.lightPos.w;

        float SM_Z = depthTexture.Sample(depthSampler, shadowTexUV).x;

        if(depth > SM_Z + 0.003f)
        {
            LightAmount = 0.5f;
        }
    }


	// 影を含めたライトの強度を求める
    light = saturate(light * LightAmount);
	// 色を求める
    color = LightDiffuse.rgb * MaterialDiffuse.rgb * light + LightAmbient.rgb * MaterialAmbient.rgb;
	// 最終的な色の決定
    result.fragment = float4(color, 1.0f);

    return result;
}