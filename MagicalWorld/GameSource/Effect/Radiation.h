/**
* File: Radiation.h
* ���˃G�t�F�N�g
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include "..\..\Particle\ParticleManager.h"
#include <SimpleMath.h>
struct Radiation
{
	// �p�[�e�B�N���}�l�[�W���[�̃|�C���^
	ParticleManager* particleManager;
	// �����ʒu
	DirectX::SimpleMath::Vector3 pos;
	// �傫��
	int size;
	// ������
	void Initialize(ParticleManager* argParticleManager, DirectX::SimpleMath::Vector3 pos, int size);
	// ���s
	bool Run();
};

