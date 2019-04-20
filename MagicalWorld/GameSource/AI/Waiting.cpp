/**
* File: Waiting.cpp
* 行動状態：攻撃対象を探して動き回る
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#include "Waiting.h"
#include <SimpleMath.h>
#include "Follow.h"

using namespace std;
using namespace DirectX::SimpleMath;
/**
* コンストラクタ
* @param subject -> このクラスの行動を実行させたいオブジェクト(Enemy*)
*/
Waiting::Waiting(Enemy* subject)
:subject_(subject)
{
	count_ = 0;
	rotation_ = 0.f;
}
/**
* デストラクタ
*/
Waiting::~Waiting()
{

}
/**
* 初期化処理
* @param 無し
* @return(void)
*/
void Waiting::Entry()
{
	count_ = 0;
	rotation_ = 0.f;
}
/**
* 敵意リスト一位のオブジェクトを追従する
* @param 無し
* @return(void)
*/
void Waiting::Execute()
{
	count_++;
	if (count_ >= 60)
	{
		count_ = 0;
		rotation_ = (rand() % 180 - 180) * 1.f;
		rotation_ = rotation_ * 3.14f / 180;
	}
	Vector3 rot = subject_->Rotation();
	rot.y = shortAngle(rot.y, rotation_);
	subject_->Rotation(rot);
	subject_->setVelocity(subject_->MoveForward(1.f));
	for each (TARGET* target in subject_->getTargetInfo())
	{
		if (target->hate != 0)
		{
			subject_->ChangeState(new Follow(subject_));
			return;
		}
	}
}
/**
* 終了処理
* @param 無し
* @return(void)
*/
void Waiting::Exit()
{

}
