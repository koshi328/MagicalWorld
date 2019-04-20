/**
* File: TitleScene.h
*
* タイトルシーン
*
* Date: 2016.11.1
* Author: koshi ito
*/
#pragma once

#include "Scene.h"

class TitleScene : public Scene
{
public:
	// コンストラクタ
	TitleScene();
	// デストラクタ
	~TitleScene();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Render();
private:
	float count_; // 経過時間
};