/**
* File: Enemy.h
*
* エネミーの共通アルゴリズム
*
* Date: 2016.12.20
* Author: koshi ito
*/
#pragma once
#include "Actor.h"
#include "..\AI\AIState.h"
#include "..\Collision\Collision.h"
#include <list>
#include <vector>

class EnemyCreator;
/**
* 敵視管理構造体
*/
struct TARGET
{
	GameObject* target;	// 対象
	int hate;			// 敵視
};

class Enemy : public Actor
{
private:
	std::list<TARGET*> targetList_;		// 敵視リスト
	std::vector<int> skillNumberList_;	// 使用するスキルの番号リスト
	int skillDelay_;					// スキル硬直
	unsigned int skillID_;				// 使用しているスキルのID
	Sphere collision_;					// 衝突判定コライダー
	AIState* state_;					// AIの状態
	float seachRange_;					// 索敵範囲
	int modelID_;						// 表示するモデルのID
	EnemyCreator* myCreator_;			// 作成元
	int point_;							// スコア
	int unrivaledTime_;
public:
	Enemy(Enemy* copy);
	Enemy();
	~Enemy();
	// ターゲット情報更新
	void AmbientSearch();

	// Stateの変更
	void ChangeState(AIState* state);

	// 衝突判定へのアクセス
	inline Collision* getCollision();
	// ターゲット情報へアクセス
	inline std::list<TARGET*>& getTargetInfo() { return targetList_; }
	// スキルにアクセス
	inline void setSkillDelay(const int& time) { skillDelay_ = time; }
	inline const int& getSkillDelay() { return skillDelay_; }
	inline void setUseSkillID(const int& id) { skillID_ = id; }
	inline const unsigned int& getUseSkillID() { return skillID_; }
	inline std::vector<int>& getSkillList() { return skillNumberList_; }

	void Initialize();
	void Update();
	void Draw();

	bool LoadStatus(const wchar_t* filename);
	void setCreator(EnemyCreator* creator);

private:
	void EnemysRebound();
};