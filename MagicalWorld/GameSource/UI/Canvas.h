/**
* File: Canvas.h
*
* �\������UI���܂Ƃ߂ĊǗ�����N���X
*
* Date: 2016.12.14
* Author: koshi ito
*/
#pragma once
#include "StatusGauge.h"
#include "SkillWindow.h"
#include "EnemyCounter.h"
#include <memory>

class Canvas
{
private:
	std::unique_ptr<StatusGauge> statusGauge_;	// �X�e�[�^�XUI
	std::unique_ptr<SkillWindow> skillWindow_;	// �X�L��UI
	std::unique_ptr<EnemyCounter> enemyCounter_;// �G�l�~�[�J�E���^�[UI
public:
	// �R���X�g���N�^
	Canvas();
	// �f�X�g���N�^
	~Canvas();
	// ������
	void Initialize(Actor* actor, SkillController* pCon);
	// �X�V
	void Update();
	// �`��
	void Render();
};