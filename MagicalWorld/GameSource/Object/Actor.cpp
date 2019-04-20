/**
* File: Actor.cpp
*
* ゲームに登場するキャラクターの基底
*
* Date: 2016.11.20
* Author: koshi ito
*/

#include "Actor.h"
#include <SimpleMath.h>
#include "..\Camera\CameraManager.h"
#include "..\Camera\Camera.h"
#include "..\..\Utility\TaskManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "..\Effect\MessageUI.h"
#include "..\Effect\HitEffect.h"
#include "Player.h"
#include "Enemy.h"
// サウンド
#include "..\..\Sound\ADX2Le.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"

/**
* 名前空間
*/
using namespace DirectX::SimpleMath;
/**
* グローバル関数
*/
extern TaskManager* getTaskManager();
extern ParticleManager* getParticleManager();

/**
* static変数
*/
bool Actor::soundFlag_ = false;

/**
* 最短角度を求める
* @param start -- 現在の角度
* @param end   -- 目標角度
* @return(float) -- 最短角度
*/
float shortAngle(float start, float end)
{
	float angle = (end - start) * 180.0f / 3.14f;
	if (angle >= 180.0f)
	{
		angle -= 360.0f;
	}
	return angle * 3.14f / 180.0f;
}
/**
* コンストラクタ
*/
Actor::Actor()
{
}
/**
* デストラクタ
*/
Actor::~Actor()
{
}

/**
* 前方移動
* @param speed -- 移動速度
* @return(Vector3) 移動量
*/
const Vector3 Actor::MoveForward(float speed)
{
	// 回転量
	Matrix rot = Matrix::CreateRotationY(rotation_.y);
	// 移動量
	Vector3 vel(0, 0, speed);
	vel = Vector3::Transform(vel, rot);
	return vel;
}

/**
* 落下処理
* @param 無し
* @return(float) 重力による加速度
*/
void Actor::useGravity()
{
	if (!fallFlag_)
		velocity_.y = 0.0f;
	else
	{
		if (velocity_.y >= -3.0f)
			velocity_.y -= 0.098f;
	}
}

/**
* ダメージを受ける処理
* @param sender -- 攻撃を加えた者
* @param attack -- 攻撃力
* @param hate   -- 攻撃によって増える敵視
*/
void Actor::ReceiveDamage(Actor* sender, const int& attack, const int& hate)
{
	if (damageFlag_) return;
	velocity_.y = 2.0f;
	position_.y += velocity_.y;
	fallFlag_ = true;
	damageFlag_ = true;
	int damage = (attack - defense_);
	if (damage < 0) damage = 1;
	damage += (int)(damage * ((rand() / 100) * 0.01f));

	hp_ -= damage;
	if (hp_ <= 0) hp_ = 0;

	// ダメージ文字の表示登録
	Camera* camera = CameraManager::getInstance()->getCamera();
	wchar_t buf[32];
	swprintf_s(buf, 32, L"%d", damage);

	if (Player* p = dynamic_cast<Player*>(sender))
		getTaskManager()->Add<MessageUI>()->Initialize(camera->GetViewmat(), camera->GetProjmat(), 1280, 720, this->Position(), 120, buf, Color(1, 1, 0, 1));
	if (Enemy* p = dynamic_cast<Enemy*>(sender))
		getTaskManager()->Add<MessageUI>()->Initialize(camera->GetViewmat(), camera->GetProjmat(), 1280, 720, this->Position(), 120, buf, Color(1, 0, 0, 1));
	getTaskManager()->Add<HitEffect>()->Initialize(getParticleManager(), position_, getCollision()->getMaxRange() * 5, 20);
	if(!soundFlag_)
		ADX2Le::Play(CRI_CUESHEET_0_SE_HIT);
	soundFlag_ = true;
}

/**
* World行列を生成する
* @return(Matrix) -- World行列
*/
const DirectX::SimpleMath::Matrix Actor::CreateWorldMatrix()
{
	Matrix world = Matrix::Identity;
	Matrix t = Matrix::CreateTranslation(position_);
	Matrix r = Matrix::CreateFromYawPitchRoll(rotation_.y, rotation_.x, rotation_.z);
	Matrix s = Matrix::CreateScale(scale_);
	world = s * r * t;
	return world;
}

void Actor::SoundReset()
{
	soundFlag_ = false;
}