/**
* File: Canvas.cpp
*
* �\������UI���܂Ƃ߂ĊǗ�����N���X
*
* Date: 2016.12.14
* Author: koshi ito
*/
#include "Canvas.h"

/**
* �R���X�g���N�^
*/
Canvas::Canvas()
{
}
/**
* �f�X�g���N�^
*/
Canvas::~Canvas()
{

}
/**
* ������
* @param actor -- �X�e�[�^�X��\��������Actor�̃|�C���^
* @param pCon  -- �\��������SkillController�̃|�C���^
*/
void Canvas::Initialize(Actor* actor, SkillController* pCon)
{
	statusGauge_ = std::make_unique<StatusGauge>(actor);
	skillWindow_ = std::make_unique<SkillWindow>(pCon);
	enemyCounter_ = std::make_unique<EnemyCounter>();
}
/**
* �X�V
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
* �`��
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