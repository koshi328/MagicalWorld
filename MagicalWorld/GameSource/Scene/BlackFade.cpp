/**
* File: BlackFade.cpp
*
* �����t�F�[�h�����ʑJ�ډ��o
*
* Date: 2017.1.20
* Author: koshi ito
*/
#include "BlackFade.h"
#include "..\Scene\SceneManager.h"
#include <SimpleMath.h>

/**
* ���O���
*/
using namespace DirectX::SimpleMath;
/**
* �R���X�g���N�^
*/
BlackFade::BlackFade()
{
	filter_ = new GUI::Sprite(L"Resources/Texture/square64.png", 0, 0, 1280, 720);
}
/**
* �f�X�g���N�^
*/
BlackFade::~BlackFade()
{

}
/**
* ������
*/
void BlackFade::Initialize()
{
	time_ = 180;
	alpha_ = 0.0f;
}
/**
* �X�V
*/
void BlackFade::Update()
{
	--time_;
	// �Â����鏈��
	if (time_ > 120)
	{
		alpha_ += 1.0f / 60;
	}
	// ���邭���鏈��
	if (time_ < 60)
	{
		alpha_ -= 1.0f / 60;
	}
	// �Â��Ȃ������ɃV�[����؂�ւ���
	if (time_ == 60)
	{
		SceneManager::getInstance()->ChangeScene();
	}
	// ���邭�Ȃ����玩�g��j������
	if (time_ <= 0)
	{
		SceneManager::getInstance()->DisposeEffect();
	}

}
/**
* �`��
*/
void BlackFade::Render()
{
	filter_->Draw(Color(0, 0, 0, alpha_));
}