/**
* File: Skill.h
* プレイヤーが使うスキルの基底クラス
*
* Date: 2016.11.17
* Author: koshi ito
*/

#pragma once
/**
* 前方宣言
*/
class Actor;

class Skill
{
protected:
	int id_;		// スキルID
	int recastTime_;// リキャスト時間
	Actor* subject_;// 対象者
public:
	// コンストラクタ
	Skill() = default;
	// デストラクタ
	virtual ~Skill() = default;

	// 登録
	virtual void Entry() = 0;
	// 実行
	virtual bool Execute() = 0;
	// 終了
	virtual void Exit() = 0;

	// アクセッサ
	inline void setID(const int& n) { id_ = n; }
	inline const int& getID() { return id_; }
	inline const int& getRecastTime() { return recastTime_; }
	inline void setRecastTime(int time) { recastTime_ = time; }
};