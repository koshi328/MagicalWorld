/**
* File: Swing.cpp
*
* スキル：近接格闘（3段階攻撃）
*
* Date: 2016.12.1
* Author: koshi ito
*/
#include "Swing.h"
#include "..\..\Utility\Mouse.h"
#include "..\Object\Actor.h"
#include "..\Collision\Collision2D.h"
#include "..\Object\ObjectManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "..\..\Utility\TaskManager.h"
#include "..\Effect\Afterimage.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"
#include "..\..\Sound\ADX2Le.h"
#include <SimpleMath.h>
/**
* 名前空間
*/
using namespace DirectX::SimpleMath;
/**
* グローバル関数
*/
extern ParticleManager* getParticleManager();
extern TaskManager* getTaskManager();
/**
* 定数宣言
*/
#define CAST_TIME (10)
#define RECAST_TIME (30)
#define ATTACK_RANGE (20.0f)
#define NEXT_ATTACK_ENTRY_TIME (30)
/**
* コンストラクタ
* @param subject -- 対象Actorのポインタ
*/
Swing::Swing(Actor* subject)
{
	subject_ = subject;
	recastTime_ = RECAST_TIME;
	castTime_ = CAST_TIME + NEXT_ATTACK_ENTRY_TIME;
	angle_ = 0.f;
}
/**
* デストラクタ
*/
Swing::~Swing()
{

}
/**
* 登録
*/
void Swing::Entry()
{
	// 段階によって初期化方法を変える
	switch (attackNum_)
	{
	case 0:
		FirstInit();
		break;
	case 1:
		SecondInit();
		break;
	case 2:
		ThirdInit();
		break;
	default:
		break;
	}
}

/**
* 実行
* @return(bool) 実行中はtrue,終了時にfalseを返す
*/
bool Swing::Execute()
{
	// 技の時間を計測
	castTime_--;
	// キャストタイムの間エフェクトを生成する
	if (castTime_ >= NEXT_ATTACK_ENTRY_TIME)
	{
		// 段階によって別の更新処理を行う
		switch (attackNum_)
		{
		case 0:
			FirstUpdate();
			break;
		case 1:
			SecondUpdate();
			break;
		case 2:
			ThirdUpdate();
			break;
		default:
			break;
		}
		Vector3 tmp = subject_->Rotation();
		tmp.x = 30.f * 3.14f / 180;
		subject_->Rotation(tmp);
	}
	// 追撃入力を受け付ける時間まではここで終了
	if (castTime_ > NEXT_ATTACK_ENTRY_TIME) return true;
	// 追撃入力の受付
	else if (castTime_ < NEXT_ATTACK_ENTRY_TIME)
	{
		if (Mouse::getInstance()->Touch(Mouse::LEFT))
		{		
			attackNum_++;
			Entry();
		}
	}
	// ダメージ発生フェーズ
	else
	{
		FanShape range;	// 検知範囲
		range._position = subject_->Position();
		range._radius = ATTACK_RANGE;
		range._forward = subject_->MoveForward(1.0f);
		range._rangeAngle = 180.f;
		for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Enemy"))
		{
			Circle tmp;
			tmp._position = object->Position();
			tmp._radius = object->getCollision()->getMaxRange();
			if (Circle2FanFhape(tmp, range))
			{
				//ダメージメッセージを送信
				object->ReceiveDamage(subject_, (subject_->getAttack() * (int)(1.f + attackNum_ * 0.5f)));
			}
		}
	}
	// 追撃入力がないまま０になったら終了する
	if (castTime_ > 0) return true;
	return false;
}
/**
* 終了処理
*/
void Swing::Exit()
{
	recastTime_ = RECAST_TIME;
	attackNum_ = 0;
}

/**
* 最初の攻撃の初期化
*/
void Swing::FirstInit()
{
	castTime_ = CAST_TIME + NEXT_ATTACK_ENTRY_TIME;
	angle_ = subject_->Rotation().y - (135 * 3.14f / 180.f);
	ADX2Le::Play(CRI_CUESHEET_0_SE_DASH);
}
/**
* 2発目の追撃を行う為の初期化
*/
void Swing::SecondInit()
{
	castTime_ = CAST_TIME + NEXT_ATTACK_ENTRY_TIME;
	angle_ = subject_->Rotation().y + (135 * 3.14f / 180.f);
	ADX2Le::Play(CRI_CUESHEET_0_SE_DASH);
}
/**
* 3発目の追撃を行う為の初期化
*/
void Swing::ThirdInit()
{
	castTime_ = CAST_TIME + NEXT_ATTACK_ENTRY_TIME;
	angle_ = 0.f;
	ADX2Le::Play(CRI_CUESHEET_0_SE_DASH);
}
/**
* 1撃目の更新処理
*/
void Swing::FirstUpdate()
{
	float oneFlame = 180 * 3.14f / 180.f / CAST_TIME;
	int loop = 10;
	int i;
	for (i = 0; i < loop; i++)
	{
		angle_ += oneFlame / loop;
		Vector3 pos = Vector3(sinf(angle_) * ATTACK_RANGE, 0.f, cosf(angle_) * ATTACK_RANGE);
		getTaskManager()->Add<Afterimage>()->Initialize(getParticleManager(), subject_->Position() + pos, 5.0f);
	}
	subject_->Position(subject_->Position() + subject_->MoveForward(1.f));
}
/**
* 2撃目の更新処理
*/
void Swing::SecondUpdate()
{
	float oneFlame = 180 * 3.14f / 180.f / CAST_TIME;
	int loop = 10;
	int i;
	for (i = 0; i < loop; i++)
	{
		angle_ -= oneFlame / loop;
		Vector3 pos = Vector3(sinf(angle_) * ATTACK_RANGE, 0.f, cosf(angle_) * ATTACK_RANGE);
		getTaskManager()->Add<Afterimage>()->Initialize(getParticleManager(), subject_->Position() + pos, 5.0f);
	}
	subject_->Position(subject_->Position() + subject_->MoveForward(1.f));
}
/**
* 3撃目の更新処理
*/
void Swing::ThirdUpdate()
{
	float oneFlame = 180 * 3.14f / 180.f / CAST_TIME;
	int loop = 10;
	int i;
	for (i = 0; i < loop; i++)
	{
		angle_ += oneFlame / loop;
		Vector3 pos = Vector3(sinf(subject_->Rotation().y + angle_) * ATTACK_RANGE, 0.f, cosf(subject_->Rotation().y + angle_) * ATTACK_RANGE);
		getTaskManager()->Add<Afterimage>()->Initialize(getParticleManager(), subject_->Position() + pos, 5.0f);

		pos = Vector3(sinf(subject_->Rotation().y - angle_) * ATTACK_RANGE, 0.f, cosf(subject_->Rotation().y - angle_) * ATTACK_RANGE);
		getTaskManager()->Add<Afterimage>()->Initialize(getParticleManager(), subject_->Position() + pos, 5.0f);
	}
	subject_->Position(subject_->Position() + subject_->MoveForward(3.0f));
}