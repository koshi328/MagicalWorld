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
	DirectX::XMVECTOR LightDirection; // ���C�g�̌���
	DirectX::XMVECTOR LightDiffuse; // ���C�g�̃f�B�t���[�Y
	DirectX::XMVECTOR LightAmbient; // ���C�g�̃A���r�G���g
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
	// �V�[���`��p
	Microsoft::WRL::ComPtr<ID3D11VertexShader> sceneVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> scenePS;
	// �V���h�E�}�b�v�p
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shadowVS;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shadowPS;

	std::unique_ptr<MatrixesConstant::Type> matrixesConstant;	// �s��֌W
	std::unique_ptr<MaterialsConstant::Type> materialsConstant;	// �}�e���A���֌W
	std::unique_ptr<Light::Type> lightConstant;
	std::unique_ptr<RenderTarget> renderTarget;

	ShadowMap();
	static ShadowMap* instance_;
};