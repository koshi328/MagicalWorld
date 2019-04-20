/**
* File: UseSkill.cpp
* 行動状態：スキルを使用する
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#include "UseSkill.h"
#include "Follow.h"
#include "..\Object\Enemy.h"
#include <SimpleMath.h>

// スキル一覧
#include "EnemySkills\HammerOfAnger.h"
#include "EnemySkills\Cleave.h"
#include "EnemySkills\RollAttack.h"
#include "EnemySkills\BombSetting.h"
#include "EnemySkills\Rush.h"

using namespace std;
using namespace DirectX::SimpleMath;

/**
* コンストラクタ
* @param subject -- 操作対象
*/
UseSkill::UseSkill(Enemy* subject)
	:subject_(subject), skill_(nullptr)
{
	
}

/**
* デストラクタ
*/
UseSkill::~UseSkill()
{
	if (skill_)
		delete skill_;
}

/**
* 初期化
*/
void UseSkill::Entry()
{
	subject_->setUseSkillID((subject_->getUseSkillID() + 1) % (subject_->getSkillList().size()));
	skill_ = createSkill(subject_->getSkillList()[subject_->getUseSkillID()]);
	if(skill_)
		skill_->Entry();
}

/**
* 実行
*/
void UseSkill::Execute()
{
	// スキルの実行が終了するまで呼び出す
	if (skill_->Execute()) return;
	// ディレイタイムを設定する
	subject_->setSkillDelay(skill_->getDelayTime());
	// この行動を終了する
	subject_->ChangeState(new Follow(subject_));
}

/**
* 終了
*/
void UseSkill::Exit()
{
	// 使用したスキルの破棄
	skill_->Exit();
	delete skill_;
	skill_ = nullptr;
}

/**
* スキルを生成する関数
* @param id -- 生成したいスキルのID
* @return(EnemySkill*) -- 生成したスキル
*/
EnemySkill* UseSkill::createSkill(int id)
{
	EnemySkill* tmp = nullptr;
	switch (id)
	{
	case 0:
		tmp = new HammerOfAnger(subject_);
		break;
	case 1:
		tmp = new RollAttack(subject_);
		break;
	case 2:
		tmp = new BombSetting(subject_);
		break;
	case 3: tmp = new Rush(subject_);
		break;
	}
	return tmp;
}