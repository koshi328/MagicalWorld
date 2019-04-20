/**
* File: HeelEffect.cpp
* 回復エフェクト
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "HeelEffect.h"

using namespace DirectX::SimpleMath;
/**
* グローバル関数
*/
float randomRange(float min, float max);
/**
* 初期化
* @param argParticleManager -- ParticleManagerのポインタ
* @param pos  -- 発生位置
*/
void HeelEffect::Initialize(ParticleManager* argParticleManager, Vector3 pos)
{
	particleManager = argParticleManager;

	this->pos = pos;

}
/**
* 実行
*/
bool HeelEffect::Run()
{
	for (int i = 0; i < 180; i++)
	{
		float x = sinf((i * 2) * 3.14f / 180);
		float z = cosf((i * 2) * 3.14f / 180);
		particleManager->Entry(
			30,
			pos.x, pos.y, pos.z,
			x, 0.0f, z,
			0.0f, 0.0f, 0.0f,
			0.0f, 0.0f,
			3.0f, 0.3f,
			0, 1.0f, 0.0f, 1,
			0.5f, 1.0f, 0.0f, 0.4f
			);

		x = sinf(randomRange(-3.14f, 3.14f)) * randomRange(0.0f, 12.0f);
		z = cosf(randomRange(-3.14f, 3.14f)) * randomRange(0.0f, 12.0f);
		particleManager->Entry(
			(int)randomRange(40, 50),
			pos.x, pos.y, pos.z,
			-x / 50.f, 0.5f, -z / 50.f,
			0.0f, (fabs(x) + fabs(z)) * -0.0003f - 0.01f, 0.0f,
			0.0f, 0.0f,
			1.0f, 0.1f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.4f, 1.0f, 0.0f, 1.0f
			);
	}
	return false;
}