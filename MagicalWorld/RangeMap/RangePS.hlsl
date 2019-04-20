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

float4 main(V2P pixel) : SV_TARGET
{
	// シャドウマップの座標を求める
    float2 shadowTexUV = (pixel.lightPos.xy / pixel.lightPos.w) * 0.5f + 0.5f;
	// Y座標を反転
    shadowTexUV.y = 1.0f - shadowTexUV.y;
    float4 color = rangeTexture.Sample(rangeSampler, shadowTexUV);
    color *= color;
	// ライトの向き
    float3 lightDir = normalize(float3(1, 1, 0));
	// 法線の向き
    float3 normal = normalize(pixel.normal);
	// ライトの強度を求める
    float light = dot(lightDir, normal);

	// ハーフランバートを求める
	light = light * 0.5f + 0.5f;
	light = light * light;

    color *= light;
    color += float4(0.f, 0.2f, 0.2f,0.f);

    return color;
}