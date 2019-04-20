/**
* File: CameraManager.h
*
* カメラを制御するマネージャー
*
* Date: 2016.11.10
* Author: koshi ito
*/
#pragma once

#include <memory>
#include "Camera.h"

// 先行宣言
class CameraMode;
class CameraEffect;
class Shaking;

class CameraManager
{
private:
	bool active_;
	Camera* camera_;					// カメラ
	CameraMode* mode_;					// カメラの状態
	static CameraManager* instance_;	// 自身のインスタンス

	// エフェクト
	std::unique_ptr<Shaking> shaking_;
	// コンストラクタ
	CameraManager();
public:
	// シングルトン
	static CameraManager* getInstance()
	{
		if (!instance_)
			instance_ = new CameraManager;
		return instance_;
	}
	// デストラクタ
	~CameraManager();
	// 更新
	void Update();
	// カメラを取得
	Camera* getCamera() { return camera_; }
	// モードを変更
	void setMode(CameraMode* mode);

	// カメラのActive状態を制御
	bool Active() { return active_; }
	void Active(bool input) { active_ = input; }

	// エフェクトの設定
	void ShakingMode(int time, float intensity);
};

// カメラを振動させるエフェクト
class Shaking
{
private:
	int time_;			// 時間
	float intensity_;	// 規模
	float flameWidth_;	// 揺れ幅
	float attenuation_;	// 減衰率
public:
	// コンストラクタ
	Shaking();
	// デストラクタ
	~Shaking();
	// 設定
	void Set(int time, float intensity);
	// 実行
	bool Execute(Camera* camera);
};