/**
* File: TitleScene.cpp
*
* タイトルシーン
*
* Date: 2016.11.1
* Author: koshi ito
*/
#include "TitleScene.h"
#include "GameScene.h"
#include "..\..\Utility\Key.h"
#include "..\..\Utility\Mouse.h"
#include "..\..\Utility\MeshManager.h"
#include "..\Scene\SceneManager.h"
#include "..\..\GameBase\Graphics.h"
#include "..\UI\GUI.h"
#include "..\Camera\CameraManager.h"
#include <SimpleMath.h>
#include "..\Object\Stage.h"
// サウンド
#include "..\..\Sound\ADX2Le.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"
/**
* グローバル関数
*/
extern MeshManager* getMeshManager();
/**
* 名前空間
*/
using namespace DirectX::SimpleMath;
/**
* コンストラクタ
*/
TitleScene::TitleScene()
{
}
/**
* デストラクタ
*/
TitleScene::~TitleScene()
{

}
/**
* 初期化
*/
void TitleScene::Initialize()
{
	StageManager::getInstance()->Initialize();
	StageManager::getInstance()->setMaxTime(60.f);
	Mouse::getInstance()->setMode(Mouse::MODE_ABSOLUTE);
	CameraManager::getInstance()->setMode(nullptr);
	count_ = 0.f;
	ADX2Le::Play(CRI_CUESHEET_0_BGM01);
}
/**
* 更新
*/
void TitleScene::Update()
{
	count_++;
}
/**
* 描画
*/
void TitleScene::Render()
{
	// 変数を定義
	StageManager* stageM = StageManager::getInstance();
	float width = 1280.f;
	float hight = 720.f;
	float flameW = 256.f;
	float flameH = 64.f;
	// 背景の生成と描画
	GUI::Sprite titleBack(L"Resources/Texture/titleBack.png", 0, 0, width, hight);
	titleBack.Draw();
	// タイトル文字の演出
	float loopEffect = sinf(count_ * 3.14f / 180);
	// 文字の明るさを変動させる
	float light = 1.f + fabs(loopEffect) * 5;
	// タイトル文字を生成描画
	GUI::Sprite title(L"Resources/Texture/Title.png", 0, loopEffect * 10, width, hight);
	title.Draw(Color(light, light, light, 1.f));
	// スタートボタンを描画
	GUI::Button StartButton(L"Resources/Texture/startButton.png", width / 2 - flameW / 2, hight / 2 + flameH, flameW, flameH);
	// クリックされた時の処理
	if (StartButton.ClickCheck() == 1 && !SceneManager::getInstance()->getEffectActive())
	{
		SceneManager::getInstance()->setNextScene(SceneManager::SCENE_ID::PLAY);
		SceneManager::getInstance()->ChangeScene(SceneManager::CHANGE_TYPE::BKACK_FADE);
		stageM->setTime(stageM->getMaxTime());
		ADX2Le::Stop();
		ADX2Le::Play(CRI_CUESHEET_0_SE_HIT);
	}
	if (StartButton.ClickCheck() == 2)
		StartButton.Draw(Color(1.f, 1.f, 0.f, 1.f));
	else
		StartButton.Draw();
	// 時間設定
	GUI::Button TimeFlame(L"Resources/Texture/Flame.png", width / 2 - flameW / 2, hight / 2 - flameH, flameW, flameH);
	GUI::Button LeftButton(L"Resources/Texture/leftButton.png", width / 2 - flameW / 2 - 32, hight / 2 - flameH, 32, flameH);
	GUI::Button RightButton(L"Resources/Texture/rightButton.png", width / 2 - flameW / 2 + flameW, hight / 2 - flameH, 32, flameH);
	TimeFlame.Draw();
	// クリックされた時の処理
	if (LeftButton.ClickCheck() == 1)
	{
		if (stageM->getMaxTime() > 60)
		{
			stageM->addMaxTime(-60);
			ADX2Le::Play(CRI_CUESHEET_0_SE_GET);
		}
	}
	if (LeftButton.ClickCheck() == 2)
		LeftButton.Draw(Color(1.f, 1.f, 0.f, 1.f));
	else
		LeftButton.Draw();
	if (RightButton.ClickCheck() == 1)
	{
		if (stageM->getMaxTime() <= 60 * 9)
		{
			stageM->addMaxTime(60);
			ADX2Le::Play(CRI_CUESHEET_0_SE_GET);
		}
	}
	if (RightButton.ClickCheck() == 2)
		RightButton.Draw(Color(1.f, 1.f, 0.f, 1.f));
	else
		RightButton.Draw();
	wchar_t buf[16];
	swprintf_s(buf, 8, L"%.1f", stageM->getMaxTime());
	Graphics::getInstance()->getSpriteFont()->DrawString(Graphics::getInstance()->getSpriteBatch(),
		buf, Vector2(width / 2 - 64, hight / 2 - flameH), Color(0, 0, 0, 1), 0.f, Vector2::Zero, 1.5f);
}