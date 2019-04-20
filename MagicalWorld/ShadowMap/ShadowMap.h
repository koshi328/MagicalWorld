#pragma once
#include <wrl.h>
#include <memory>
#include "ConstantBuffer.h"
#include "..\Utility\RenderTarget.h"
#include <DirectXMath.h>

struct MatrixesConstant
{
	DirectX::XMMATRIX WorldViewProjection;
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX LightViewProjection;

	using Type = ConstantBuffer<MatrixesConstant>;
};

struct Light
{
	DirectX::XMVECTOR LightDirection; // ライトの向き
	DirectX::XMVECTOR LightDiffuse; // ライトのディフューズ
	DirectX::XMVECTOR LightAmbient; // ライトのアンビエント
	using Type = ConstantBuffer<Light>;
};

struct MaterialsConstant
{
	DirectX::XMVECTOR MaterialDiffuse;
	DirectX::XMVECTOR MaterialAmbient;

	using Type = ConstantBuffer<MaterialsConstant>;
};

class ShadowMap
{
public:
	static ShadowMap* getInstance()
	{
		if (!instance_)
			instance_ = new ShadowMap();
		return instance_;
	}
	~ShadowMap();

	void Update();
	void Render();

private:
	// シーン描画用
	Microsoft::WRL::ComPtr<ID3D11VertexShader> sceneVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> scenePS;
	// シャドウマップ用
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shadowVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shadowPS;

	std::unique_ptr<MatrixesConstant::Type> matrixesConstant;	// 行列関係
	std::unique_ptr<MaterialsConstant::Type> materialsConstant;	// マテリアル関係
	std::unique_ptr<Light::Type> lightConstant;
	std::unique_ptr<RenderTarget> renderTarget;

	ShadowMap();
	static ShadowMap* instance_;
};