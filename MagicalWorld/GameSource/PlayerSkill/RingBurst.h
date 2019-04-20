/**
* File: RingBurst.h
*
* �X�L���F����
*
* Date: 2017.1.17
* Author: koshi ito
*/
#pragma once
#include "Skill.h"

class RingBurst : public Skill
{
private:
	int castTime_; // �Z���I������܂ł̎���
public:
	// �R���X�g���N�^
	RingBurst(Actor* subject);
	// �f�X�g���N�^
	~RingBurst();
	// �o�^
	void Entry() override;
	// ���s
	bool Execute() override;
	// �I��
	void Exit() override;
};