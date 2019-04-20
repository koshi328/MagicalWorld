/**
* File: HammerOfAnger.h
* スキル HammerOfAnger(訳：怒りの鉄槌)
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#pragma once
#include "EnemySkill.h"

class HammerOfAnger : public EnemySkill
{
private:
	static const float RANGE;
public:
	// コンストラクタ
	HammerOfAnger(Enemy* subject);
	// デストラクタ
	~HammerOfAnger();
	// 登録
	void Entry();
	// 実行
	bool Execute();
	// 終了
	void Exit();
};