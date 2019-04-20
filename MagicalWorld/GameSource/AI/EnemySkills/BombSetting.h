/**
* File: BombSetting.h
* スキル BombSetting(訳：爆弾設置)
*
* Data:   2017.1.10
* Author: Koshi ito
*/
#pragma once
#include "EnemySkill.h"
#include <SimpleMath.h>

class BombSetting : public EnemySkill
{
private:
	static const int RANGE;				// 最大設置距離
	DirectX::SimpleMath::Vector3 pos[5];// 設置位置
public:
	// コンストラクタ
	BombSetting(Enemy* subject);
	 // デストラクタ
	~BombSetting();
	// 登録
	void Entry();
	// 実行
	bool Execute();
	// 終了
	void Exit();
};