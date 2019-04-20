/**
* File: Waiting.h
* �s����ԁF�U���Ώۂ�T���ē������
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#pragma once
#include "AIState.h"
#include "..\Object\Enemy.h"

class Waiting : public AIState
{
private:
	Enemy* subject_; // ���s�Ώ�
	int count_;		 // �i�s������ς���܂ł̃J�E���g
	float rotation_; // �i�s����
public:
	// �R���X�g���N�^
	Waiting(Enemy* subject);
	// �f�X�g���N�^
	~Waiting();
	// �o�^
	void Entry();
	// ���s
	void Execute();
	// �I��
	void Exit();
};