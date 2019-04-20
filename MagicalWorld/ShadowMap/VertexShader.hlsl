cbuffer Matrixes : register(b0)
{
    float4x4 WorldViewProjection;
    float4x4 World;
    float4x4 LightViewProjection; // ���C�g��View*Projection
};

// ���C�g
cbuffer Lights : register(b1)
{
    float4 LightDirection; // ���C�g�̌���
    float4 LightDiffuse; // ���C�g�̃f�B�t���[�Y
    float4 LightAmbient; // ���C�g�̃A���r�G���g
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
float4 pos : SV_POSITION,   // ���W
float3 normal : NORMAL,     // �@��
float4 tangent : TANGENT,   // �ڐ�
float4 color : COLOR,       // �F
float2 uv : TEXCOORD0       // �e�N�X�`�����W
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
