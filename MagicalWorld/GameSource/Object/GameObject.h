/**
* File: GameObject.h
*
* ゲームに登場するオブジェクトの基底クラス
*
* Date: 2016.10.15
* Author: koshi ito
*/
#pragma once
#include <Simplemath.h>
class Collision;

class GameObject
{
protected:
	DirectX::SimpleMath::Vector3 position_;	// 座標
	DirectX::SimpleMath::Vector3 rotation_;	// 回転
	DirectX::SimpleMath::Vector3 scale_;	// 拡縮

	// オブジェクトの消滅フラグ
	bool isDestroy;
public:
	// コンストラクタ
	GameObject();
	// デストラクタ
	virtual ~GameObject();

	// アクセッサ
	DirectX::SimpleMath::Vector3& Position() { return position_; }
	void Position(DirectX::SimpleMath::Vector3& position) { position_ = position; }
	DirectX::SimpleMath::Vector3& Rotation() { return rotation_; }
	void Rotation(DirectX::SimpleMath::Vector3& rotation) { rotation_ = rotation; }
	DirectX::SimpleMath::Vector3& Scale() { return scale_; }
	void Scale(DirectX::SimpleMath::Vector3& scale) { scale_ = scale; }

	// 衝突判定へのアクセス
	virtual Collision* getCollision() { return nullptr; }

	// オブジェクトの消去
	bool DestroyMessage() { return isDestroy; }
	void Destroy(GameObject* p) { p->isDestroy = true; }
	// 初期化
	virtual void Initialize() = 0;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() = 0;
};