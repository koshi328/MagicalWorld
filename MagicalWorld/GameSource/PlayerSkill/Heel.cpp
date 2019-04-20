/**
* File: heel.cpp
*
* 対象を回復させるスキル
*
* Date: 2016.12.13
* Author: koshi ito
*/
#include "Heel.h"
#include "..\Object\Actor.h"
#include <SimpleMath.h>
#include "..\..\Particle\ParticleManager.h"
#include "..\..\Utility\TaskManager.h"
#include "..\Effect\HeelEffect.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"
#include "..\..\Sound\ADX2Le.h"
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
* コンストラクタ
* @param subject -- 対象Actorのポインタ
*/
Heel::Heel(Actor* subject)
{
	subject_ = subject;
	recastTime_ = 120;
}
/**
* デストラクタ
*/
Heel::~Heel()
{

}
/**
* 登録
*/
void Heel::Entry()
{

}

/**
* 実行
* @return(bool) 実行中はtrue,終了時にfalseを返す
*/
bool Heel::Execute()
{
	subject_->setHP(subject_->getHP() + subject_->getMaxHP() / 10);
	if (subject_->getMaxHP() < subject_->getHP()) subject_->setHP(subject_->getMaxHP());

	getTaskManager()->Add<HeelEffect>()->Initialize(getParticleManager(), subject_->Position());
	ADX2Le::Play(CRI_CUESHEET_0_SE_HEEL);
	recastTime_ = 120;
	return false;
}
/**
* 終了
*/
void Heel::Exit()
{

}