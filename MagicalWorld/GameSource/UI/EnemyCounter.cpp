/**
* File: EnemyCounter.cpp
* エネミーの討伐率を示すUI
*
* Data:   2017.1.24
* Author: Koshi ito
*/
#include "EnemyCounter.h"
#include "..\Object\Actor.h"
#include "..\..\GameBase\Graphics.h"
#include "..\Object\Stage.h"

using namespace GUI;
using namespace DirectX::SimpleMath;
/**
* 数値設定
*/
#define FLAME_SIZE_X (64)
#define FLAME_SIZE_Y (720)
#define GAUGE_SIZE_X (28)
#define GAUGE_SIZE_Y (558)
#define OFFSET_X (18)
#define OFFSET_Y (150)
#define EFFECT_TIME (60)
#define EFFECT_SIZE_X (512)
#define EFFECT_SIZE_Y (128)
/**
* コンストラクタ
*/
EnemyCounter::EnemyCounter()
{
	effectTime_ = 0;
	float x = 1280.f - FLAME_SIZE_X;
	float y = 0.f;
	flame_ = new Sprite(L"Resources/Texture/EnemyCounter.png", x, y, FLAME_SIZE_X, FLAME_SIZE_Y);
	Gauge_ = new Sprite(L"Resources/Texture/EnemyCounterGauge.png", x + OFFSET_X, y + OFFSET_Y, GAUGE_SIZE_X, GAUGE_SIZE_Y);
}
/**
* デストラクタ
*/
EnemyCounter::~EnemyCounter()
{
	if(flame_) // ウィンドウ
		delete flame_;
	if (Gauge_)	// ゲージ
		delete Gauge_;
}
/**
* 更新
*/
void EnemyCounter::Update()
{
	StageManager* stageManager = StageManager::getInstance();
	int nextLevelNum = (stageManager->getLevel() * (stageManager->getLevel() + 1) / 2) * 10;
	nextLevelNum = nextLevelNum - stageManager->getKillEnemyNum();
	float length = (GAUGE_SIZE_Y - nextLevelNum / (stageManager->getLevel() * 10.f) * GAUGE_SIZE_Y) * 1.f;
	length = Gauge_->H() + (length - Gauge_->H()) * 0.05f;
	Gauge_->H(length);
	Gauge_->Y(OFFSET_Y + GAUGE_SIZE_Y - length);
	if (nextLevelNum <= 0)
	{
		effectTime_ = EFFECT_TIME;
	}
}
/**
* 描画
*/
void EnemyCounter::Draw()
{
	if(flame_) // ウィンドウ
		flame_->Draw();
	if (Gauge_) // HP
		Gauge_->Draw();
	if (effectTime_ > 0)
	{
		effectTime_--;
		Sprite levelUp(L"Resources/Texture/levelUp.png", 1280 / 2 - EFFECT_SIZE_X / 2, 720 / 2 - EFFECT_SIZE_Y / 2, EFFECT_SIZE_X, EFFECT_SIZE_Y);
		levelUp.Draw(Color(1, 1, 1, effectTime_ / (EFFECT_TIME * 1.f)));
	}
	// レベルの表示
	wchar_t buf[4];
	swprintf_s(buf, 4, L"%d", StageManager::getInstance()->getLevel());
	Graphics::getInstance()->getSpriteFont()->DrawString(Graphics::getInstance()->getSpriteBatch(),
		buf, Vector2(flame_->X() + 15, flame_->Y() + 90), Color(1, 1, 1, 1), 0.f, Vector2::Zero, 1.5f);
}