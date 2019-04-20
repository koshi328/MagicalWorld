/**
* File: HammerOfAnger.h
* �X�L�� HammerOfAnger(��F�{��̓S��)
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#pragma once
#include "EnemySkill.h"

class HammerOfAnger : public EnemySkill
{
private:
	static const float RANGE;
public:
	// �R���X�g���N�^
	HammerOfAnger(Enemy* subject);
	// �f�X�g���N�^
	~HammerOfAnger();
	// �o�^
	void Entry();
	// ���s
	bool Execute();
	// �I��
	void Exit();
};