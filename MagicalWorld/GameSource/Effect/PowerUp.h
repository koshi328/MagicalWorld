/**
* File: PowerUp.h
* パワーアップエフェクト
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include "..\..\Particle\ParticleManager.h"
#include <SimpleMath.h>
struct PowerUp
{
	// パーティクルマネージャーのポインタ
	ParticleManager* particleManager;
	// 発生位置
	DirectX::SimpleMath::Vector3 pos;
	// 大きさ
	float size;
	// 初期化
	void Initialize(ParticleManager* argParticleManager, DirectX::SimpleMath::Vector3 pos, float size);
	// 実行
	bool Run();
};

