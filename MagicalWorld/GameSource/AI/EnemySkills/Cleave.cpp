/**
* File: Clave.cpp
* スキル Clave(訳：鉄槌)
*
* Data:   2017.1.10
* Author: Koshi ito
*/
#include "Cleave.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Collision\Collision2D.h"
#include <SimpleMath.h>
#include "..\..\Object\Enemy.h"
#include "..\..\..\Utility\MeshManager.h"
#include "..\..\..\Particle\ParticleManager.h"
#include "..\..\..\Utility\TaskManager.h"
#include "..\..\Effect\CircleEffect.h"

using namespace DirectX::SimpleMath;
/**
* グローバル関数
*/
extern MeshManager* getMeshManager();
extern ParticleManager* getParticleManager();
extern TaskManager* getTaskManager();

/**
* コンストラクタ
* @param subject -> 操作対象オブジェクトのポインタ
*/
Cleave::Cleave(Enemy* subject)
{
	// 対象オブジェクトを設定
	subject_ = subject;
	castTime_ = 120;
	delayTime_ = 30;
	angle_ = 0.0f;
}

/**
* デストラクタ
*/
Cleave::~Cleave()
{

}

/**
* 初期化
*/
void Cleave::Entry()
{
}

/**
* 実行
* @return true  --- 実行中
* @return false --- 実行終了
*/
bool Cleave::Execute()
{
	if (--effectDilay_ <= 0)
	{
		effectDilay_ = 60;
		Vector3 p = subject_->Position();
		p.y -= subject_->getCollision()->getMaxRange() / 2;
		getTaskManager()->Add<CircleEffect>()->Initialize(getParticleManager(), p, 50);
	}
	// スキル発動準備の処理
	if (castTime_ <= 40) angle_ += 360 * 3.14f / 180 / 40;
	Matrix Lworld = Matrix::Identity;
	Matrix Rworld = Matrix::Identity;
	Matrix Ltrans = Matrix::CreateTranslation(Vector3(2, 0, 0));
	Matrix Rtrans = Matrix::CreateTranslation(Vector3(-2, 0, 0));
	Matrix rot = Matrix::CreateRotationY(angle_);
	Matrix scale = Matrix::CreateScale(0.4f);
	Lworld = scale * Ltrans * rot * subject_->CreateWorldMatrix();
	Rworld = scale * Rtrans * rot * subject_->CreateWorldMatrix();

	getMeshManager()->Entry(2, Lworld);
	getMeshManager()->Entry(2, Rworld);

	if (--castTime_ >= 0) return true;
	// スキル発動
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
			object->ReceiveDamage(subject_, subject_->getAttack());
		}
	}
	return false;
}

/**
* 終了
*/
void Cleave::Exit()
{
}
