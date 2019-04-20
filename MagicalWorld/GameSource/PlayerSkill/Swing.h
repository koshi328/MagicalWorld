/**
* File: Swing.h
*
* スキル：近接格闘（3段階攻撃）
*
* Date: 2016.12.1
* Author: koshi ito
*/
#pragma once
#include "Skill.h"

class Swing : public Skill
{
private:
	int attackNum_;	// 攻撃の段階
	int castTime_;	// 発動時間
	float angle_;	// 斬撃の角度
public:
	// コンストラクタ
	Swing(Actor* subject);
	// デストラクタ
	~Swing();

	// 登録
	void Entry() override;
	// 実行
	bool Execute() override;
	// 終了
	void Exit() override;

	// 斬撃の段階毎の初期化
	void FirstInit();
	void SecondInit();
	void ThirdInit();

	// 斬撃の段階毎の更新
	void FirstUpdate();
	void SecondUpdate();
	void ThirdUpdate();
};