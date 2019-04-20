/**
* File: Actor.h
*
* ゲームに登場するキャラクターの基底
*
* Date: 2016.11.20
* Author: koshi ito
*/
#pragma once
#include "GameObject.h"


class Actor : public GameObject
{
protected:
	int maxHp_;			// 最大体力
	int hp_;			// 体力
	int attack_;		// 攻撃力(物理)
	int defense_;		// 防御力(物理)

	bool fallFlag_;		// 落下フラグ
	bool damageFlag_;	// ダメージを受けているフラグ
	DirectX::SimpleMath::Vector3 velocity_; // 速度
private:
	static bool soundFlag_;
public:
	// コンストラクタ
	Actor();
	// デストラクタ
	virtual ~Actor();
	// 前進方向へのベクトルを求める
	const DirectX::SimpleMath::Vector3 MoveForward(float speed);
	// 重力を掛ける
	void useGravity();
	// World座標を算出
	const DirectX::SimpleMath::Matrix CreateWorldMatrix();

	// アクセッサ
	// MaxHP
	inline int getMaxHP() { return maxHp_; };
	inline void setMaxHP(int data) { maxHp_ = data; }
	// HP
	inline int getHP() { return hp_; };
	inline void setHP(int data) { hp_ = data; }
	// Attack
	inline int getAttack() { return attack_; }
	inline void setAttack(int data) { attack_ = data; }
	// Defense
	inline int getDefense() { return defense_; }
	inline void setDefense(int data) { defense_ = data; }
	// fallFlag
	inline bool getFall() { return fallFlag_; }
	inline void setFall(bool data) { fallFlag_ = data; }
	// Velocity
	inline const DirectX::SimpleMath::Vector3& getVelocity() { return velocity_; }
	inline void setVelocity(const DirectX::SimpleMath::Vector3& vel) { velocity_ = vel; }
	inline void setVelocityX(const float& speed) { velocity_.x = speed; }
	inline void setVelocityY(const float& speed) { velocity_.y = speed; }
	inline void setVelocityZ(const float& speed) { velocity_.z = speed; }

	// ダメージを受ける
	virtual void ReceiveDamage(Actor* sender, const int& attack, const int& hate = 0);

	static void SoundReset();
};
// 最短角度を求める
float shortAngle(float start, float end);