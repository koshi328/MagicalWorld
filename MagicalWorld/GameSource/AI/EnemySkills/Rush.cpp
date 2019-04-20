/**
* File: Rush.cpp
* スキル Rush(訳：突進)
*
* Data:   2017.1.10
* Author: Koshi ito
*/

#include "Rush.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Collision\Collision2D.h"
#include <SimpleMath.h>
#include "..\..\Object\Enemy.h"
#include "..\..\..\Utility\MeshManager.h"
#include "..\..\..\Particle\ParticleManager.h"
#include "..\..\..\Utility\TaskManager.h"
#include "..\..\Effect\Gether.h"
#include "..\..\..\RangeMap\RangeMap.h"

using namespace DirectX::SimpleMath;

extern MeshManager* getMeshManager();
extern ParticleManager* getParticleManager();
extern TaskManager* getTaskManager();
extern float randomRange(float min, float max);

#define LENGTH (200)

/**
* コンストラクタ
* @param subject -> 操作対象オブジェクトのポインタ
*/
Rush::Rush(Enemy* subject)
{
	// 対象オブジェクトを設定
	subject_ = subject;
	castTime_ = 120;
}

/**
* デストラクタ
*/
Rush::~Rush()
{

}

/**
* 初期化
*/
void Rush::Entry()
{
}

/**
* 実行
* @return true  --- 実行中
* @return false --- 実行終了
*/
bool Rush::Execute()
{
	// 対象がいなければ終了
	if (subject_->getTargetInfo().empty()) return false;
	castTime_--;
	if (castTime_ > 60)
	{
		Square square;
		square._position = subject_->Position() + subject_->MoveForward(LENGTH / 2.f);
		square._angle = subject_->Rotation().y;
		square._x = subject_->getCollision()->getMaxRange() * 4;
		square._z = LENGTH;
		RangeMap::getInstance()->Add(&square);

		for (int i = 0; i < 20; i++)
			getTaskManager()->Add<Gether>()->Initialize(getParticleManager(), subject_->Position(),
				subject_->getCollision()->getMaxRange() * subject_->getCollision()->getMaxRange(),
				1.f, 0.f, 0.f, 1.f);
		return true;
	}
	else if(castTime_ < 40)
	{
		subject_->setVelocityX(subject_->MoveForward(LENGTH).x / 40);
		subject_->setVelocityZ(subject_->MoveForward(LENGTH).z / 40);

		Circle range;	// 検知範囲
		range._position = subject_->Position();
		range._radius = subject_->getCollision()->getMaxRange();
		for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Player"))
		{
			Circle tmp;
			tmp._position = object->Position();
			tmp._radius = object->getCollision()->getMaxRange();
			if (Circle2Circle(tmp, range))
			{
				object->ReceiveDamage(subject_, subject_->getAttack());
			}
		}
	}

	if (castTime_ > 0) return true;
	return false;
}

/**
* 終了
*/
void Rush::Exit()
{
}
