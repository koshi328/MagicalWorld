/**
* File: EnemySkill.h
* �G�l�~�[�̃X�L���̊��N���X
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#pragma once

// �O���錾
class Enemy;
class EnemySkill
{
protected:
	int castTime_;		// ���s�ɂ����鎞��
	int delayTime_;		// ���s��A���̃X�L���𔭓�����܂ł̎���
	Enemy* subject_;	// ����Ώ�
public:
	// �R���X�g���N�^
	EnemySkill();
	// �f�X�g���N�^
	virtual ~EnemySkill();

	// ���s�֐�
	virtual void Entry() = 0;
	virtual bool Execute() = 0;
	virtual void Exit() = 0;

	// �A�N�Z�b�T
	inline const int& getDelayTime() { return delayTime_; }
};