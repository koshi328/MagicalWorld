/**
* File: RollAttack.cpp
* スキル RollAttack(訳：回転攻撃)
*
* Data:   2017.1.10
* Author: Koshi ito
*/

#include "RollAttack.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Collision\Collision2D.h"
#include "..\..\Object\Enemy.h"
#include "..\..\..\Utility\MeshManager.h"
#include "..\..\..\Particle\ParticleManager.h"
#include "..\..\..\Utility\TaskManager.h"
#include "..\..\Effect\CircleEffect.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

extern MeshManager* getMeshManager();
extern ParticleManager* getParticleManager();
extern TaskManager* getTaskManager();

#define CAST_TIME (120)

/**
* コンストラクタ
* @param subject -> 操作対象オブジェクトのポインタ
*/
RollAttack::RollAttack(Enemy* subject)
{
	// 対象オブジェクトを設定
	subject_ = subject;
	castTime_ = CAST_TIME;
	saveAngle_ = subject->Rotation().y;
}

/**
* デストラクタ
*/
RollAttack::~RollAttack()
{

}

/**
* 初期化
*/
void RollAttack::Entry()
{
}

/**
* 実行
* @return true  --- 実行中
* @return false --- 実行終了
*/
bool RollAttack::Execute()
{
	castTime_--;
	if (castTime_ > 30) return true;
	float angle = 360.f * 3.14f / 180.f / (CAST_TIME - 90);
	Vector3 rot = subject_->Rotation();
	angle = angle * ((CAST_TIME - 90) - castTime_) + saveAngle_;
	rot.y = angle;
	subject_->Rotation(rot);
	if (castTime_ > 0) return true;
	Circle range;	// 検知範囲用
	range._position = subject_->Position();
	range._radius = 50.0f;
	for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Player"))
	{
		Circle tmp;
		tmp._position = object->Position();
		tmp._radius = object->getCollision()->getMaxRange();
		if (Circle2Circle(tmp, range))
		{
			object->ReceiveDamage(subject_, subject_->getAttack() / 10);
		}
	}
	return false;
}

/**
* 終了
*/
void RollAttack::Exit()
{
}
