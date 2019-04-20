/**
* File: EnemyCreator.h
*
* エネミーの生成ポイント
*
* Date: 2016.12.20
* Author: koshi ito
*/
#pragma once
#include "Actor.h"
#include <vector>
#include "Enemy.h"

class EnemyCreator : public Actor
{
public:
	// コンストラクタ
	EnemyCreator();
	// デストラクタ
	~EnemyCreator();
	
	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 最大数の設定
	void setMaxCreateNum(int num);
	// 生成頻度の設定
	void setFrequency(int num);
	// 生成するエネミーのプロトタイプを登録
	void setEnemyPrototype(Enemy* enemy);
	// 生成数をカウント
	void addCreateNum(int num);
private:
	int maxCreateNum_;					// 最大数
	int nowCreateNum_;					// 現在の数
	int frequency_;						// 生成頻度
	int time_;							// 経過時間
	std::vector<Enemy*> prototypeList_;	// プロトタイプのリスト
};