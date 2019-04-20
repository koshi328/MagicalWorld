/**
* File: StatusGauge.cpp
*
* ステータスを表示するUI
*
* Date: 2016.12.20
* Author: koshi ito
*/
#include "StatusGauge.h"
#include "..\Object\Actor.h"
#include "..\..\GameBase\Graphics.h"
/**
* 名前空間
*/
using namespace GUI;
using namespace DirectX::SimpleMath;
/**
* 定数宣言
*/
#define OFF_SET (8)

/**
* コンストラクタ
* @param actor -- ステータスを表示したいActorのポインタ
*/
StatusGauge::StatusGauge(Actor* actor)
:actor_(actor)
{
	gaugeLength_ = 1000.f - (OFF_SET * 2);
	float x = (1280.f - 1000.f) / 2;
	float y = 720.f - 118.f;
	float gaugeHight = 10.f;
	flame_ = new Sprite(L"Resources/Texture/StatusGauge.png", x, y, 1000.f, 32.f);
	hpGauge_ = new Sprite(L"Resources/Texture/Gauge.png", x, y, 1000.f, 32.f);
	hpLostGauge_ = new Sprite(L"Resources/Texture/Gauge.png", x, y, 1000.f, 32.f);
}
/**
* デストラクタ
*/
StatusGauge::~StatusGauge()
{
	if(flame_)		// ウィンドウ
		delete flame_;
	if (hpLostGauge_)	// HPゲージ減少時の演出用
		delete hpLostGauge_;
	if(hpGauge_)	// HPゲージ
		delete hpGauge_;
}
/**
* 更新
*/
void StatusGauge::Update()
{
	// HPの計算
	float restWidth = ((float)actor_->getHP() / (float)actor_->getMaxHP()) * gaugeLength_ + OFF_SET;
	float width = hpGauge_->W() + (restWidth - hpGauge_->W()) * 0.5f;
	hpGauge_->W(width);
	// HP赤ゲージ
	width = hpLostGauge_->W() + (restWidth - hpLostGauge_->W()) * 0.05f;
	hpLostGauge_->W(width);
}
/**
* 描画
*/
void StatusGauge::Draw()
{
	if(flame_)			// ウィンドウ
		flame_->Draw();
	if (hpLostGauge_)	// HPゲージ減少時の演出用
		hpLostGauge_->Draw(Color(1, 0, 0, 1));
	if (hpGauge_)		// HP
		hpGauge_->Draw(Color(0, 1, 0.3f, 1));

	wchar_t buf[128];
	swprintf_s(buf, 128, L"%d",actor_->getHP());
	Graphics* graphics = Graphics::getInstance();
	graphics->getSpriteFont()->DrawString(graphics->getSpriteBatch(),
		buf,
		Vector2(flame_->X(), flame_->Y()), Color(1.f, 1.f, 1.f, 1.f));
}