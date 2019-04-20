/**
* File: GameBase.h
*
* ゲームの基礎部分の処理
*
* Date: 2016.11.30
* Author: koshi ito
*/
#pragma once

#include <memory>

class GameBase
{
public:
	// コンストラクタ
	GameBase(GameBase* pGame);
	// デストラクタ
	~GameBase();
	// 初期化
	virtual void Initialize();
	// 更新
	virtual void Update();
	// 描画
	virtual void Render();
	// 終了
	virtual void Finalize();
	// 実行
	void Run();
private:
	class Impl;
	std::unique_ptr<Impl> _pImpl;
};