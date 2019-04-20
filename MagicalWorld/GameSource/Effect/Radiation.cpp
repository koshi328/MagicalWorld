/**
* File: Radiation.cpp
* 放射エフェクト
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "Radiation.h"

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
void Radiation::Initialize(ParticleManager* argParticleManager, Vector3 pos, int size)
{
	particleManager = argParticleManager;

	this->pos = pos;
	this->size = size;
}
/**
* 実行
*/
bool Radiation::Run()
{
	for (int i = 0; i < 8; i++)
	{
		float x = sinf((360.f / 8 * i) * 3.14f / 180);
		float z = cosf((360.f / 8 * i) * 3.14f / 180);
		for (int j = 0; j < 10; j++)
		{
			particleManager->Entry(
				size,
				pos.x, pos.y + j, pos.z,
				x, 0.f, z,
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f,
				5.0f, 5.0f,
				1.0f, 0.0f, 0.0f, 1.0f,
				0.0f, 1.0f, 1.0f, 0.0f
				);
		}
	}
	return false;
}