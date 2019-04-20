/**
* File: HitEffect.cpp
* ヒットエフェクト
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "HitEffect.h"

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
void HitEffect::Initialize(ParticleManager* argParticleManager, Vector3 pos, float size, int time)
{
	particleManager = argParticleManager;

	this->pos = pos;
	this->size = size;
	this->time = time;
}

/**
* 実行
*/
bool HitEffect::Run()
{
	//一本目
	for (int i = 0; i < size; i++)
	{
		float angle = 45 * 3.14f / 180;
		float x = sinf(angle) * i;
		float y = cosf(angle) * i;
		float scale = (size - i) / size * (size / 6);
		particleManager->Entry(
			time,
			pos.x, pos.y, pos.z,
			x / time, y / time, 0,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			scale, scale,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 0.0f
			);

		x = sinf(angle + 3.14f) * i;
		y = cosf(angle + 3.14f) * i;
		particleManager->Entry(
			time,
			pos.x, pos.y, pos.z,
			x / time, y / time, 0,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			scale, scale,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 0.0f
			);
	}

	// 二本目
	for (int i = 0; i < size; i++)
	{
		float angle = 45 * 3.14f / 180;
		float x = sinf(-angle) * i;
		float y = cosf(angle) * i;
		float z = cosf(angle) * i;
		float scale = (size - i) / size  * (size / 6);
		particleManager->Entry(
			time,
			pos.x, pos.y, pos.z,
			x / time, y / time, z / time,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			scale, scale,
			1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 0.0f
			);

		x = sinf(-angle + 3.14f) * i;
		y = cosf(angle + 3.14f) * i;
		z = cosf(angle + 3.14f) * i;
		particleManager->Entry(
			time,
			pos.x, pos.y, pos.z,
			x / time, y / time, z / time,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			scale, scale,
			1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 0.0f
			);
	}

	for (int i = 0; i < 10; i++)
	{
		float x = sinf(randomRange(-3.14f, 3.14f)) * size;
		float y = cosf(randomRange(-3.14f, 3.14f)) * size;
		float z = cosf(randomRange(-3.14f, 3.14f)) * size;
		particleManager->Entry(
			time,
			pos.x, pos.y, pos.z,
			x / time, y / time, z / time,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			2.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 0.0f
			);
	}
	return false;
}