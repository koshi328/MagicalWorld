/**
* File: Radiation.cpp
* ���˃G�t�F�N�g
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "Radiation.h"

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
void Radiation::Initialize(ParticleManager* argParticleManager, Vector3 pos, int size)
{
	particleManager = argParticleManager;

	this->pos = pos;
	this->size = size;
}
/**
* ���s
*/
bool Radiation::Run()
{
	for (int i = 0; i < 8; i++)
	{
		float x = sinf((360.f / 8 * i) * 3.14f / 180);
		float z = cosf((360.f / 8 * i) * 3.14f / 180);
		for (int j = 0; j < 10; j++)
		{
			particleManager->Entry(
				size,
				pos.x, pos.y + j, pos.z,
				x, 0.f, z,
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f,
				5.0f, 5.0f,
				1.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 1.0f, 0.0f
				);
		}
	}
	return false;
}