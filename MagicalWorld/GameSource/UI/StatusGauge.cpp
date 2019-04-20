/**
* File: StatusGauge.cpp
*
* �X�e�[�^�X��\������UI
*
* Date: 2016.12.20
* Author: koshi ito
*/
#include "StatusGauge.h"
#include "..\Object\Actor.h"
#include "..\..\GameBase\Graphics.h"
/**
* ���O���
*/
using namespace GUI;
using namespace DirectX::SimpleMath;
/**
* �萔�錾
*/
#define OFF_SET (8)

/**
* �R���X�g���N�^
* @param actor -- �X�e�[�^�X��\��������Actor�̃|�C���^
*/
StatusGauge::StatusGauge(Actor* actor)
:actor_(actor)
{
	gaugeLength_ = 1000.f - (OFF_SET * 2);
	float x = (1280.f - 1000.f) / 2;
	float y = 720.f - 118.f;
	float gaugeHight = 10.f;
	flame_ = new Sprite(L"Resources/Texture/StatusGauge.png", x, y, 1000.f, 32.f);
	hpGauge_ = new Sprite(L"Resources/Texture/Gauge.png", x, y, 1000.f, 32.f);
	hpLostGauge_ = new Sprite(L"Resources/Texture/Gauge.png", x, y, 1000.f, 32.f);
}
/**
* �f�X�g���N�^
*/
StatusGauge::~StatusGauge()
{
	if(flame_)		// �E�B���h�E
		delete flame_;
	if (hpLostGauge_)	// HP�Q�[�W�������̉��o�p
		delete hpLostGauge_;
	if(hpGauge_)	// HP�Q�[�W
		delete hpGauge_;
}
/**
* �X�V
*/
void StatusGauge::Update()
{
	// HP�̌v�Z
	float restWidth = ((float)actor_->getHP() / (float)actor_->getMaxHP()) * gaugeLength_ + OFF_SET;
	float width = hpGauge_->W() + (restWidth - hpGauge_->W()) * 0.5f;
	hpGauge_->W(width);
	// HP�ԃQ�[�W
	width = hpLostGauge_->W() + (restWidth - hpLostGauge_->W()) * 0.05f;
	hpLostGauge_->W(width);
}
/**
* �`��
*/
void StatusGauge::Draw()
{
	if(flame_)			// �E�B���h�E
		flame_->Draw();
	if (hpLostGauge_)	// HP�Q�[�W�������̉��o�p
		hpLostGauge_->Draw(Color(1, 0, 0, 1));
	if (hpGauge_)		// HP
		hpGauge_->Draw(Color(0, 1, 0.3f, 1));

	wchar_t buf[128];
	swprintf_s(buf, 128, L"%d",actor_->getHP());
	Graphics* graphics = Graphics::getInstance();
	graphics->getSpriteFont()->DrawString(graphics->getSpriteBatch(),
		buf,
		Vector2(flame_->X(), flame_->Y()), Color(1.f, 1.f, 1.f, 1.f));
}