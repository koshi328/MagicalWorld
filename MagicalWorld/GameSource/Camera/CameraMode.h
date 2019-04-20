/**
* File: CameraMode.h
*
* カメラの状態の基底クラス
*
* Date: 2016.11.10
* Author: koshi ito
*/
#pragma once
// 先行宣言
class Camera;

class CameraMode
{
protected:
	Camera* camera_;	// 操作するカメラのポインタ
public:
	// コンストラクタ
	CameraMode() = default;
	// デストラクタ
	virtual ~CameraMode() = default;

	// 更新
	virtual void Update() = 0;
	// アクセッサ
	void setCamera(Camera* camera)
	{
		camera_ = camera;
	}
};