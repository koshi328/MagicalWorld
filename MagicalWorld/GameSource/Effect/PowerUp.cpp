/**
* File: PowerUp.cpp
* �p���[�A�b�v�G�t�F�N�g
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "PowerUp.h"

using namespace DirectX::SimpleMath;
/**
* �O���[�o���֐�
*/
float randomRange(float min, float max);
/**
* ������
* @param argParticleManager -- ParticleManager�̃|�C���^
* @param pos  -- �����ʒu
* @param size -- �傫��
*/
void PowerUp::Initialize(ParticleManager* argParticleManager, Vector3 pos, float size)
{
	particleManager = argParticleManager;

	this->pos = pos;
	this->size = size;
}
/**
* ���s
*/
bool PowerUp::Run()
{
	int num = 8;

	for (int i = 0; i < num; i++)
	{
		float x = sinf((360 / num * i) * 3.14f / 180) * size;
		float z = cosf((360 / num * i) * 3.14f / 180) * size;
		for (int j = 0; j < 20; j++)
		{
			particleManager->Entry(
				20,
				pos.x + x, pos.y + j, pos.z + z,
				0.0f, size / 20, 0.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f,
				size / 2, size / 2,
				1.0f, 0.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 0.0f, 0.0f
				);
		}
	}

	return false;
}