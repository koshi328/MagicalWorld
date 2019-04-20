/**
* File: Item.h
*
* アイテムクラス
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once
#include "..\Collision\Collision.h"
#include "Actor.h"
class Player;

class Item : public Actor
{
private:
	Sphere collision_;	// コライダー
	Player* owner_;		// 取得者
	int lifeTime_;		// 生存時間
	int type_;			// アイテムの種類
public:
	// コンストラクタ
	Item(DirectX::SimpleMath::Vector3 position, int lifeTime);
	// デストラクタ
	~Item();

	// 衝突判定へのアクセス
	Collision* getCollision();
	// アクセッサ
	inline const int& getType() { return type_; }
	inline void setOwner(Player* owner) {if(!owner_) owner_ = owner; }
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// 描画
	void Draw();
};