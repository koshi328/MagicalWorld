/**
* File: SkillWindow.h
*
* 取得しているスキルを表示するUI
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
	// コントローラーのポインタ
	SkillController* _pSkillController;
	// スキルのアイコンを表示する為のSprite
	std::unique_ptr<GUI::Sprite> _sprite[SKILL_NUM - 1];
public:
	// コンストラクタ
	SkillWindow(SkillController* pCon);
	// デストラクタ
	~SkillWindow();
	// 更新
	void Update();
	// 描画
	void Draw();
};