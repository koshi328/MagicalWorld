/**
* File: GUI.h
*
* UIに表示する機能をまとめたライブラリ
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
	// テクスチャをスクリーンに表示するクラス
	class Sprite
	{
		// フレンド設定
		friend class Button;
	private:
		// アドレスからテクスチャのプールを作成
		static std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> texturePool_;
		// 表示するテクスチャのポインタ
		ID3D11ShaderResourceView* pTexture_;
		float x_;	// 表示位置X（左上基点）
		float y_;	// 表示位置Y（左上基点）
		float w_;	// 右端
		float h_;	// 下端
		float left_;// 左端
		float top_;	// 上端
	public:
		// コンストラクタ
		Sprite(wchar_t* path, float x, float y, float w, float h);
		// デストラクタ
		~Sprite();
		// 描画
		void Draw(DirectX::FXMVECTOR color = DirectX::Colors::White, float rotation = 0.f);

		void changeTexture(wchar_t* path);

		// アクセッサ
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
	// ボタンクラス
	class Button
	{
	private:
		Sprite* sprite_; // 表示するテクスチャ
	public:
		// コンストラクタ
		Button(wchar_t* path, float x, float y, float w, float h);
		// デストラクタ
		~Button();
		// クリック処理
		int ClickCheck();
		// 描画
		void Draw(DirectX::FXMVECTOR color = DirectX::Colors::White, float rotation = 0.f);
	};
}