/**
* File: Camera.cpp
*
* カメラの姿勢を司る
*
* Date: 2016.11.10
* Author: koshi ito
*/
#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
/*
*	@brief コンストラクタ
*/
Camera::Camera()
:_fovY(XMConvertToRadians(60.0f))
, _aspect(1280.0f / 720.0f)
, _nearClip(0.1f)
, _farClip(3000.0f)
{
	_view = Matrix::Identity;
	_eye = Vector3(0.0f, 6.0f, -20.0f);
	_target = Vector3(0.0f, 2.0f, 0.0f);
	_up = Vector3(0.0f, 1.0f, 0.0f);
	_proj = Matrix::Identity;
}

/*
*	@brief デストラクタ
*/
Camera::~Camera()
{
}
/*
*	@brief 更新
*/
void Camera::Update()
{
	// ビュー行列を計算
	_view = Matrix::CreateLookAt(_eye, _target, _up);
	// プロジェクション行列を計算
	_proj = Matrix::CreatePerspectiveFieldOfView(_fovY, _aspect, _nearClip, _farClip);
}