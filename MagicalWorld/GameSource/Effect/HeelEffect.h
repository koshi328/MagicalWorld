/**
* File: HeelEffect.h
* �񕜃G�t�F�N�g
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include "..\..\Particle\ParticleManager.h"
#include <SimpleMath.h>
struct HeelEffect
{
	// �p�[�e�B�N���}�l�[�W���[
	ParticleManager* particleManager;
	// �����ʒu
	DirectX::SimpleMath::Vector3 pos;
	// ������
	void Initialize(ParticleManager* argParticleManager, DirectX::SimpleMath::Vector3 pos);
	// ���s
	bool Run();
};

