/**
* File: heel.h
*
* �Ώۂ��񕜂�����X�L��
*
* Date: 2016.12.13
* Author: koshi ito
*/
#pragma once
#include "Skill.h"

class Heel : public Skill
{
public:
	// �R���X�g���N�^
	Heel(Actor* subject);
	// �f�X�g���N�^
	~Heel();
	// �o�^
	void Entry() override;
	// ���s
	bool Execute();
	// �I��
	void Exit() override;
};