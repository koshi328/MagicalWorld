/**
* File: StatusGauge.h
*
* ステータスを表示するUI
*
* Date: 2016.12.20
* Author: koshi ito
*/
#pragma once
#include "GUI.h"
/**
* 前方宣言
*/
class Actor;
class StatusGauge
{
private:
	Actor* actor_;				// 対象キャラクター
	GUI::Sprite* flame_;		// 枠の画像
	GUI::Sprite* hpGauge_;		// HPゲージ
	GUI::Sprite* hpLostGauge_;	// HP減少時のバック演出
	float gaugeLength_;			// ゲージの長さ
public:
	// コンストラクタ
	StatusGauge(Actor* actor);
	// デストラクタ
	~StatusGauge();
	// 更新
	void Update();
	// 描画
	void Draw();
};