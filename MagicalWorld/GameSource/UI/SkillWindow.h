/**
* File: SkillWindow.h
*
* �擾���Ă���X�L����\������UI
*
* Date: 2016.12.20
* Author: koshi ito
*/
#pragma once
#include <memory>
#include "GUI.h"
#include "..\PlayerSkill\SkillController.h"
class SkillWindow
{
private:
	// �R���g���[���[�̃|�C���^
	SkillController* _pSkillController;
	// �X�L���̃A�C�R����\������ׂ�Sprite
	std::unique_ptr<GUI::Sprite> _sprite[SKILL_NUM - 1];
public:
	// �R���X�g���N�^
	SkillWindow(SkillController* pCon);
	// �f�X�g���N�^
	~SkillWindow();
	// �X�V
	void Update();
	// �`��
	void Draw();
};