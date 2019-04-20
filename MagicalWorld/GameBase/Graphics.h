/**
* File: Graphics.h
*
* 描画関連
*
* Date: 2016.11.30
* Author: koshi ito
*/
#pragma once

#include <d3d11.h>
#include <memory>

#include <SpriteFont.h>
#include <CommonStates.h>


class Graphics
{
public:
	// シングルトン
	static Graphics* getInstance();
	// デストラクタ
	~Graphics();
	// 初期化
	HRESULT Initialize(HWND hwnd);
	// 終了
	void Finalize();

	// D3D11
	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	IDXGISwapChain* getSwapChain();
	ID3D11RenderTargetView* getRenderTargetView();
	ID3D11Texture2D* getDepthStencil();
	ID3D11DepthStencilView* getDepthStencilView();
	DirectX::SpriteBatch* getSpriteBatch();
	DirectX::SpriteFont* getSpriteFont();
	DirectX::CommonStates* getCommonStates();
private:
	// コンストラクタ
	Graphics();

	class Impl;
	std::unique_ptr<Impl> _pImpl;
	// インスタンス
	static Graphics* _instance;
};