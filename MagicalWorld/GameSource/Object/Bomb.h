/**
* File: Bomb.h
*
* 爆弾クラス
*
* Date: 2016.12.25
* Author: koshi ito
*/
#pragma once
#include "..\Collision\Collision.h"
#include "Actor.h"

class Bomb : public Actor
{
private:
	Sphere collision_;	// 衝突判定コライダー
	int lifeTime_;		// 生存可能時間
	int effectDilay_;	// エフェクトの間隔
public:
	Bomb(DirectX::SimpleMath::Vector3 position, int lifeTime);
	~Bomb();

	// 衝突判定へのアクセス
	Collision* getCollision();

	void Initialize();
	void Update();
	void Draw();
};