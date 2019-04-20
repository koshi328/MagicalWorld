/**
* File: Swing.h
*
* �X�L���F�ߐڊi���i3�i�K�U���j
*
* Date: 2016.12.1
* Author: koshi ito
*/
#pragma once
#include "Skill.h"

class Swing : public Skill
{
private:
	int attackNum_;	// �U���̒i�K
	int castTime_;	// ��������
	float angle_;	// �a���̊p�x
public:
	// �R���X�g���N�^
	Swing(Actor* subject);
	// �f�X�g���N�^
	~Swing();

	// �o�^
	void Entry() override;
	// ���s
	bool Execute() override;
	// �I��
	void Exit() override;

	// �a���̒i�K���̏�����
	void FirstInit();
	void SecondInit();
	void ThirdInit();

	// �a���̒i�K���̍X�V
	void FirstUpdate();
	void SecondUpdate();
	void ThirdUpdate();
};