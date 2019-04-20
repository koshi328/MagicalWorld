/**
* File: TrackingMode.cpp
*
* 対象を追尾するカメラモード
*
* Date: 2016.11.10
* Author: koshi ito
*/
#pragma once

#include "CameraMode.h"
#include "..\Object\GameObject.h"

class TrackingMode : public CameraMode
{
private:
	GameObject* target_;	// 追尾対象
	float x_;				// X軸回転量
	float y_;				// Y軸回転量
	float distance_;		// 対象との距離
public:
	// コンストラクタ
	TrackingMode(GameObject* target);
	// デストラクタ
	~TrackingMode();
	// 更新
	void Update() override;
};