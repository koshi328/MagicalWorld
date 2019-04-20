/**
* File: GUI.cpp
*
* UI�ɕ\������@�\���܂Ƃ߂����C�u����
*
* Date: 2016.12.14
* Author: koshi ito
*/
#include "GUI.h"
#include <WICTextureLoader.h>
#include <SimpleMath.h>
#include "..\..\GameBase\Graphics.h"
#include "..\..\Utility\Mouse.h"

using namespace DirectX::SimpleMath;
/**
* static�ϐ�
*/
std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> GUI::Sprite::texturePool_;

// ---------------------------------------------------
// Sprite
// ---------------------------------------------------
/**
* �R���X�g���N�^
* @param path -- �t�@�C���p�X
* @param x    -- X���W
* @param y    -- Y���W
* @param w    -- ��
* @param h    -- ����
*/
GUI::Sprite::Sprite(wchar_t* path, float x, float y, float w, float h)
	:x_(x), y_(y), w_(w), h_(h)
{
	if (texturePool_.count(path) == 0)
	{
		// �e�N�X�`���쐬
		DirectX::CreateWICTextureFromFile(Graphics::getInstance()->getDevice(), path, nullptr, texturePool_[path].GetAddressOf());
	}
	pTexture_ = texturePool_[path].Get();
	top_ = 0.f;
	left_ = 0.f;
}
/**
* �f�X�g���N�^
*/
GUI::Sprite::~Sprite()
{

}
/**
* �`��
* @param color    -- �\���F
* @param rotation -- ��]
*/
void GUI::Sprite::Draw(DirectX::FXMVECTOR color, float rotation)
{
	RECT rect = { (LONG)left_, (LONG)top_, (LONG)w_, (LONG)h_ };
	Graphics::getInstance()->getSpriteBatch()->Draw(pTexture_, Vector2(x_ + w_ / 2, y_ + h_ / 2), &rect, color, rotation, Vector2(w_ / 2, h_ / 2));
}
/**
* �e�N�X�`����ς���
* @param path -- �\���������摜�̃t�@�C���p�X
*/
void GUI::Sprite::changeTexture(wchar_t* path)
{
	if (texturePool_.count(path) == 0)
	{
		// �e�N�X�`���쐬
		DirectX::CreateWICTextureFromFile(Graphics::getInstance()->getDevice(), path, nullptr, texturePool_[path].GetAddressOf());
	}
	pTexture_ = texturePool_[path].Get();
}

// ---------------------------------------------------
// Button
// ---------------------------------------------------
/**
* �R���X�g���N�^
* @param path -- �t�@�C���p�X
* @param x    -- X���W
* @param y    -- Y���W
* @param w    -- ��
* @param h    -- ����
*/
GUI::Button::Button(wchar_t* path, float x, float y, float w, float h)
{
	sprite_ = new Sprite(path, x, y, w, h);
}
/**
* �f�X�g���N�^
*/
GUI::Button::~Button()
{
	delete sprite_;
}
/**
* �`��
* @param color    -- �\���F
* @param rotation -- ��]
*/
void GUI::Button::Draw(DirectX::FXMVECTOR color, float rotation)
{
	sprite_->Draw(color, rotation);
}
/**
* �N���b�N���ꂽ���̏���
* @return -1 -- �{�^���̏�ɃJ�[�\��������Ă���
* @return  0 -- �����ꂽ��
* @return  1 -- �b���ꂽ��
* @return  2 -- �J�[�\�����G��Ă��Ȃ���
*/
int GUI::Button::ClickCheck()
{
	Mouse* mouse = Mouse::getInstance();
	if (sprite_->x_ > mouse->X()) return -1;
	if (sprite_->x_ + sprite_->w_ < mouse->X()) return -1;
	if (sprite_->y_ > mouse->Y()) return -1;
	if (sprite_->y_ + sprite_->h_ < mouse->Y()) return -1;

	if (mouse->Press(Mouse::LEFT))
		return 0;
	if (mouse->Release(Mouse::LEFT))
		return 1;

	return 2;
}
