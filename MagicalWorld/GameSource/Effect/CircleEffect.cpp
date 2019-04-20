/**
* File: CircleEffect.cpp
* �~�`�g��G�t�F�N�g
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "CircleEffect.h"

using namespace DirectX::SimpleMath;
/**
* �O���[�o���֐�
*/
float randomRange(float min, float max);
/**
* ������
* @param argParticleManager -- ParticleManager�̃|�C���^
* @param pos  -- �����ʒu
* @param radius -- ���a
*/
void CircleEffect::Initialize(ParticleManager* argParticleManager, Vector3 pos, float radius)
{
	particleManager = argParticleManager;

	this->pos = pos;
	this->radius = radius;
}

/**
* ���s
*/
bool CircleEffect::Run()
{
	for (int i = 0; i < 100; i++)
	{
		float x = sinf(6.28f / 100 * i) * radius / 30;
		float z = cosf(6.28f / 100 * i) * radius / 30;
		particleManager->Entry(
			30,
			pos.x, pos.y, pos.z,
			x, 0.0f, z,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			1.0f, 5.0f,
			1.0f, 0.5f, 0.0f, 0.2f,
			1.0f, 0.5f, 0.0f, 1.0f

			);
	}
	return false;
}