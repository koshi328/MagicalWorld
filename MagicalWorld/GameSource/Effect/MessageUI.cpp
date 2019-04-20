/**
* File: MessageUI.cpp
* 文字表示
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "MessageUI.h"
#include "..\..\GameBase\Graphics.h"

using namespace DirectX::SimpleMath;
/**
* 初期化
* @param view  -- カメラview
* @param proj  -- カメラprojection
* @param screenWidth  -- スクリーンサイズW
* @param screenHeight -- スクリーンサイズH
* @param pos   -- 発生位置
* @param count -- 表示時間
* @param buf   -- 表示文字
* @param color -- 表示色
*/
void MessageUI::Initialize(const Matrix& view, const Matrix& proj,
	const float& screenWidth, const float& screenHight, const Vector3& pos, const int& count, const wchar_t* buf, Color color)
{
	// スクリーン座標計算
	float w = screenWidth / 2;
	float h = screenHight / 2;
	Matrix viewPort = {
		w,  0, 0, 0,
		0, -h, 0, 0,
		0,  0, 1, 0,
		w,  h, 0, 1
	};
	Vector3 tmp = pos;
	tmp = Vector3::Transform(tmp, view);
	tmp = Vector3::Transform(tmp, proj);
	tmp.x /= tmp.z; tmp.y /= tmp.z; tmp.z /= tmp.z;
	position_ = Vector3::Transform(tmp, viewPort);
	count_ = count;
	wcscpy_s(str_, 32, buf);
	color_ = color;
}
/**
* 実行
*/
bool MessageUI::Run()
{
	color_.w = 1.f - (1.0f / count_) * 4.0f;
	position_.y -= 0.5f;
	Graphics::getInstance()->getSpriteFont()->DrawString(
		Graphics::getInstance()->getSpriteBatch(), str_, position_,
		color_, 0.0f, Vector2::Zero, 1.5f);
	if (--count_ > 0) return true;
	return false;
}