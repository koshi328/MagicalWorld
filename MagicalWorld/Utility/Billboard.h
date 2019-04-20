/**
* File: Billboard.h
*
* �r���{�[�h��\������N���X
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
	// �R���X�g���N�^
	Billboard();
	// �f�X�g���N�^
	~Billboard();
	// ������
	static void Initialize();
	// �ǂݍ���
	void LoadTexture(const wchar_t* file_name);
	// �`��
	void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	// �r���{�[�hMatrix���v�Z
	DirectX::SimpleMath::Matrix Billboard::CreateBillboardMatrix(DirectX::SimpleMath::Matrix view);
private:
	// �ǂݍ��ݍς݃e�N�X�`���[�R���e�i
	static std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> s_textureArray;
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	static std::unique_ptr<DirectX::AlphaTestEffect> effect;
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> shadowBatch;

private:
	// �`��e�N�X�`��
	ID3D11ShaderResourceView* texture_;
};

