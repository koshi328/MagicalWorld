/**
* File: Clave.h
* �X�L�� Clave(��F�S��)
*
* Data:   2017.1.10
* Author: Koshi ito
*/
#pragma once
#include "EnemySkill.h"

class Cleave : public EnemySkill
{
private:
	float angle_;		// ��]��
	int effectDilay_;	// �G�t�F�N�g�Ԋu
public:
	// �R���X�g���N�^
	Cleave(Enemy* subject);
	 // �f�X�g���N�^
	~Cleave();

	// �o�^
	void Entry();
	// ���s
	bool Execute();
	// �I��
	void Exit();
};