/**
* File: Waiting.h
* 行動状態：攻撃対象を探して動き回る
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#pragma once
#include "AIState.h"
#include "..\Object\Enemy.h"

class Waiting : public AIState
{
private:
	Enemy* subject_; // 実行対象
	int count_;		 // 進行方向を変えるまでのカウント
	float rotation_; // 進行方向
public:
	// コンストラクタ
	Waiting(Enemy* subject);
	// デストラクタ
	~Waiting();
	// 登録
	void Entry();
	// 実行
	void Execute();
	// 終了
	void Exit();
};