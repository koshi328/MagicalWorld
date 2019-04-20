/**
* File: Follow.cpp
* 行動状態：対象を追跡する
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#include "Follow.h"
#include <SimpleMath.h>
#include "Waiting.h"
#include "UseSkill.h"

using namespace DirectX::SimpleMath;
/**
* コンストラクタ
* @param subject -> このクラスの行動を実行させたいオブジェクト(Enemy*)
*/
Follow::Follow(Enemy* subject)
:subject_(subject)
{

}

/**
* デストラクタ
*/
Follow::~Follow()
{

}

/**
* 初期化処理
* @param 無し
* @return(void)
*/
void Follow::Entry()
{

}

/**
* 敵意リスト一位のオブジェクトを追従する
* @param 無し
* @return(void)
*/
void Follow::Execute()
{
	// 最も敵視しているオブジェクトの座標を取得
	Vector3 tophatePosition = subject_->Position();
	int topHate = 0;
	for each (TARGET* target in subject_->getTargetInfo())
	{
		if (topHate < target->hate)
		{
			topHate = target->hate;
			tophatePosition = target->target->Position();
		}
	}
	// 狙う敵が見つからなかった場合
	if (topHate == 0)
	{
		subject_->ChangeState(new Waiting(subject_));
		return;
	}
	// 座標から角度を算出
	Vector3 length = tophatePosition - subject_->Position();
	float angle = atan2(length.x, length.z);
	// 角度を適用
	Vector3 rot = subject_->Rotation();
	rot.y = angle;
	subject_->Rotation(rot);
	
	if (length.Length() >= subject_->getCollision()->getMaxRange() * 2.0f)
	{
		subject_->setVelocityX(subject_->MoveForward(1.0f).x);
		subject_->setVelocityZ(subject_->MoveForward(1.0f).z);
	}


	if (subject_->getSkillList().empty()) return;
	// スキルディレイが０ならスキルを使用する
	if (subject_->getSkillDelay() == 0)
	{
		subject_->ChangeState(new UseSkill(subject_));
	}
}

/**
* 終了処理
* @param 無し
* @return(void)
*/
void Follow::Exit()
{

}
