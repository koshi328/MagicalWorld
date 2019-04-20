/**
* File: Spherical.cpp
* 半球状放射エフェクト
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "Spherical.h"

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
* @param time -- 実行時間
*/
void Spherical::Initialize(ParticleManager* argParticleManager, Vector3 pos, float size, int time)
{
	particleManager = argParticleManager;

	this->pos = pos;
	this->size = size;
	this->time = time;
}
/**
* 実行
*/
bool Spherical::Run()
{
	for (int i = 0; i < 3000; i++)
	{
		Vector3 vel;
		vel.x = randomRange(-1.0f, 1.0f);
		vel.y = randomRange(0.0f, 1.0f);
		vel.z = randomRange(-1.0f, 1.0f);
		vel.Normalize();
		vel *= size;
		particleManager->Entry(
			time,
			pos.x, pos.y, pos.z,
			vel.x / time, vel.y / time, vel.z / time,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			5.0f, 2.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f
			);
	}
	return false;
}