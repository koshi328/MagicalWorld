/**
* File: Stage.h
*
* ゲームのステージを管理するクラス
*
* Date: 2017.1.20
* Author: koshi ito
*/
#pragma once
#include <string>
#include <vector>
#include <SimpleMath.h>
class EnemyCreator;

class StageManager
{
public:
	// シングルトン
	static StageManager* getInstance()
	{
		if (!instance_)
			instance_ = new StageManager();
		return instance_;
	}
	// デストラクタ
	~StageManager()
	{
		instance_ = nullptr;
	}
	// 初期化
	void Initialize();
	// 更新
	void Update();
	// ステージ読み込み
	bool LoadStageInfo(const wchar_t* filename);
	// アクセッサ
	inline void setMaxTime(const float& num) { maxTime_ = num; }
	inline void addMaxTime(const float& num) { maxTime_ += num; }
	inline const float& getMaxTime() { return maxTime_; }
	inline void setTime(const float& num) { time_ = num; }
	inline const float& getTime() { return time_; }
	inline void addTime(const float& num) { time_ += num; }
	inline void setKillEnemyNum(const int& num) { killEnemyNum_ = num; }
	inline void addKillEnemyNum(const int& num) { killEnemyNum_ += num; }
	inline const int& getKillEnemyNum() { return killEnemyNum_; }
	inline void setLevel(const int& num) { stageLevel_ = num; }
	inline void addLevel(const int& num) { stageLevel_ += num; }
	inline const int& getLevel() { return stageLevel_; }
public:
	// 地形設定
	bool TerrainSetting(const std::string& str);
	// イベント設定
	bool IventSetting(const std::string& str);
	// エネミー生成設定
	EnemyCreator* EnemyCreatorSetting(const std::string& str);
private:
	float maxTime_;		// ゲームの制限時間
	float time_;		// 経過時間
	int killEnemyNum_;	// 倒したエネミーの数
	int stageLevel_;	// ステージのレベル
	std::vector<DirectX::SimpleMath::Vector3> creatorPos_; // エネミー生成位置
	static StageManager* instance_; // インスタンス
	// コンストラクタ
	StageManager();
};