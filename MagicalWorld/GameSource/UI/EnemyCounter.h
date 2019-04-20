/**
* File: EnemyCounter.h
* �G�l�~�[�̓�����������UI
*
* Data:   2017.1.24
* Author: Koshi ito
*/
#pragma once
#include "GUI.h"
/**
* �O���錾
*/
class Actor;
class EnemyCounter
{
public:
	// �R���X�g���N�^
	EnemyCounter();
	// �f�X�g���N�^
	~EnemyCounter();
	// �X�V
	void Update();
	// �`��
	void Draw();
private:
	GUI::Sprite* flame_;		// �g�̉摜
	GUI::Sprite* Gauge_;		// �Q�[�W
	int effectTime_;			// ���o����
};