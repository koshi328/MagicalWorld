/**
* File: CircleEffect.h
* 円形波状エフェクト
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include "..\..\Particle\ParticleManager.h"
#include <SimpleMath.h>
struct CircleEffect
{
	// パーティクルマネージャーのポインタ
	ParticleManager* particleManager;
	// 発生位置
	DirectX::SimpleMath::Vector3 pos;
	// 半径
	float radius;
	// 初期化
	void Initialize(ParticleManager* argParticleManager, DirectX::SimpleMath::Vector3 pos, float radius);
	// 実行
	bool Run();
};

