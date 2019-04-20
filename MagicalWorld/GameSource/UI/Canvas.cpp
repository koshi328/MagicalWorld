/**
* File: Canvas.cpp
*
* 表示するUIをまとめて管理するクラス
*
* Date: 2016.12.14
* Author: koshi ito
*/
#include "Canvas.h"

/**
* コンストラクタ
*/
Canvas::Canvas()
{
}
/**
* デストラクタ
*/
Canvas::~Canvas()
{

}
/**
* 初期化
* @param actor -- ステータスを表示したいActorのポインタ
* @param pCon  -- 表示したいSkillControllerのポインタ
*/
void Canvas::Initialize(Actor* actor, SkillController* pCon)
{
	statusGauge_ = std::make_unique<StatusGauge>(actor);
	skillWindow_ = std::make_unique<SkillWindow>(pCon);
	enemyCounter_ = std::make_unique<EnemyCounter>();
}
/**
* 更新
*/
void Canvas::Update()
{
	if (statusGauge_)
		statusGauge_->Update();
	if (skillWindow_)
		skillWindow_->Update();
	if (enemyCounter_)
		enemyCounter_->Update();
}
/**
* 描画
*/
void Canvas::Render()
{
	if (statusGauge_)
		statusGauge_->Draw();
	if (skillWindow_)
		skillWindow_->Draw();
	if (enemyCounter_)
		enemyCounter_->Draw();
}