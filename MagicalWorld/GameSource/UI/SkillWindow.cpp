/**
* File: SkillWindow.cpp
*
* �擾���Ă���X�L����\������UI
*
* Date: 2016.12.20
* Author: koshi ito
*/
#include "SkillWindow.h"
#include <SimpleMath.h>
#include "..\PlayerSkill\Skill.h"
#include "..\..\GameBase\Graphics.h"
/**
* ���O���
*/
using namespace GUI;
using namespace DirectX::SimpleMath;
/**
* �萔�錾
*/
#define ICON_SIZE (64.f)
/**
* �R���X�g���N�^
* @param pCon -- SkillController�̃|�C���^
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
* �f�X�g���N�^
*/
SkillWindow::~SkillWindow()
{
}
/**
* �X�V
*/
void SkillWindow::Update()
{
	// �I�΂�Ă���A�C�R����������₷���悤�ɕ\���@��ς���
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
		// �A�C�R���̐ݒ�
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
* �`��
*/
void SkillWindow::Draw()
{
	for (int i = 0; i < SKILL_NUM - 1; i++)
	{
		// �A�C�R���\��
		_sprite[i]->Draw();
	}
}