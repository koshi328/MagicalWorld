/**
* File: Canvas.h
*
* 表示するUIをまとめて管理するクラス
*
* Date: 2016.12.14
* Author: koshi ito
*/
#pragma once
#include "StatusGauge.h"
#include "SkillWindow.h"
#include "EnemyCounter.h"
#include <memory>

class Canvas
{
private:
	std::unique_ptr<StatusGauge> statusGauge_;	// ステータスUI
	std::unique_ptr<SkillWindow> skillWindow_;	// スキルUI
	std::unique_ptr<EnemyCounter> enemyCounter_;// エネミーカウンターUI
public:
	// コンストラクタ
	Canvas();
	// デストラクタ
	~Canvas();
	// 初期化
	void Initialize(Actor* actor, SkillController* pCon);
	// 更新
	void Update();
	// 描画
	void Render();
};