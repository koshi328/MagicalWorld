/**
* File: RenderTarget.h
*
* レンダーターゲット
*
* Date: 2017.1.15
* Author: koshi ito
*/
#pragma once

#include <memory>
#include <SimpleMath.h>

class RenderTarget
{
public:
	// コンストラクタ
	RenderTarget();
	// デストラクタ
	~RenderTarget();

	using Color = DirectX::SimpleMath::Color;
	// オブジェクトの作成
	void Create(ID3D11Device * device, UINT width, UINT height, DXGI_FORMAT targetFormat, DXGI_FORMAT depthFormat);
	// レンダーターゲットの変更
	void Begin(ID3D11DeviceContext *context);
	// レンダーターゲットの変更とクリア
	void BeginAndClear(ID3D11DeviceContext *context, Color color);
	// レンダーターゲットを元に戻す
	void End(ID3D11DeviceContext *context);
	// テクスチャお取得
	ID3D11ShaderResourceView* GetResourceView();

private:
	// インプリメントクラス
	class Impl;
	std::unique_ptr<Impl> pImpl_;
};