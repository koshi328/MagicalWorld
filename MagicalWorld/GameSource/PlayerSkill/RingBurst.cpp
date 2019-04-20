/**
* File: RingBurst.cpp
*
* スキル：爆発
*
* Date: 2017.1.17
* Author: koshi ito
*/

#include "RingBurst.h"
#include "..\Object\Actor.h"
#include "..\Collision\Collision2D.h"
#include "..\Object\ObjectManager.h"
#include "..\..\Utility\TaskManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "..\Effect\Spherical.h"
#include "..\Effect\Radiation.h"
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
/**
* 定数宣言
*/
#define RANGE (150)
/**
* コンストラクタ
* @param subject -- 対象Actorのポインタ
*/
RingBurst::RingBurst(Actor* subject)
{
	subject_ = subject;
	recastTime_ = 30;
	castTime_ = 90;
}
/**
* デストラクタ
*/
RingBurst::~RingBurst()
{

}
/**
* 登録
*/
void RingBurst::Entry()
{
	// エフェクトの登録
	getTaskManager()->Add<Radiation>()->Initialize(getParticleManager(), subject_->Position(), RANGE);
}

/**
* 実行
* @return(bool) 実行中はtrue,終了時にfalseを返す
*/
bool RingBurst::Execute()
{
	if (--castTime_ > 0) return true;
	getTaskManager()->Add<Spherical>()->Initialize(getParticleManager(), subject_->Position(), RANGE, 60);
	ADX2Le::Play(CRI_CUESHEET_0_SE_WATER02);
	// 範囲を生成
	Circle range;
	range._position = subject_->Position();
	range._radius = RANGE;
	// 範囲内判定
	for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Enemy"))
	{
		Circle tmp;
		tmp._position = object->Position();
		tmp._radius = object->getCollision()->getMaxRange();
		if (Circle2Circle(tmp, range))
		{
			//ダメージメッセージを送信
			object->ReceiveDamage(subject_, subject_->getAttack() * 5);
		}
	}

	return false;
}
/**
* 終了
*/
void RingBurst::Exit()
{

}