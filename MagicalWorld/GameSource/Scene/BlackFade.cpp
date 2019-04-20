/**
* File: BlackFade.cpp
*
* 黒くフェードする画面遷移演出
*
* Date: 2017.1.20
* Author: koshi ito
*/
#include "BlackFade.h"
#include "..\Scene\SceneManager.h"
#include <SimpleMath.h>

/**
* 名前空間
*/
using namespace DirectX::SimpleMath;
/**
* コンストラクタ
*/
BlackFade::BlackFade()
{
	filter_ = new GUI::Sprite(L"Resources/Texture/square64.png", 0, 0, 1280, 720);
}
/**
* デストラクタ
*/
BlackFade::~BlackFade()
{

}
/**
* 初期化
*/
void BlackFade::Initialize()
{
	time_ = 180;
	alpha_ = 0.0f;
}
/**
* 更新
*/
void BlackFade::Update()
{
	--time_;
	// 暗くする処理
	if (time_ > 120)
	{
		alpha_ += 1.0f / 60;
	}
	// 明るくする処理
	if (time_ < 60)
	{
		alpha_ -= 1.0f / 60;
	}
	// 暗くなった時にシーンを切り替える
	if (time_ == 60)
	{
		SceneManager::getInstance()->ChangeScene();
	}
	// 明るくなったら自身を破棄する
	if (time_ <= 0)
	{
		SceneManager::getInstance()->DisposeEffect();
	}

}
/**
* 描画
*/
void BlackFade::Render()
{
	filter_->Draw(Color(0, 0, 0, alpha_));
}