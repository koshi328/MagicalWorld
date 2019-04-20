/**
* File: SkillController.cpp
*
* プレイヤーのスキルの実行を管理するクラス
*
* Date: 2016.12.5
* Author: koshi ito
*/

#include "SkillController.h"
#include "Skill.h"
#include "..\..\Utility\Mouse.h"
#include "..\..\Sound\ADX2Le.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"
// スキル派生クラスのインクルード
#include "Swing.h"
#include "Heel.h"
#include "OverRay.h"
#include "RingBurst.h"
/**
* コンストラクタ
* @param subject -- 対象Actorのポインタ
*/
SkillController::SkillController(Actor* subject)
{
	nowSkillNum_ = 0;
	ZeroMemory(skills_, sizeof(Skill*) * SKILL_NUM);
	Cursor_ = 0;
	subject_ = subject;
	executeSkill_ = -1;
	setSkill(10, 0);
}
/**
* デストラクタ
*/
SkillController::~SkillController()
{
	for (int i = 0; i < SKILL_NUM; i++)
	{
		if (!skills_[i]) continue;
		delete skills_[i];
	}
}
/**
* 指定スロットに指定のスキルを設定する
* @param number -- スロットの番号
* @param id     -- 登録したいスキルのID
*/
void SkillController::setSkill(int number, int id)
{
	if (skills_[number])
	{
		delete skills_[number];
	}
	skills_[number] = createSkill(id);
}
/**
* 指定スロットのスキルを実行
* @param number -- 実行したいスロットの番号
*/
void SkillController::setExecute(int number)
{
	executeSkill_ = number;
	// 指定した番号にスキルが無かった時
	if (!skills_[executeSkill_])
	{
		executeSkill_ = -1;
		return;
	}
	skills_[executeSkill_]->Entry();
}
/**
* 指定スロットのスキルIDの取得
* @param number -- スロットの番号
* @return const int& -- スキルのID
*/
const int& SkillController::getID(int number)
{
	if (!skills_[number]) return -1;
	return skills_[number]->getID();
}
/**
* 指定IDのスキルを生成して返す
* @param id      -- スキルID
* @return Skill* -- 生成されたスキルのポインタ
*/
Skill* SkillController::createSkill(int id)
{
	Skill* skill = nullptr;
	switch (id)
	{
	case 0:
		skill = new Swing(subject_);
		break;
	case 1:
		skill = new Heel(subject_);
		break;
	case 2:
		skill = new RingBurst(subject_);
		break;
	case 3:
		skill = new OverRay(subject_);
		break;
	default:
		skill = new Swing(subject_);
		break;
	}
	skill->setID(id);
	return skill;
}

/**
* カーソルの位置を取得
*/
int SkillController::getCursor()
{
	return Cursor_;
}
/**
* 指定番号のスキルを取得
* @param  number -- スロットの番号
* @return Skill* -- 指定スロットのスキルのポインタ
*/
Skill* SkillController::getSkill(const int& number)
{
	return skills_[number];
}
/**
* スキルスロットを指定位置から左詰めにする
* @param begin -- ソートの基準位置
*/
void SkillController::sortSkill(int begin)
{
	int i;
	for (i = begin; i < SKILL_NUM - 2; i++)
	{
		skills_[i] = skills_[i + 1];
	}
	skills_[SKILL_NUM - 2] = nullptr;
}
/**
* 取得しているスキルの数を取得
* @return int -- スキルの数
*/
int SkillController::getNowSkillNum()
{
	return nowSkillNum_;
}
/**
* スキルの数を設定する
*/
void SkillController::setNowSkillNum(const int& num)
{
	nowSkillNum_ = num;
}

/**
* 実行
*/
bool SkillController::Run()
{
	int i;
	// リキャストタイムの計算
	for (i = 0; i < SKILL_NUM; i++)
	{
		if (!skills_[i]) continue;
		int recast = skills_[i]->getRecastTime();
		if (recast != 0)
		{
			skills_[i]->setRecastTime(recast - 1);
		}
	}
	// 使用されるスキルにカーソルを合わせる
	int tmp = Cursor_;
	Cursor_ = Mouse::getInstance()->getWheelValue() % 10;
	Cursor_ = abs(Cursor_);
	// カーソルが移動したら音を鳴らす
	if (tmp != Cursor_)
	{
		ADX2Le::Play(CRI_CUESHEET_0_SE_GET);
	}
	// 左クリックで通常攻撃スキルを発動
	if (Mouse::getInstance()->Touch(Mouse::LEFT) && executeSkill_ == -1)
	{
		if (skills_[SKILL_NUM - 1])
		{
			if (skills_[SKILL_NUM - 1]->getRecastTime() == 0)
				setExecute(SKILL_NUM - 1);
		}
	}
	// 右クリックで特殊スキルを発動
	if (Mouse::getInstance()->Touch(Mouse::RIGHT) && executeSkill_ == -1)
	{
		if (skills_[Cursor_])
		{
			if (skills_[Cursor_]->getRecastTime() == 0)
				setExecute(Cursor_);
		}
	}
	// スキルを使用していない時
	if (executeSkill_ == -1) return false;

	// スキルの実行が終了した時
	if (!skills_[executeSkill_]->Execute())
	{
		skills_[executeSkill_]->Exit();
		if (executeSkill_ != SKILL_NUM - 1)
		{
			sortSkill(executeSkill_);
			nowSkillNum_--;
		}

		executeSkill_ = -1;
		return false;
	}
	// スキルが実行中だった場合
	return true;
}