/**
* File: GameScene.h
*
* ゲームシーンの処理
*
* Date: 2016.12.1
* Author: koshi ito
*/
#pragma once

#include "Scene.h"
#include "..\Object\ObjectManager.h"
#include "..\Collision\CollisionManager.h"
#include "..\Object\Stage.h"
#include "..\..\RangeMap\RangeMap.h"
#include "..\..\Sound\ADX2Le.h"

class GameScene : public Scene
{
public:
	// コンストラクタ
	GameScene();
	// デストラクタ
	~GameScene();

	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Render();
private:
	// オブジェクトマネージャー
	std::unique_ptr<ObjectManager> objectManager_;
	// コリジョンマネージャー
	std::unique_ptr<CollisionManager> collisionManager_;
	std::unique_ptr<StageManager> stageManager_;
	std::unique_ptr<RangeMap> rangeMap_;
	ADX2Le_Player* bgm_;
};