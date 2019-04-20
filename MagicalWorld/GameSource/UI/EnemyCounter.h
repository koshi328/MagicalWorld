/**
* File: EnemyCounter.h
* エネミーの討伐率を示すUI
*
* Data:   2017.1.24
* Author: Koshi ito
*/
#pragma once
#include "GUI.h"
/**
* 前方宣言
*/
class Actor;
class EnemyCounter
{
public:
	// コンストラクタ
	EnemyCounter();
	// デストラクタ
	~EnemyCounter();
	// 更新
	void Update();
	// 描画
	void Draw();
private:
	GUI::Sprite* flame_;		// 枠の画像
	GUI::Sprite* Gauge_;		// ゲージ
	int effectTime_;			// 演出時間
};