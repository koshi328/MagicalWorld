/**
* File: HeelEffect.h
* 回復エフェクト
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include "..\..\Particle\ParticleManager.h"
#include <SimpleMath.h>
struct HeelEffect
{
	// パーティクルマネージャー
	ParticleManager* particleManager;
	// 発生位置
	DirectX::SimpleMath::Vector3 pos;
	// 初期化
	void Initialize(ParticleManager* argParticleManager, DirectX::SimpleMath::Vector3 pos);
	// 実行
	bool Run();
};

