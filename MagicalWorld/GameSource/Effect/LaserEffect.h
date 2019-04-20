/**
* File: LaserEffect.h
* ���[�U�[�G�t�F�N�g
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include "..\..\Particle\ParticleManager.h"
#include <SimpleMath.h>
struct LaserEffect
{
	// �p�[�e�B�N���}�l�[�W���[�̃|�C���^
	ParticleManager* particleManager;
	// �����ʒu
	DirectX::SimpleMath::Vector3 pos;
	// ����
	float size;
	// ����
	float length;
	// ����
	float angle;
	// ������
	void Initialize(ParticleManager* argParticleManager, DirectX::SimpleMath::Vector3 pos, float size,float length, float angle);
	// ���s
	bool Run();
};

