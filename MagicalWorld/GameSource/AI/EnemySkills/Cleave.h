/**
* File: Clave.h
* スキル Clave(訳：鉄槌)
*
* Data:   2017.1.10
* Author: Koshi ito
*/
#pragma once
#include "EnemySkill.h"

class Cleave : public EnemySkill
{
private:
	float angle_;		// 回転量
	int effectDilay_;	// エフェクト間隔
public:
	// コンストラクタ
	Cleave(Enemy* subject);
	 // デストラクタ
	~Cleave();

	// 登録
	void Entry();
	// 実行
	bool Execute();
	// 終了
	void Exit();
};