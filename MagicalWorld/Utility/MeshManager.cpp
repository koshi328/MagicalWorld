/**
* File: MeshManager.cpp
* モデルの描画管理クラス
*
* Date: 2016.12.10
* Author: koshi ito
*/

#include "MeshManager.h"
#include "..\GameBase\Graphics.h"
#include <DirectXColors.h>

using namespace DirectX;
/**
* static変数
*/
std::unique_ptr <std::unique_ptr<DirectX::Model>[]> MeshManager::modelList_;

/**
* グローバル変数
*/
float timeCount;

/**
* デフォルトコンストラクタ
*/
MeshManager::MeshManager()
{
	timeCount = 0.f;
	maxNum_ = 0;
	context_ = nullptr;
	device_ = nullptr;
}

/**
* コンストラクタ
* @param num -- 読み込むモデルの最大数
* @param context -- デバイスコンテキスト
*/
MeshManager::MeshManager(UINT num, ID3D11DeviceContext* context, ID3D11Device* device)
{
	// デバイスポインタの確保
	context_ = context;
	device_ = device;
	// 読み込みモデルの配列を確保する
	if (!modelList_)
	{
		modelList_.reset(new std::unique_ptr<Model>[num]);
	}
	maxNum_ = num;
}

/**
* デストラクタ
*/
MeshManager::~MeshManager()
{
	drawList_.clear();
}

void MeshManager::Initialize(UINT num, ID3D11DeviceContext* context, ID3D11Device* device)
{
	// デバイスポインタの確保
	context_ = context;
	device_ = device;
	// 読み込みモデルの配列を確保する
	if (!modelList_)
	{
		modelList_.reset(new std::unique_ptr<Model>[num]);
	}

	maxNum_ = num;
}

/**
* モデルの読み込み
* @param num     -- 登録する番号
* @param file    -- ファイルパス
* @param factory -- エフェクトファクトリー
* @return(bool) true  -- 読み込み成功
* @return(bool) false -- 読み込み失敗
*/
bool MeshManager::LoadModel(UINT num, wchar_t* file, DGSLEffectFactory* factory)
{
	// 確保した配列を超えた指定があったら終了
	if (num >= maxNum_) return false;
	// モデルの読み込み
	modelList_[num] = Model::CreateFromCMO(device_, file, *factory);

	return true;
}

/**
* 描画の登録
* @param num   -- 描画するモデル番号
* @param world -- World座標
* @param func  -- 描画方法の指定(HLSL等)
*/
void MeshManager::Entry(UINT num, DirectX::SimpleMath::Matrix world, std::function<void()> func)
{
	ModelInfo info;
	info.lamda_ = func;
	info.world_ = world;
	info.model_ = num;
	// 追加
	drawList_.push_back(info);
}

/**
* 描画
* @param state      -- CommonState
* @param view       -- View行列
* @param projection -- Projection行列
*/
void MeshManager::Render(CommonStates* state, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	for each (ModelInfo data in drawList_)
	{
		modelList_[data.model_]->Draw(context_, *state, data.world_, view, projection, false, data.lamda_);
	}
}

/**
* 描画の登録をリセット
*/
void MeshManager::Reset()
{
	drawList_.clear();
}