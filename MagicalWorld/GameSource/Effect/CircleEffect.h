/**
* File: CircleEffect.h
* �~�`�g��G�t�F�N�g
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include "..\..\Particle\ParticleManager.h"
#include <SimpleMath.h>
struct CircleEffect
{
	// �p�[�e�B�N���}�l�[�W���[�̃|�C���^
	ParticleManager* particleManager;
	// �����ʒu
	DirectX::SimpleMath::Vector3 pos;
	// ���a
	float radius;
	// ������
	void Initialize(ParticleManager* argParticleManager, DirectX::SimpleMath::Vector3 pos, float radius);
	// ���s
	bool Run();
};

