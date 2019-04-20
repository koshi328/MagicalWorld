/**
* File: SkillController.h
*
* プレイヤーのスキルの実行を管理するクラス
*
* Date: 2016.12.5
* Author: koshi ito
*/
#pragma once
/**
* 定数宣言
*/
#define SKILL_NUM (11)
#define SKILL_TYPE_NUM (4)
/**
* 前方宣言
*/
class Skill;
class Actor;

class SkillController
{
private:
	Skill* skills_[SKILL_NUM];	// スキルのスロット
	int executeSkill_;			// 実行しているスキルのID
	Actor* subject_;			// 対象者
	int Cursor_;				// スキルスロットの指している番号
	int nowSkillNum_;			// 取得しているスキルの数
private:
	// 
	Skill* createSkill(int id);
public:
	// コンストラクタ
	SkillController(Actor* subject);
	// デストラクタ
	~SkillController();

	// 実行
	bool Run();
	// スキルを左詰めにする
	void sortSkill(int begin);
	// アクセッサ
	void setSkill(int number, int id);
	const int& getID(int number);
	void setExecute(int number);
	Skill* getSkill(const int& number);
	int getCursor();
	int getNowSkillNum();
	void setNowSkillNum(const int& num);
};