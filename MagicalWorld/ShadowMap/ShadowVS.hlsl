// �e�N�X�`��
Texture2D texture1 : register(t0); // �f�B�t���[�Y�e�N�X�`��
Texture2D depthTexture : register(t1); // �V���h�E�}�b�v�i�[�x�}�b�v�j
SamplerState textureSampler : register(s0); // �f�B�t���[�Y�e�N�X�`���p�T���v��
SamplerState depthSampler : register(s1); // �V���h�E�}�b�v�̃T���v��

// �s��
cbuffer Matrixes : register(b0)
{
    float4x4 WorldViewProjection; // World*View*Projection
    float4x4 World; // World
    float4x4 LightViewProjection; // ���C�g��View*Projection
};

// ���C�g
cbuffer Lights : register(b1)
{
    float4 LightDirection; // ���C�g�̌���
    float4 LightDiffuse; // ���C�g�̃f�B�t���[�Y
    float4 LightAmbient; // ���C�g�̃A���r�G���g
};

// �}�e���A��
cbuffer Materials : register(b2)
{
    float4 MaterialDiffuse; // �}�e���A���̃f�B�t���[�Y
    float4 MaterialAmbient; // �}�e���A���̃A���r�G���g
};

// �s�N�Z���V�F�[�_�ɑ�����
struct V2P
{
    float4 pos : SV_POSITION; // ���W
    float3 normal : NORMAL; // �@��
    float4 color : COLOR; // �F
    float2 uv : TEXCOORD0; // �e�N�X�`���̍��W
    float4 lightPos : LIGHTPOS; // ���C�g��ԃv���W�F�N�V�������W
};

// �ŏI����
struct P2F
{
    float4 fragment : SV_TARGET; // �F
};

// ���C���֐�
V2P main(
	float4 pos : SV_POSITION, // ���W
	float3 normal : NORMAL, // �@��
	float4 tangent : TANGENT, // �ڐ�
	float4 color : COLOR, // �F
	float2 uv : TEXCOORD0 // �e�N�X�`�����W
	)
{
    V2P result;

	// ���W�ϊ�
    result.pos = mul(pos, WorldViewProjection);
	// ���[���h�@��
    result.normal = mul(normal, (float3x3) World);
	// �F
    result.color = color;
	// �e�N�X�`�����W
    result.uv = uv;

	// ���C�g��ԃv���W�F�N�V�������W
    float4 lightPos = pos;
    lightPos = mul(lightPos, World);
    lightPos = mul(lightPos, LightViewProjection);
    result.lightPos = lightPos;

    return result;
}