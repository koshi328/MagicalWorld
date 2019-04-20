/**
* File: RingBurst.h
*
* スキル：爆発
*
* Date: 2017.1.17
* Author: koshi ito
*/
#pragma once
#include "Skill.h"

class RingBurst : public Skill
{
private:
	int castTime_; // 技が終了するまでの時間
public:
	// コンストラクタ
	RingBurst(Actor* subject);
	// デストラクタ
	~RingBurst();
	// 登録
	void Entry() override;
	// 実行
	bool Execute() override;
	// 終了
	void Exit() override;
};