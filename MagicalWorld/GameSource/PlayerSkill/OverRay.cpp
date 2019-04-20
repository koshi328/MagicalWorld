/**
* File: OverRay.cpp
*
* スキル：レーザー
*
* Date: 2017.1.17
* Author: koshi ito
*/

#include "OverRay.h"
#include "..\Object\Actor.h"
#include "..\Collision\Collision2D.h"
#include "..\Object\ObjectManager.h"
#include "..\..\Utility\TaskManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "..\Effect\Gether.h"
#include "..\Effect\LaserEffect.h"
#include "..\..\Utility\MeshManager.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"
#include "..\..\Sound\ADX2Le.h"
/**
* 名前空間
*/
using namespace DirectX::SimpleMath;
/**
* グローバル関数
*/
extern TaskManager* getTaskManager();
extern ParticleManager* getParticleManager();
extern MeshManager* getMeshManager();

/**
* コンストラクタ
* @param subject -- 対象Actorのポインタ
*/
OverRay::OverRay(Actor* subject)
{
	subject_ = subject;
	recastTime_ = 30;
	castTime_ = 90;
}
/**
* デストラクタ
*/
OverRay::~OverRay()
{

}
/**
* 登録
*/
void OverRay::Entry()
{

}

/**
* 実行
* @return(bool) 実行中はtrue,終了時にfalseを返す
*/
bool OverRay::Execute()
{
	// 範囲の設定
	Square range;	// 検知範囲
	range._angle = subject_->Rotation().y;
	range._x = 100.f;
	range._z = 600.f;
	range._position = subject_->Position() + subject_->MoveForward(range._z / 2);

	// エフェクト
	getTaskManager()->Add<Gether>()->Initialize(getParticleManager(), subject_->Position(), 50.f, 1.f, 0.5f, 0.0f, 1.0f);
	if (--castTime_ > 0) return true;
	getTaskManager()->Add<LaserEffect>()->Initialize(getParticleManager(), subject_->Position(), 100.f, 400.f, subject_->Rotation().y);
	ADX2Le::Play(CRI_CUESHEET_0_SE_WATER01);
	// 範囲内判定
	for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Enemy"))
	{
		Circle tmp;
		tmp._position = object->Position();
		tmp._radius = object->getCollision()->getMaxRange();
		if (Circle2Square(tmp, range))
		{
			//ダメージメッセージを送信
			object->ReceiveDamage(subject_, subject_->getAttack() * 10);
		}
	}
	return false;
}
/**
* 終了
*/
void OverRay::Exit()
{

}