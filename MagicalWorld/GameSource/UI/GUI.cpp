/**
* File: GUI.cpp
*
* UIに表示する機能をまとめたライブラリ
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
* static変数
*/
std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> GUI::Sprite::texturePool_;

// ---------------------------------------------------
// Sprite
// ---------------------------------------------------
/**
* コンストラクタ
* @param path -- ファイルパス
* @param x    -- X座標
* @param y    -- Y座標
* @param w    -- 幅
* @param h    -- 高さ
*/
GUI::Sprite::Sprite(wchar_t* path, float x, float y, float w, float h)
	:x_(x), y_(y), w_(w), h_(h)
{
	if (texturePool_.count(path) == 0)
	{
		// テクスチャ作成
		DirectX::CreateWICTextureFromFile(Graphics::getInstance()->getDevice(), path, nullptr, texturePool_[path].GetAddressOf());
	}
	pTexture_ = texturePool_[path].Get();
	top_ = 0.f;
	left_ = 0.f;
}
/**
* デストラクタ
*/
GUI::Sprite::~Sprite()
{

}
/**
* 描画
* @param color    -- 表示色
* @param rotation -- 回転
*/
void GUI::Sprite::Draw(DirectX::FXMVECTOR color, float rotation)
{
	RECT rect = { (LONG)left_, (LONG)top_, (LONG)w_, (LONG)h_ };
	Graphics::getInstance()->getSpriteBatch()->Draw(pTexture_, Vector2(x_ + w_ / 2, y_ + h_ / 2), &rect, color, rotation, Vector2(w_ / 2, h_ / 2));
}
/**
* テクスチャを変える
* @param path -- 表示したい画像のファイルパス
*/
void GUI::Sprite::changeTexture(wchar_t* path)
{
	if (texturePool_.count(path) == 0)
	{
		// テクスチャ作成
		DirectX::CreateWICTextureFromFile(Graphics::getInstance()->getDevice(), path, nullptr, texturePool_[path].GetAddressOf());
	}
	pTexture_ = texturePool_[path].Get();
}

// ---------------------------------------------------
// Button
// ---------------------------------------------------
/**
* コンストラクタ
* @param path -- ファイルパス
* @param x    -- X座標
* @param y    -- Y座標
* @param w    -- 幅
* @param h    -- 高さ
*/
GUI::Button::Button(wchar_t* path, float x, float y, float w, float h)
{
	sprite_ = new Sprite(path, x, y, w, h);
}
/**
* デストラクタ
*/
GUI::Button::~Button()
{
	delete sprite_;
}
/**
* 描画
* @param color    -- 表示色
* @param rotation -- 回転
*/
void GUI::Button::Draw(DirectX::FXMVECTOR color, float rotation)
{
	sprite_->Draw(color, rotation);
}
/**
* クリックされた時の処理
* @return -1 -- ボタンの上にカーソルが乗っている
* @return  0 -- 押された時
* @return  1 -- 話された時
* @return  2 -- カーソルが触れていない時
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
