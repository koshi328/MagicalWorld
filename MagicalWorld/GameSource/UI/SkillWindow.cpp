/**
* File: SkillWindow.cpp
*
* 取得しているスキルを表示するUI
*
* Date: 2016.12.20
* Author: koshi ito
*/
#include "SkillWindow.h"
#include <SimpleMath.h>
#include "..\PlayerSkill\Skill.h"
#include "..\..\GameBase\Graphics.h"
/**
* 名前空間
*/
using namespace GUI;
using namespace DirectX::SimpleMath;
/**
* 定数宣言
*/
#define ICON_SIZE (64.f)
/**
* コンストラクタ
* @param pCon -- SkillControllerのポインタ
*/
SkillWindow::SkillWindow(SkillController* pCon)
:_pSkillController(pCon)
{
	float x = 1280.f / 2.f;
	float y = 720.f - ICON_SIZE;
	x -= ICON_SIZE * 5;
	for (int i = 0; i < SKILL_NUM - 1; i++)
	{
		_sprite[i].reset(new Sprite(L"Resources/Texture/SkillIcon/None.png", x + (ICON_SIZE * i), y, ICON_SIZE, ICON_SIZE));
	}


}
/**
* デストラクタ
*/
SkillWindow::~SkillWindow()
{
}
/**
* 更新
*/
void SkillWindow::Update()
{
	// 選ばれているアイコンが分かりやすいように表示法を変える
	for (int i = 0; i < SKILL_NUM - 1; i++)
	{
		float y = 720.f - ICON_SIZE;
		if (i == _pSkillController->getCursor())
		{
			_sprite[i]->Y(y - 20);
		}
		else
		{
			_sprite[i]->Y(y);
		}
		// アイコンの設定
		switch (_pSkillController->getID(i))
		{
		case 0:
			_sprite[i]->changeTexture(L"Resources/Texture/SkillIcon/attackIcon.png");
			break;
		case 1:
			_sprite[i]->changeTexture(L"Resources/Texture/SkillIcon/hpIcon.png");
			break;
		case 2:
			_sprite[i]->changeTexture(L"Resources/Texture/SkillIcon/bomb.png");
			break;
		case 3:
			_sprite[i]->changeTexture(L"Resources/Texture/SkillIcon/laser.png");
			break;
		case -1:
			_sprite[i]->changeTexture(L"Resources/Texture/SkillIcon/none.png");
			break;
		default:
			break;
		}
	}
}
/**
* 描画
*/
void SkillWindow::Draw()
{
	for (int i = 0; i < SKILL_NUM - 1; i++)
	{
		// アイコン表示
		_sprite[i]->Draw();
	}
}