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
    result.uv = uv;
    result.color = color;

    return result; // �s�N�Z���V�F�[�_�֌��ʂ𑗂�
}