/**
* File: Billboard.cpp
*
* ビルボードを表示するクラス
*
* Date: 2016.9.20
* Author: koshi ito
*/
#include "Billboard.h"
#include <WICTextureLoader.h>
/**
* 名前空間
*/
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;
/**
* static変数
*/
std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> Billboard::s_textureArray;
Microsoft::WRL::ComPtr<ID3D11InputLayout> Billboard::inputLayout;
std::unique_ptr<DirectX::AlphaTestEffect> Billboard::effect;
std::unique_ptr<PrimitiveBatch<DirectX::VertexPositionTexture>> Billboard::shadowBatch;

// 立方体の頂点データ
DirectX::VertexPositionTexture g_vertexes[4] =
{
	DirectX::VertexPositionTexture(Vector3(-0.5f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)),	 // 0
	DirectX::VertexPositionTexture(Vector3(0.5f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)),	 // 1
	DirectX::VertexPositionTexture(Vector3(-0.5f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)),	 // 2
	DirectX::VertexPositionTexture(Vector3(0.5f, 0.0f, 0.0f), Vector2(1.0f, 1.0f))	 // 3
};
// 頂点の順番
uint16_t g_indexes[6] =
{
	0, 1, 2,
	1, 3, 2
};
/**
* コンストラクタ
*/
Billboard::Billboard()
{
}

/**
* デストラクタ
*/
Billboard::~Billboard()
{
}
/**
* 初期化
*/
void Billboard::Initialize()
{
	// アルファテストエフェクトの作成
	effect.reset(new AlphaTestEffect(Graphics::getInstance()->getDevice()));
	effect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
	effect->SetReferenceAlpha(254);
	// 入力レイアウトの作成
	const void* buf;
	size_t size;
	// 使用するシェーダープログラムとサイズを取得する
	effect->GetVertexShaderBytecode(&buf, &size);
	Graphics::getInstance()->getDevice()->CreateInputLayout(
		DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		buf, size, inputLayout.GetAddressOf());

	// プリミティブを表示する為のオブジェクトを作成する
	shadowBatch.reset(new PrimitiveBatch<DirectX::VertexPositionTexture>(Graphics::getInstance()->getDeviceContext()));
}
/**
* テクスチャを読み込む
* @param file_name -- ファイルのパス
*/
void Billboard::LoadTexture(const wchar_t* file_name)
{
	// 同じ名前のモデルを読み込み済みでなければ
	if (s_textureArray.count(file_name) == 0)
	{
		CreateWICTextureFromFile(Graphics::getInstance()->getDevice(),
			file_name,
			nullptr,
			s_textureArray[file_name].GetAddressOf());
	}
	texture_ = s_textureArray[file_name].Get();
}
/**
* 描画
* @param world -- World座標
* @param view  -- カメラView
* @param proj  -- カメラProjection
*/
void Billboard::Draw(Matrix world, Matrix view, Matrix proj)
{
	// ビルボードにする
	world = CreateBillboardMatrix(view) * world;
	effect->SetWorld(world);
	effect->SetView(view);
	effect->SetProjection(proj);
	effect->SetTexture(texture_);
	effect->Apply(Graphics::getInstance()->getDeviceContext());
	// 入力レイアウトの設定 //
	Graphics::getInstance()->getDeviceContext()->IASetInputLayout(inputLayout.Get());
	// アルファの設定
	Graphics::getInstance()->getDeviceContext()->OMSetBlendState(Graphics::getInstance()->getCommonStates()->NonPremultiplied(), nullptr, 0xffffffff);

	shadowBatch->Begin();
	shadowBatch->DrawIndexed(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		g_indexes, 6,	// 頂点の順番
		g_vertexes, 4	// 頂点の情報
		);
	shadowBatch->End();
}
/**
* ビルボードマトリクスを計算
*/
Matrix Billboard::CreateBillboardMatrix(Matrix view)
{
	// 転置行列を求める
	Matrix m = view.Transpose();
	// 平行移動要素は無くす
	m._41 = m._42 = m._43 = 0.0f;
	m._44 = 1.0f;

	return m;
}
