/**
* File: BlackFade.h
*
* 黒くフェードする画面遷移演出
*
* Date: 2017.1.20
* Author: koshi ito
*/
#pragma once

#include "Scene.h"
#include "..\UI\GUI.h"

class BlackFade : public Scene
{
public:
	// コンストラクタ
	BlackFade();
	// デストラクタ
	~BlackFade();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Render();
private:
	int time_;				// フェード時間
	float alpha_;			// アルファ値
	GUI::Sprite* filter_;	// テクスチャ
};
