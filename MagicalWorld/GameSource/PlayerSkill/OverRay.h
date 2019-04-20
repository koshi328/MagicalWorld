/**
* File: OverRay.h
*
* �X�L���F���[�U�[
*
* Date: 2017.1.17
* Author: koshi ito
*/
#pragma once
#include "Skill.h"

class OverRay : public Skill
{
private:
	int castTime_; // �Z���I������܂ł̎���
public:
	// �R���X�g���N�^
	OverRay(Actor* subject);
	// �f�X�g���N�^
	~OverRay();
	// �o�^
	void Entry() override;
	// ���s
	bool Execute() override;
	// �I��
	void Exit() override;
};