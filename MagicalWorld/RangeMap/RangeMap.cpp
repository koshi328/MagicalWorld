/**
* File: RangeMap.cpp
*
* 攻撃予測範囲を表示するクラス
*
* Date: 2017.2.1
* Author: koshi ito
*/
#include "RangeMap.h"
#include "..\GameSource\Collision\Collision2D.h"
#include "..\Utility\RenderTarget.h"
#include "..\Utility\BinaryLoader.h"
#include "..\Utility\MeshManager.h"
#include "..\GameBase\Graphics.h"
#include "..\Utility\ConstantBuffer.h"

using namespace std;
using namespace DirectX::SimpleMath;
using namespace DirectX;
using namespace Microsoft::WRL;
/**
* グローバル関数
*/
extern MeshManager* getMeshManager();
/**
* static変数
*/
RangeMap* RangeMap::instance_ = nullptr;

/**
* コンスタントバッファ
*/
struct MatrixesConstant
{
	XMMATRIX WVP;
	XMMATRIX AboveWVP;

	using Type = ConstantBuffer<MatrixesConstant>;
};

/**
* インプリメントクラス
*/
class RangeMap::Impl
{
public:
	unique_ptr<MeshManager> meshManager;				// 範囲表示モデルを描画するメッシュマネージャー
	unique_ptr<RenderTarget> renderTarget[2];			// レンダーターゲット
	unique_ptr<MatrixesConstant::Type> matrixesConstant;// コンスタントバッファ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> rangeVS;	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> rangePS;	// ピクセルシェーダー
	int renderNum;										// レンダーターゲット切り替え用
	// コンストラクタ
	Impl()
	{
		ID3D11Device* device = Graphics::getInstance()->getDevice();
		// メッシュマネージャーの生成
		meshManager = make_unique<MeshManager>();
		meshManager->Initialize(50, Graphics::getInstance()->getDeviceContext(), Graphics::getInstance()->getDevice());
		// レンダーターゲットの生成
		renderTarget[0] = make_unique<RenderTarget>();
		renderTarget[0]->Create(device, 1280 * 2, 720 * 2, 
			DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
		renderTarget[1] = make_unique<RenderTarget>();
		renderTarget[1]->Create(device, 1280 * 2, 720 * 2,
			DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
		renderNum = 0;

		// ファイルオブジェクト
		BinaryLoader buffer;
		// 表示用shader
		buffer.Load(L"Resources/cModels/rangeVS.cso");
		device->CreateVertexShader(buffer.Data(), buffer.Size(), nullptr, rangeVS.GetAddressOf());
		buffer.Load(L"Resources/cModels/rangePS.cso");
		device->CreatePixelShader(buffer.Data(), buffer.Size(), nullptr, rangePS.GetAddressOf());
		// コンスタントバッファの生成
		matrixesConstant.reset(new MatrixesConstant::Type(device));
	}
	// デストラクタ
	~Impl()
	{

	}
	/**
	* サークル範囲を登録する
	* @param circle -- Collision2DのCircleのポインタ
	*/
	void Add(Circle* circle)
	{
		ID3D11DeviceContext* context = Graphics::getInstance()->getDeviceContext();
		CommonStates* state = Graphics::getInstance()->getCommonStates();
		Matrix world = Matrix::Identity;
		Matrix trans = Matrix::CreateTranslation(Vector3(circle->_position.x, 0.f, circle->_position.z));
		Matrix scale = Matrix::CreateScale(circle->_radius);
		world *= scale * trans;
		meshManager->Entry(5, world, [context, state]{
			context->OMSetBlendState(state->AlphaBlend(), Colors::Brown, 0xFFFFFFFF);
		});
	}
	/**
	* 矩形範囲を登録する
	* @param square -- Collision2DのSquareのポインタ
	*/
	void Add(Square* square)
	{
		ID3D11DeviceContext* context = Graphics::getInstance()->getDeviceContext();
		CommonStates* state = Graphics::getInstance()->getCommonStates();
		Matrix world = Matrix::Identity;
		Matrix trans = Matrix::CreateTranslation(Vector3(square->_position.x, 0.f, square->_position.z));
		Matrix scale = Matrix::CreateScale(Vector3(square->_x / 2, 1.f, square->_z / 2));
		Matrix rotY = Matrix::CreateRotationY(square->_angle);
		world *= scale * rotY * trans;

		meshManager->Entry(7, world, [context, state] {
			context->OMSetBlendState(state->AlphaBlend(), Colors::Brown, 0xFFFFFFFF);
		});
	}
	/**
	* 扇範囲を登録する
	* @param fanShape -- Collision2DのFanShapeのポインタ
	*/
	void Add(FanShape* fanShape)
	{
		//Matrix world = Matrix::Identity;
		//Matrix trans = Matrix::CreateTranslation(Vector3(fanShape->_position.x, 0.f, fanShape->_position.z));
		//Matrix scale = Matrix::CreateScale(fanShape->_radius);
		//Matrix rotY = Matrix::CreateRotationY();
		//meshManager->Entry(5, world);
	}
	/**
	* 範囲表示のテクスチャを生成する
	* @param context -- デバイスコンテキスト
	* @param state   -- CommonState
	*/
	void CreateTexture(ID3D11DeviceContext* context, CommonStates* state)
	{
		Graphics* graphics = Graphics::getInstance();
		// 書き込むレンダーを切り替える
		auto& main = renderTarget[renderNum];
		renderNum = 1 - renderNum;
		auto& sub = renderTarget[renderNum];
		// 上方向から見るカメラを設定
		Matrix aboveView = Matrix::CreateLookAt(
			Vector3(0, 900, 0),
			Vector3(0, 0, 0),
			Vector3(1, 0, 0));
		Matrix aboveProj = Matrix::CreatePerspectiveFieldOfView(
			XMConvertToRadians(45), 1280.f / 720.f, 10, 1000);
		Matrix world = Matrix::Identity;
		world *= aboveView * aboveProj;
		// コンスタントバッファを設定
		matrixesConstant->AboveWVP = world.Transpose();
		static float time = 0.f;
		time += 0.2f;
		// モデルを描画
		main->BeginAndClear(context, Color(1, 1, 1, 1));
		meshManager->Render(state, aboveView, aboveProj);
		main->End(context);
		meshManager->Reset();
		// 透明度を変える
		sub->BeginAndClear(context, Color(1, 1, 1, 1));
		graphics->getSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
			graphics->getCommonStates()->NonPremultiplied(),
			graphics->getCommonStates()->PointClamp());
		RECT rect = rect = { 0,0,1280 * 2,720 * 2 };
		graphics->getSpriteBatch()->Draw(main->GetResourceView(), rect, Color(1, 1, 1, sinf(time) + 1.0f));
		graphics->getSpriteBatch()->End();
		sub->End(context);
	}
	/**
	* DirectX::ModelのCustomStateを設定する関数
	* @param context -- デバイスコンテキスト
	* @param state   -- CommonState
	*/
	std::function<void(void)> CustomState(ID3D11DeviceContext* context, CommonStates* state)
	{
		return [this, context, state]() {
			// mapとunmapの処理
			matrixesConstant->SetData(context);
			// バーテックスシェーダの設定
			context->VSSetShader(rangeVS.Get(), nullptr, 0);
			context->PSSetShader(rangePS.Get(), nullptr, 0);
			// バーテックスシェーダに渡すコンスタントバッファ
			ID3D11Buffer *buffer[] = {
				matrixesConstant->GetBuffer(),
			};

			context->VSSetConstantBuffers(0, 1, buffer);
			context->PSSetConstantBuffers(0, 1, buffer);
			// テクスチャの設定
			ID3D11ShaderResourceView *textures[] = { renderTarget[renderNum]->GetResourceView() };
			context->PSSetShaderResources(1, 1, textures);

			ID3D11SamplerState *samplers[] = { state->LinearClamp() };
			context->PSSetSamplers(1, 1, samplers);
		};
	}
};
/**
* コンストラクタ
*/
RangeMap::RangeMap() : impl_(new Impl())
{
}

/**
* デストラクタ
*/
RangeMap::~RangeMap()
{
	instance_ = nullptr;
}
/**
* 円範囲範囲を追加する関数
* @param circle -- Collision2DのCircleポインタ
*/
void RangeMap::Add(Circle* circle)
{
	impl_->Add(circle);
}
/**
* 矩形範囲を追加する関数
* @param square -- Collision2DのSquareポインタ
*/
void RangeMap::Add(Square* square)
{
	impl_->Add(square);
}
/**
* 扇範囲を追加する関数
* @param fanShape -- Collision2DのFanShapeポインタ
*/
void RangeMap::Add(FanShape* fanShape)
{
	impl_->Add(fanShape);
}
/**
* 範囲表示のテクスチャを生成する関数
* @param context -- デバイスコンテキスト
* @param state   -- CommonState
*/
void RangeMap::CreateTexture(ID3D11DeviceContext* context, CommonStates* state)
{
	impl_->CreateTexture(context, state);
}
/**
* DirectX::ModelのCustomStateを設定する関数
* @param context -- デバイスコンテキスト
* @param state   -- CommonState
*/
std::function<void(void)> RangeMap::CustomState(ID3D11DeviceContext* context, CommonStates* state)
{
	return impl_->CustomState(context, state);
}
/**
* コンスタントバッファを設定する関数（視点カメラのWorldViewProjection行列）
* @param wvp -- 画面を写しているカメラのViewProjection
*/
void RangeMap::setWorldViewProjection(const DirectX::SimpleMath::Matrix& wvp)
{
	impl_->matrixesConstant->WVP = wvp;
}
/**
* コンスタントバッファを設定する関数（ステージを見下ろす視点のカメラ）
* @param wvp -- 画面を上から見ているWorldViewProjection行列
*/
void RangeMap::setAboveWorldViewProjection(const DirectX::SimpleMath::Matrix& wvp)
{
	impl_->matrixesConstant->AboveWVP = wvp;
}
/**
* 範囲表示のテクスチャを取得する関数
*/
ID3D11ShaderResourceView* RangeMap::getResourceView()
{
	return impl_->renderTarget[0]->GetResourceView();
}