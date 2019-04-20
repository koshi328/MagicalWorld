#include "ShadowMap.h"
#include "..\Utility\BinaryLoader.h"
#include "..\GameBase\Graphics.h"
#include "..\GameSource\Camera\CameraManager.h"
#include "..\Utility\MeshManager.h"
#include <SimpleMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

extern MeshManager* getMeshManager();

ShadowMap* ShadowMap::instance_ = nullptr;

ShadowMap::ShadowMap()
{
	ID3D11Device* device = Graphics::getInstance()->getDevice();
	// ファイルオブジェクト
	BinaryLoader buffer;
	// バーテックスシェーダの作成
	buffer.Load(L"ShadowMap/VertexShader.cso");
	device->CreateVertexShader(buffer.Data(), buffer.Size(), nullptr, sceneVS.GetAddressOf());
	// ピクセルシェーダの作成
	buffer.Load(L"ShadowMap/PixelShader.cso");
	device->CreatePixelShader(buffer.Data(), buffer.Size(), nullptr, scenePS.GetAddressOf());
	// シャドウマップ
	// バーテックスシェーダの作成
	buffer.Load(L"ShadowMap/ShadowVS.cso");
	device->CreateVertexShader(buffer.Data(), buffer.Size(), nullptr, shadowVS.GetAddressOf());
	// ピクセルシェーダの作成
	buffer.Load(L"ShadowMap/ShadowPS.cso");
	device->CreatePixelShader(buffer.Data(), buffer.Size(), nullptr, shadowPS.GetAddressOf());

	// コンスタントバッファ -----------------------------------------------
	matrixesConstant.reset(new MatrixesConstant::Type(device));
	materialsConstant.reset(new MaterialsConstant::Type(device));
	lightConstant.reset(new Light::Type(device));

	renderTarget = std::make_unique<RenderTarget>();
	renderTarget.reset(new RenderTarget());
	renderTarget->Create(device, 1280, 720, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);


	// コンスタントバッファ -----------------------------------------------
	materialsConstant->MaterialDiffuse = Vector4(1, 1, 1, 1);
	materialsConstant->MaterialAmbient = Vector4(0.2f, 0.2f, 0.2f, 1.0f);


	// ライトのビュー行列
	Matrix lightView = Matrix::CreateLookAt(
		Vector3(50.0f, 50.f, 0.0f),
		Vector3(0, 0, 0),
		Vector3(0, 1, 0)
		);
	// ライトのプロジェクション
	Matrix lightProj = Matrix::CreatePerspectiveFieldOfView(XMConvertToRadians(40), 1280.f / 720.f, 20.f, 10000);
	Matrix lightWVP = lightView * lightProj;
	matrixesConstant->LightViewProjection = lightWVP.Transpose();
}

ShadowMap::~ShadowMap()
{

}

void ShadowMap::Update()
{
}

void ShadowMap::Render()
{
	Graphics* graphics = Graphics::getInstance();
	MeshManager meshM = *getMeshManager();
	meshM.Reset();
	ID3D11DeviceContext* context = graphics->getDeviceContext();

	for each(MeshManager::ModelInfo info in getMeshManager()->getDrawList())
	{
		Matrix world = info.world_;
		Matrix wvp = Matrix::Identity;
		wvp *= world;
		wvp *= CameraManager::getInstance()->getCamera()->GetViewmat();
		wvp *= CameraManager::getInstance()->getCamera()->GetProjmat();

		// コンスタントバッファに設定
		matrixesConstant->WorldViewProjection = wvp.Transpose();
		matrixesConstant->World = world.Transpose();
		meshM.Entry(info.model_, info.world_, [&]
		{
			// mapとunmapの処理
			matrixesConstant->SetData(context);
			lightConstant->SetData(context);
			materialsConstant->SetData(context);
			// バーテックスシェーダの設定
			context->VSSetShader(sceneVS.Get(), nullptr, 0);
			context->PSSetShader(scenePS.Get(), nullptr, 0);
			// バーテックスシェーダに渡すコンスタントバッファ
			ID3D11Buffer *buffer[] = {
				matrixesConstant->GetBuffer(),
				lightConstant->GetBuffer(),
				materialsConstant->GetBuffer() };

			context->VSSetConstantBuffers(0, 3, buffer);
			context->PSSetConstantBuffers(0, 3, buffer);

			// テクスチャの設定
			ID3D11ShaderResourceView *textures[] = { nullptr };
			context->PSSetShaderResources(1, 1, textures);

			ID3D11SamplerState *samplers[] = { graphics->getCommonStates()->LinearClamp() };
			context->PSSetSamplers(1, 1, samplers);
		});
	}
	renderTarget->BeginAndClear(context, Color(1, 1, 1, 1));

	meshM.Render(graphics->getCommonStates(), CameraManager::getInstance()->getCamera()->GetViewmat(),
		CameraManager::getInstance()->getCamera()->GetProjmat());
	renderTarget->End(context);

	RECT rect = { 0,0,1280,720 };
	//graphics->getSpriteBatch()->Draw(renderTarget->GetResourceView(), rect, Color(1, 1, 1, 1));
	meshM.Reset();
	//for each(MeshManager::ModelInfo info in getMeshManager()->getDrawList())
	//{
	//	meshM.Entry(info.model_, info.world_, [&]
	//	{
	//		// mapとunmapの処理
	//		matrixesConstant->SetData(context);
	//		lightConstant->SetData(context);
	//		materialsConstant->SetData(context);
	//		// バーテックスシェーダの設定
	//		context->VSSetShader(shadowVS.Get(), nullptr, 0);
	//		context->PSSetShader(shadowPS.Get(), nullptr, 0);
	//		// バーテックスシェーダに渡すコンスタントバッファ
	//		ID3D11Buffer *buffer[] = {
	//			matrixesConstant->GetBuffer(),
	//			lightConstant->GetBuffer(),
	//			materialsConstant->GetBuffer() };

	//		context->VSSetConstantBuffers(0, 3, buffer);
	//		context->PSSetConstantBuffers(0, 3, buffer);

	//		// テクスチャの設定
	//		ID3D11ShaderResourceView *textures[] = { renderTarget->GetResourceView() };
	//		context->PSSetShaderResources(1, 1, textures);

	//		ID3D11SamplerState *samplers[] = { graphics->getCommonStates()->LinearClamp() };
	//		context->PSSetSamplers(1, 1, samplers);
	//	});
	//}

	//meshM.Render(graphics->getCommonStates(), CameraManager::getInstance()->getCamera()->GetViewmat(),
	//	CameraManager::getInstance()->getCamera()->GetProjmat());
}