/**
* File: heel.h
*
* 対象を回復させるスキル
*
* Date: 2016.12.13
* Author: koshi ito
*/
#pragma once
#include "Skill.h"

class Heel : public Skill
{
public:
	// コンストラクタ
	Heel(Actor* subject);
	// デストラクタ
	~Heel();
	// 登録
	void Entry() override;
	// 実行
	bool Execute();
	// 終了
	void Exit() override;
};