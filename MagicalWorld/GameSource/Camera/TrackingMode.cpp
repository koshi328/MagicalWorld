/**
* File: TrackingMode.cpp
*
* 対象を追尾するカメラモード
*
* Date: 2016.11.10
* Author: koshi ito
*/
#include "TrackingMode.h"
#include "..\..\Utility\Mouse.h"
#include "Camera.h"
#include <SimpleMath.h>
#include "..\..\Utility\Key.h"

using namespace DirectX::SimpleMath;

TrackingMode::TrackingMode(GameObject* target)
{
	target_ = target;
	Mouse::getInstance()->setMode(Mouse::MODE_RELATIVE);
	x_ = 0.0f;
	y_ = -30 * 3.14f / 180;
	distance_ = 100.0f;
}

TrackingMode::~TrackingMode()
{
}

void TrackingMode::Update()
{
	// カメラの距離制御
	if (Key::getInstance()->Press(VK_UP))
	{
		distance_ += 0.1f;
	}
	if (Key::getInstance()->Press(VK_DOWN))
	{
		distance_ -= 0.1f;
	}
	// ターゲットが居ない場合処理を中断
	if (!target_) return;
	// ターゲットが消滅する時のメッセージを受け取る
	if (target_->DestroyMessage())
	{
		target_ = nullptr;
	}
	// マウスから回転量を取得
	x_ -= Mouse::getInstance()->X() * 0.001f;
	y_ -= Mouse::getInstance()->Y() * 0.001f;

	// 制限
	if (y_ >= 1.57f) y_ = 1.57f;
	if (y_ <= -1.57f) y_ = -1.57f;
	// 回転量を適用した位置を算出
	Vector3 pos(0, 0, distance_);
	Matrix rotX = Matrix::CreateRotationX(y_);
	Matrix rotY = Matrix::CreateRotationY(x_);
	Matrix rot = rotX * rotY;
	pos = Vector3::Transform(pos, rot);

	// カメラに適用
	Vector3 offset(0, distance_ * 0.3f, 0);
	camera_->SetRefpos(target_->Position() + offset);
	camera_->SetEyepos(target_->Position() + pos + offset);
}
