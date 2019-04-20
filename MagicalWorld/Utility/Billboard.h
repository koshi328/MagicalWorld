/**
* File: Billboard.h
*
* ビルボードを表示するクラス
*
* Date: 2016.9.20
* Author: koshi ito
*/
#pragma once
#include <map>
#include <wrl.h>
#include "..\GameBase\Graphics.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>


class Billboard
{
public:
	// コンストラクタ
	Billboard();
	// デストラクタ
	~Billboard();
	// 初期化
	static void Initialize();
	// 読み込み
	void LoadTexture(const wchar_t* file_name);
	// 描画
	void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	// ビルボードMatrixを計算
	DirectX::SimpleMath::Matrix Billboard::CreateBillboardMatrix(DirectX::SimpleMath::Matrix view);
private:
	// 読み込み済みテクスチャーコンテナ
	static std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> s_textureArray;
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	static std::unique_ptr<DirectX::AlphaTestEffect> effect;
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> shadowBatch;

private:
	// 描画テクスチャ
	ID3D11ShaderResourceView* texture_;
};

