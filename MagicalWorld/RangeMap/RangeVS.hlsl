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

V2P main(
float4 pos : SV_POSITION, // ���W
float3 normal : NORMAL, // �@��
float4 tangent : TANGENT, // �ڐ�
float4 color : COLOR, // �F
float2 uv : TEXCOORD0 // �e�N�X�`�����W
)
{
    V2P result; // ���ʂ�������ϐ�
    result.pos = mul(pos, WorldViewProjection);
    result.normal = normal;

    	// ���C�g��ԃv���W�F�N�V�������W
    float4 lightPos = pos;
    lightPos = mul(lightPos, AboveWorldViewProj);
    result.lightPos = lightPos;

    return result; // �s�N�Z���V�F�[�_�֌��ʂ𑗂�
}