/**
* File: GUI.h
*
* UI�ɕ\������@�\���܂Ƃ߂����C�u����
*
* Date: 2016.12.14
* Author: koshi ito
*/
#pragma once
#include <d3d11.h>
#include <map>
#include <wrl.h>
#include <DirectXColors.h>

namespace GUI
{
	// �e�N�X�`�����X�N���[���ɕ\������N���X
	class Sprite
	{
		// �t�����h�ݒ�
		friend class Button;
	private:
		// �A�h���X����e�N�X�`���̃v�[�����쐬
		static std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> texturePool_;
		// �\������e�N�X�`���̃|�C���^
		ID3D11ShaderResourceView* pTexture_;
		float x_;	// �\���ʒuX�i�����_�j
		float y_;	// �\���ʒuY�i�����_�j
		float w_;	// �E�[
		float h_;	// ���[
		float left_;// ���[
		float top_;	// ��[
	public:
		// �R���X�g���N�^
		Sprite(wchar_t* path, float x, float y, float w, float h);
		// �f�X�g���N�^
		~Sprite();
		// �`��
		void Draw(DirectX::FXMVECTOR color = DirectX::Colors::White, float rotation = 0.f);

		void changeTexture(wchar_t* path);

		// �A�N�Z�b�T
		inline const float& X() { return x_; }
		inline void X(const float& x) { x_ = x; }
		inline const float& Y() { return y_; }
		inline void Y(const float& y) { y_ = y; }
		inline const float& W() { return w_; }
		inline void W(const float& w) { w_ = w; }
		inline const float& H() { return h_; }
		inline void H(const float& h) { h_ = h; }
		inline const float& Left() { return left_; }
		inline void Left(const float& left) { left_ = left; }
		inline const float& Top() { return top_; }
		inline void Top(const float& top) { top_ = top; }
	};
	// �{�^���N���X
	class Button
	{
	private:
		Sprite* sprite_; // �\������e�N�X�`��
	public:
		// �R���X�g���N�^
		Button(wchar_t* path, float x, float y, float w, float h);
		// �f�X�g���N�^
		~Button();
		// �N���b�N����
		int ClickCheck();
		// �`��
		void Draw(DirectX::FXMVECTOR color = DirectX::Colors::White, float rotation = 0.f);
	};
}