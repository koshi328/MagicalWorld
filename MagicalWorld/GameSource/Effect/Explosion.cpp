/**
* File: Explosion.cpp
* �c���G�t�F�N�g
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "Explosion.h"

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
void Explosion::Initialize(ParticleManager* argParticleManager, Vector3 pos, int size)
{
	particleManager = argParticleManager;
	this->pos = pos;
	this->size = size;
}
/**
* ���s
*/
bool Explosion::Run()
{
	int time = 30;
	for (int i = 0; i < 100; i++)
	{
		Vector3 vel(randomRange(-1.0f, 1.0f), randomRange(-1.0f, 1.0f), randomRange(-1.0f, 1.0f));
		vel.Normalize();
		vel *= size * 1.f;
		vel /= time * 1.f;

		particleManager->Entry(
			time,
			pos.x, pos.y, pos.z,
			vel.x, vel.y, vel.z,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			size * 0.4f, 0.5f,
			1.0f, 1.0f, 0.5f, 1.0f,
			1.0f, 0.0f, 0.0f, 0.0f
			);
	}
	return false;
}