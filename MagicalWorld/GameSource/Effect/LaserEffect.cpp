/**
* File: LaserEffect.cpp
* ���[�U�[�G�t�F�N�g
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "LaserEffect.h"

using namespace DirectX::SimpleMath;
/**
* �O���[�o���֐�
*/
float randomRange(float min, float max);
/**
* ������
* @param argParticleManager -- ParticleManager�̃|�C���^
* @param pos    -- �����ʒu
* @param size   -- ����
* @param length -- ����
* @param angle  -- ����
*/
void LaserEffect::Initialize(ParticleManager* argParticleManager, Vector3 pos, float size, float length, float angle)
{
	particleManager = argParticleManager;

	this->pos = pos;
	this->size = size;
	this->angle = angle;
	this->length = length;
}
/**
* ���s
*/
bool LaserEffect::Run()
{
	float x = sinf(angle);
	float z = cosf(angle);
	for (int i = 0; i < length; i++)
	{
		particleManager->Entry(
			30,
			pos.x + x * i, pos.y, pos.z + z * i,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			size, 0.0f,
			1.0f, 0.5f, 0.5f, 1.0f,
			1.0f, 1.0f, 1.0f, 0.0f
			);
	}
	return false;
}