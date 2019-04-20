/**
* File: MeshManager.h
* モデルの描画管理クラス
*
* Date: 2016.12.10
* Author: koshi ito
*/

#pragma once
#include <vector>
#include <Model.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>

class MeshManager
{
public:
	// 描画に必要な情報を纏めた構造体
	struct ModelInfo
	{
		UINT model_; // モデル番号
		DirectX::SimpleMath::Matrix world_; // World座標
		std::function<void()> lamda_; // 関数
	};
private:
	// デバイスコンテキストのポインタ
	ID3D11DeviceContext* context_;
	// デバイスのポインタ
	ID3D11Device* device_;

	// 描画したいモデルの確保配列
	static std::unique_ptr <std::unique_ptr<DirectX::Model>[]> modelList_;
	// 描画するモデルの配列
	std::vector<ModelInfo> drawList_;
	// 登録最大数
	UINT maxNum_;
public:
	// デフォルトコンストラクタ
	MeshManager();
	// コンストラクタ
	MeshManager(UINT um, ID3D11DeviceContext* context, ID3D11Device* device);
	// デストラクタ
	~MeshManager();
	// 初期化
	void Initialize(UINT um, ID3D11DeviceContext* context, ID3D11Device* device);
	// モデル読み込み
	bool LoadModel(UINT num, wchar_t* file, DirectX::DGSLEffectFactory* factory);
	// モデル登録
	void Entry(UINT num, DirectX::SimpleMath::Matrix world, std::function<void()> func = nullptr);
	// 描画
	void Render(DirectX::CommonStates* state, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	// リセット
	void Reset();
	// Drawリストの取得
	std::vector<ModelInfo>& getDrawList() { return drawList_; }
};
