/**
* File: Graphics.h
*
* �`��֘A
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
	// �V���O���g��
	static Graphics* getInstance();
	// �f�X�g���N�^
	~Graphics();
	// ������
	HRESULT Initialize(HWND hwnd);
	// �I��
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
	// �R���X�g���N�^
	Graphics();

	class Impl;
	std::unique_ptr<Impl> _pImpl;
	// �C���X�^���X
	static Graphics* _instance;
};