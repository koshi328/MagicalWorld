/**
* File: Gether.cpp
* �W��G�t�F�N�g
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "Gether.h"

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
* @param r -- ��
* @param g -- ��
* @param b -- ��
* @param a -- �A���t�@
*/
void Gether::Initialize(ParticleManager* argParticleManager, Vector3 pos, float size, float r, float g, float b, float a)
{
	particleManager = argParticleManager;

	this->pos = pos;
	this->size = size;
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}
/**
* ���s
*/
bool Gether::Run()
{
	float x = randomRange(-size, size);
	float y = randomRange(-size, size);
	float z = randomRange(-size, size);
	particleManager->Entry(
		22,
		pos.x + x, pos.y + y, pos.z + z,
		-x / 20, -y / 20, -z / 20,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 5.5f,
		r, g, b, a,
		1.0f, 1.0f, 0.0f, 1.0f
		);
	return false;
}