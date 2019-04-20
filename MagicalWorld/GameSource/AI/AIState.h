/**
* File: AIState.h
* �G�l�~�[�̍s����Ԃ̊��N���X
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#pragma once

class AIState
{
public:
	// �R���X�g���N�^
	AIState() = default;
	// �f�X�g���N�^
	virtual ~AIState() = default;
	// �o�^
	virtual void Entry() = 0;
	// ���s
	virtual void Execute() = 0;
	// �I��
	virtual void Exit() = 0;
};