// �e�N�X�`��
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
	// �V���h�E�}�b�v�̍��W�����߂�
    float2 shadowTexUV = (pixel.lightPos.xy / pixel.lightPos.w) * 0.5f + 0.5f;
	// Y���W�𔽓]
    shadowTexUV.y = 1.0f - shadowTexUV.y;
    float4 color = rangeTexture.Sample(rangeSampler, shadowTexUV);
    color *= color;
	// ���C�g�̌���
    float3 lightDir = normalize(float3(1, 1, 0));
	// �@���̌���
    float3 normal = normalize(pixel.normal);
	// ���C�g�̋��x�����߂�
    float light = dot(lightDir, normal);

	// �n�[�t�����o�[�g�����߂�
	light = light * 0.5f + 0.5f;
	light = light * light;

    color *= light;
    color += float4(0.f, 0.2f, 0.2f,0.f);

    return color;
}