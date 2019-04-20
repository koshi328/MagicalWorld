/**
* File: Game.h
*
* ゲームのメイン処理
*
* Date: 2016.11.30
* Author: koshi ito
*/
#pragma once

#include <memory>
#include "GameBase.h"
#include "Graphics.h"

class Game : public GameBase
{
public:
	// コンストラクタ
	Game();
	// デストラクタ
	~Game();
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Render();
	// 終了
	void Finalize();
private:
	class Impl;
	std::unique_ptr<Impl> _pImpl;
};