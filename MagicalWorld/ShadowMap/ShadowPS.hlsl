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
P2F main(V2P pixel)
{
    P2F result;

	// ���C�g�̌���
    float3 lightDir = normalize(-LightDirection.xyz);
	// �@���̌���
    float3 normal = normalize(pixel.normal);

	// ���C�g�̋��x�����߂�
    float light = dot(lightDir, normal);

	// �F�����߂�
    float3 color;

	// ���C�g�̉e�������߂�i�e���A�e�ł͂Ȃ����j
    float LightAmount = 1.0f;
    {
		// �V���h�E�}�b�v�̍��W�����߂�
        float2 shadowTexUV = (pixel.lightPos.xy / pixel.lightPos.w) * 0.5f + 0.5f;
		// Y���W�𔽓]
        shadowTexUV.y = 1.0f - shadowTexUV.y;
		// �[���̌W�������߂�
        float depth = pixel.lightPos.z / pixel.lightPos.w;

        float SM_Z = depthTexture.Sample(depthSampler, shadowTexUV).x;

        if(depth > SM_Z + 0.003f)
        {
            LightAmount = 0.5f;
        }
    }


	// �e���܂߂����C�g�̋��x�����߂�
    light = saturate(light * LightAmount);
	// �F�����߂�
    color = LightDiffuse.rgb * MaterialDiffuse.rgb * light + LightAmbient.rgb * MaterialAmbient.rgb;
	// �ŏI�I�ȐF�̌���
    result.fragment = float4(color, 1.0f);

    return result;
}