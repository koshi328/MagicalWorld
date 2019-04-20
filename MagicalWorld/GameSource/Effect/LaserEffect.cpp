/**
* File: LaserEffect.cpp
* レーザーエフェクト
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "LaserEffect.h"

using namespace DirectX::SimpleMath;
/**
* グローバル関数
*/
float randomRange(float min, float max);
/**
* 初期化
* @param argParticleManager -- ParticleManagerのポインタ
* @param pos    -- 発生位置
* @param size   -- 太さ
* @param length -- 長さ
* @param angle  -- 向き
*/
void LaserEffect::Initialize(ParticleManager* argParticleManager, Vector3 pos, float size, float length, float angle)
{
	particleManager = argParticleManager;

	this->pos = pos;
	this->size = size;
	this->angle = angle;
	this->length = length;
}
/**
* 実行
*/
bool LaserEffect::Run()
{
	float x = sinf(angle);
	float z = cosf(angle);
	for (int i = 0; i < length; i++)
	{
		particleManager->Entry(
			30,
			pos.x + x * i, pos.y, pos.z + z * i,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			size, 0.0f,
			1.0f, 0.5f, 0.5f, 1.0f,
			1.0f, 1.0f, 1.0f, 0.0f
			);
	}
	return false;
}