/**
* File: StatusGauge.h
*
* �X�e�[�^�X��\������UI
*
* Date: 2016.12.20
* Author: koshi ito
*/
#pragma once
#include "GUI.h"
/**
* �O���錾
*/
class Actor;
class StatusGauge
{
private:
	Actor* actor_;				// �ΏۃL�����N�^�[
	GUI::Sprite* flame_;		// �g�̉摜
	GUI::Sprite* hpGauge_;		// HP�Q�[�W
	GUI::Sprite* hpLostGauge_;	// HP�������̃o�b�N���o
	float gaugeLength_;			// �Q�[�W�̒���
public:
	// �R���X�g���N�^
	StatusGauge(Actor* actor);
	// �f�X�g���N�^
	~StatusGauge();
	// �X�V
	void Update();
	// �`��
	void Draw();
};