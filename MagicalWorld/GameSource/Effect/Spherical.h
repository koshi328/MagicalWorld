/**
* File: Spherical.h
* ��������˃G�t�F�N�g
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include "..\..\Particle\ParticleManager.h"
#include <SimpleMath.h>
struct Spherical
{
	// �p�[�e�B�N���}�l�[�W���[�̃|�C���^
	ParticleManager* particleManager;
	// �����ʒu
	DirectX::SimpleMath::Vector3 pos;
	// �傫��
	float size;
	// ����
	int time;
	// ������
	void Initialize(ParticleManager* argParticleManager, DirectX::SimpleMath::Vector3 pos, float size, int time);
	// ���s
	bool Run();
};

