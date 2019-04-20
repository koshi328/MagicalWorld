/**
* File: BombSetting.h
* �X�L�� BombSetting(��F���e�ݒu)
*
* Data:   2017.1.10
* Author: Koshi ito
*/
#pragma once
#include "EnemySkill.h"
#include <SimpleMath.h>

class BombSetting : public EnemySkill
{
private:
	static const int RANGE;				// �ő�ݒu����
	DirectX::SimpleMath::Vector3 pos[5];// �ݒu�ʒu
public:
	// �R���X�g���N�^
	BombSetting(Enemy* subject);
	 // �f�X�g���N�^
	~BombSetting();
	// �o�^
	void Entry();
	// ���s
	bool Execute();
	// �I��
	void Exit();
};