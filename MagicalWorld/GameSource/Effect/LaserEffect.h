/**
* File: LaserEffect.h
* レーザーエフェクト
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include "..\..\Particle\ParticleManager.h"
#include <SimpleMath.h>
struct LaserEffect
{
	// パーティクルマネージャーのポインタ
	ParticleManager* particleManager;
	// 発生位置
	DirectX::SimpleMath::Vector3 pos;
	// 太さ
	float size;
	// 長さ
	float length;
	// 向き
	float angle;
	// 初期化
	void Initialize(ParticleManager* argParticleManager, DirectX::SimpleMath::Vector3 pos, float size,float length, float angle);
	// 実行
	bool Run();
};

