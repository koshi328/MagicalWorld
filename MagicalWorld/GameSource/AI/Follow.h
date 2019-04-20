/**
* File: Follow.h
* 行動状態：対象を追跡する
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#pragma once
#include "AIState.h"
#include "..\Object\Enemy.h"

class Follow : public AIState
{
private:
	Enemy* subject_; // 実行対象
public:
	// コンストラクタ
	Follow(Enemy* subject);
	// デストラクタ
	~Follow();

	// 登録
	void Entry();
	// 実行
	void Execute();
	// 終了
	void Exit();
};