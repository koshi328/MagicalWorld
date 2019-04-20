/**
* File: Gether.h
* 集約エフェクト
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include "..\..\Particle\ParticleManager.h"
#include <SimpleMath.h>
struct Gether
{
	// パーティクルマネージャーのポインタ
	ParticleManager* particleManager;
	// 発生位置
	DirectX::SimpleMath::Vector3 pos;
	// 大きさ
	float size;
	// RGBA数値
	float r;
	float g;
	float b;
	float a;
	// 初期化
	void Initialize(ParticleManager* argParticleManager, DirectX::SimpleMath::Vector3 pos, float size, float r, float g, float b, float a);
	// 実行
	bool Run();
};

