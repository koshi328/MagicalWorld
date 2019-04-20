/**
* File: EnemySkill.h
* エネミーのスキルの基底クラス
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#pragma once

// 前方宣言
class Enemy;
class EnemySkill
{
protected:
	int castTime_;		// 実行にかかる時間
	int delayTime_;		// 実行後、次のスキルを発動するまでの時間
	Enemy* subject_;	// 操作対象
public:
	// コンストラクタ
	EnemySkill();
	// デストラクタ
	virtual ~EnemySkill();

	// 実行関数
	virtual void Entry() = 0;
	virtual bool Execute() = 0;
	virtual void Exit() = 0;

	// アクセッサ
	inline const int& getDelayTime() { return delayTime_; }
};