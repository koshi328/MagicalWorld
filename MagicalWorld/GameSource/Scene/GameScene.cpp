/**
* File: GameScene.cpp
*
* ゲームシーンの処理
*
* Date: 2016.12.1
* Author: koshi ito
*/

#include "GameScene.h"
#include "..\UI\GUI.h"
#include "..\..\GameBase\Graphics.h"
#include "..\..\Utility\MeshManager.h"
#include "..\Object\Player.h"
#include "..\Object\Enemy.h"
#include "..\Collision\Terrain.h"
#include <SimpleMath.h>
#include "..\..\Utility\TaskManager.h"
#include "..\..\Utility\Key.h"
#include "..\Scene\SceneManager.h"
#include "..\..\RangeMap\RangeMap.h"
// サウンド
#include "..\..\Sound\ADX2Le.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"

#include "..\Collision\Collision2D.h"
/**
* グローバル関数
*/
extern MeshManager* getMeshManager();
extern TaskManager* getTaskManager();
/**
* 名前空間
*/
using namespace DirectX::SimpleMath;
using namespace GUI;
/**
* コンストラクタ
*/
GameScene::GameScene()
{
	objectManager_.reset(ObjectManager::getInstance());
	collisionManager_.reset(CollisionManager::getInstance());
	stageManager_.reset(StageManager::getInstance());
	rangeMap_.reset(RangeMap::getInstance());
}
/**
* デストラクタ
*/
GameScene::~GameScene()
{

}
/**
* 初期化
*/
void GameScene::Initialize()
{
	// オブジェクトの初期化
	getTaskManager()->Claer();
	objectManager_->Clear();
	// ステージの生成
	stageManager_->LoadStageInfo(L"Resources/Data/Stage/stage1.txt");
	// プレイヤーの生成(スクリプト化予定)
	objectManager_->Add(L"Player", new Player(Vector3(0, 0, 0)))->Initialize();
	// BGMの再生
	ADX2Le::Play(CRI_CUESHEET_0_BGM_WAR);
}
/**
* 更新
*/
void GameScene::Update()
{
	if (SceneManager::getInstance()->getEffectActive()) return;
	// 背景用天球の登録
	getMeshManager()->Entry(3, Matrix::Identity);
	// オブジェクトの更新処理を呼び出す
	objectManager_->Update();
	// 衝突判定を更新
	collisionManager_->Update();
	// ステージ情報を更新
	stageManager_->Update();
	// ダメージサウンドの多重再生防止（仮）
	Actor::SoundReset();
}
/**
* 描画
*/
void GameScene::Render()
{
	Graphics* graphics = Graphics::getInstance();
	wchar_t buf[32];
	swprintf_s(buf, 32, L"KILL:%d", stageManager_->getKillEnemyNum());
	// オブジェクトの描画を呼び出す
	objectManager_->Render();

	// プレイヤーが居ない場合リザルトを表示する
	for each(Player* obj in ObjectManager::getInstance()->getActorList(L"Player"))
	{
		if (obj->getHP() <= 0)
		{
			Sprite result(L"Resources/Texture/Result.png", 0, 0, 1280, 720);
			result.Draw();
			graphics->getSpriteFont()->DrawString(graphics->getSpriteBatch(), buf, Vector2(1280 / 3, 720 / 2), Color(1, 0, 0, 1), 0.f, Vector2::Zero, 2.0f);
			// リザルト画面が出ている時スペースキーを押すとタイトルに遷移
			if (Key::getInstance()->Touch(VK_SPACE))
			{
				SceneManager::getInstance()->setNextScene(SceneManager::TITLE);
				SceneManager::getInstance()->ChangeScene(SceneManager::BKACK_FADE);
			}
		}
	}
	// 時間切れクリア
	if (stageManager_->getTime() <= 0)
	{
		for each(GameObject* obj in ObjectManager::getInstance()->getActorList(L"Enemy"))
		{
			obj->Destroy(obj);
		}
		Sprite result(L"Resources/Texture/Clear.png", 0, 0, 1280, 720);
		result.Draw();
		graphics->getSpriteFont()->DrawString(graphics->getSpriteBatch(), buf, Vector2(1280 / 3, 720 / 2), Color(1, 0, 0, 1), 0.f, Vector2::Zero, 2.0f);
		// リザルト画面が出ている時スペースキーを押すとタイトルに遷移
		if (Key::getInstance()->Touch(VK_SPACE))
		{
			SceneManager::getInstance()->setNextScene(SceneManager::TITLE);
			SceneManager::getInstance()->ChangeScene(SceneManager::BKACK_FADE);
		}
	}
	// スコア表示
	graphics->getSpriteFont()->DrawString(graphics->getSpriteBatch(), buf, Vector2(), Color(1, 0, 0, 1), 0.f, Vector2::Zero, 1.5f);

	// 時計表示
	float x = 1280 / 2 - 32.f;
	float rotation = 6.14f - stageManager_->getTime() / stageManager_->getMaxTime() * 6.28f;
	Sprite TimerFlame(L"Resources/Texture/TimerFlame.png", x, 0, 128, 128);
	Sprite Timer(L"Resources/Texture/Timer.png", x, 0, 128, 128);
	TimerFlame.Draw();
	Timer.Draw(Color(1, 1, 1, 1), rotation);
	//ShadowMap::getInstance()->Render();

}