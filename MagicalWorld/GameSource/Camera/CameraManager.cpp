/**
* File: CameraManager.cpp
*
* カメラを制御するマネージャー
*
* Date: 2016.11.10
* Author: koshi ito
*/
#include "CameraManager.h"
#include "CameraMode.h"

using namespace DirectX::SimpleMath;

// staticの実体化
CameraManager* CameraManager::instance_;

/**
* コンストラクタ
*/
CameraManager::CameraManager()
{
	active_ = true;
	camera_ = new Camera;
	shaking_.reset(new Shaking);
}

/**
* デストラクタ
*/
CameraManager::~CameraManager()
{
	delete mode_;
	delete camera_;
	instance_ = nullptr;
}

/**
* 更新
*/
void CameraManager::Update()
{
	// アクティブ判定
	if (!active_) return;
	// エフェクト
	// 揺れ
	shaking_->Execute(camera_);
	// カメラ更新
	camera_->Update();
	// カメラのモード
	if(mode_)
		mode_->Update();
}

/**
* カメラのモード変更
* @param mode --- CameraModeの派生クラス
*/
void CameraManager::setMode(CameraMode* mode)
{
	if (mode_)
		delete mode_;
	mode_ = mode;

	if(mode_)
		mode_->setCamera(camera_);
}

/**
* 揺れエフェクトの設定
*/
void CameraManager::ShakingMode(int time, float intensity)
{
	shaking_->Set(time, intensity);
}

/**
* カメラエフェクト
*/

/**
* 揺れエフェクト
*/
/**
* コンストラクタ
*/
Shaking::Shaking()
{

}
/**
* デストラクタ
*/
Shaking::~Shaking()
{

}

/**
* 設定
* @param time      --- 時間
* @param intensity --- 規模レベル
*/
void Shaking::Set(int time, float intensity)
{
	time_ = time;
	intensity_ = intensity;
	flameWidth_ = 3.14f * 10 / time_;
	attenuation_ = intensity_ / time_;
}

/**
* 実行
* @param camera --- 適用するカメラのポインタ
*/
bool Shaking::Execute(Camera* camera)
{
	if (--time_ < 0) return false;
	intensity_ -= attenuation_;
	float shak = sinf(flameWidth_ * time_) * intensity_;

	Vector3 eye = camera->GetEyepos();
	Vector3 ref = camera->GetRefpos();
	eye.y += shak;
	ref.y += shak;
	camera->SetEyepos(eye);
	camera->SetRefpos(ref);
}