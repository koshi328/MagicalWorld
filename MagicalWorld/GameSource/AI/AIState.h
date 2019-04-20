/**
* File: AIState.h
* エネミーの行動状態の基底クラス
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#pragma once

class AIState
{
public:
	// コンストラクタ
	AIState() = default;
	// デストラクタ
	virtual ~AIState() = default;
	// 登録
	virtual void Entry() = 0;
	// 実行
	virtual void Execute() = 0;
	// 終了
	virtual void Exit() = 0;
};