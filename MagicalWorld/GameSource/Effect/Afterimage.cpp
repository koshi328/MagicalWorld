/**
* File: Afterimage.cpp
* 残像エフェクト
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "Afterimage.h"

using namespace DirectX::SimpleMath;
/**
* グローバル関数
*/
float randomRange(float min, float max);
/**
* 初期化
* @param argParticleManager -- ParticleManagerのポインタ
* @param pos  -- 発生位置
* @param size -- 大きさ
*/
void Afterimage::Initialize(ParticleManager* argParticleManager, Vector3 pos, float size)
{
	particleManager = argParticleManager;

	this->pos = pos;
	this->size = size;
}
/**
* 実行
*/
bool Afterimage::Run()
{
	particleManager->Entry(
		10,
		pos.x, pos.y, pos.z,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,
		size, 0.5f,
		1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 0.0f
		);
	return false;
}