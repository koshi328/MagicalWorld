/**
* File: Afterimage.cpp
* �c���G�t�F�N�g
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "Afterimage.h"

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
void Afterimage::Initialize(ParticleManager* argParticleManager, Vector3 pos, float size)
{
	particleManager = argParticleManager;

	this->pos = pos;
	this->size = size;
}
/**
* ���s
*/
bool Afterimage::Run()
{
	particleManager->Entry(
		10,
		pos.x, pos.y, pos.z,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		size, 0.5f,
		1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 0.0f
		);
	return false;
}