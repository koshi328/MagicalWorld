/**
* File: Explosion.h
* 残像エフェクト
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include "..\..\Particle\ParticleManager.h"
#include <SimpleMath.h>
struct Explosion
{
	// パーティクルマネージャーのポインタ
	ParticleManager* particleManager;
	// 発生位置
	DirectX::SimpleMath::Vector3 pos;
	// 大きさ
	int size;
	// 初期化
	void Initialize(ParticleManager* argParticleManager, DirectX::SimpleMath::Vector3 pos, int size);
	// 実行
	bool Run();
};

