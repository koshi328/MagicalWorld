/**
* File: Follow.h
* �s����ԁF�Ώۂ�ǐՂ���
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#pragma once
#include "AIState.h"
#include "..\Object\Enemy.h"

class Follow : public AIState
{
private:
	Enemy* subject_; // ���s�Ώ�
public:
	// �R���X�g���N�^
	Follow(Enemy* subject);
	// �f�X�g���N�^
	~Follow();

	// �o�^
	void Entry();
	// ���s
	void Execute();
	// �I��
	void Exit();
};