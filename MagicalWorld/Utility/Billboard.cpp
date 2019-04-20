/**
* File: Billboard.cpp
*
* �r���{�[�h��\������N���X
*
* Date: 2016.9.20
* Author: koshi ito
*/
#include "Billboard.h"
#include <WICTextureLoader.h>
/**
* ���O���
*/
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;
/**
* static�ϐ�
*/
std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> Billboard::s_textureArray;
Microsoft::WRL::ComPtr<ID3D11InputLayout> Billboard::inputLayout;
std::unique_ptr<DirectX::AlphaTestEffect> Billboard::effect;
std::unique_ptr<PrimitiveBatch<DirectX::VertexPositionTexture>> Billboard::shadowBatch;

// �����̂̒��_�f�[�^
DirectX::VertexPositionTexture g_vertexes[4] =
{
	DirectX::VertexPositionTexture(Vector3(-0.5f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)),	 // 0
	DirectX::VertexPositionTexture(Vector3(0.5f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)),	 // 1
	DirectX::VertexPositionTexture(Vector3(-0.5f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)),	 // 2
	DirectX::VertexPositionTexture(Vector3(0.5f, 0.0f, 0.0f), Vector2(1.0f, 1.0f))	 // 3
};
// ���_�̏���
uint16_t g_indexes[6] =
{
	0, 1, 2,
	1, 3, 2
};
/**
* �R���X�g���N�^
*/
Billboard::Billboard()
{
}

/**
* �f�X�g���N�^
*/
Billboard::~Billboard()
{
}
/**
* ������
*/
void Billboard::Initialize()
{
	// �A���t�@�e�X�g�G�t�F�N�g�̍쐬
	effect.reset(new AlphaTestEffect(Graphics::getInstance()->getDevice()));
	effect->SetAlphaFunction(D3D11_COMPARISON_GREATER);
	effect->SetReferenceAlpha(254);
	// ���̓��C�A�E�g�̍쐬
	const void* buf;
	size_t size;
	// �g�p����V�F�[�_�[�v���O�����ƃT�C�Y���擾����
	effect->GetVertexShaderBytecode(&buf, &size);
	Graphics::getInstance()->getDevice()->CreateInputLayout(
		DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		buf, size, inputLayout.GetAddressOf());

	// �v���~�e�B�u��\������ׂ̃I�u�W�F�N�g���쐬����
	shadowBatch.reset(new PrimitiveBatch<DirectX::VertexPositionTexture>(Graphics::getInstance()->getDeviceContext()));
}
/**
* �e�N�X�`����ǂݍ���
* @param file_name -- �t�@�C���̃p�X
*/
void Billboard::LoadTexture(const wchar_t* file_name)
{
	// �������O�̃��f����ǂݍ��ݍς݂łȂ����
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
* �`��
* @param world -- World���W
* @param view  -- �J����View
* @param proj  -- �J����Projection
*/
void Billboard::Draw(Matrix world, Matrix view, Matrix proj)
{
	// �r���{�[�h�ɂ���
	world = CreateBillboardMatrix(view) * world;
	effect->SetWorld(world);
	effect->SetView(view);
	effect->SetProjection(proj);
	effect->SetTexture(texture_);
	effect->Apply(Graphics::getInstance()->getDeviceContext());
	// ���̓��C�A�E�g�̐ݒ� //
	Graphics::getInstance()->getDeviceContext()->IASetInputLayout(inputLayout.Get());
	// �A���t�@�̐ݒ�
	Graphics::getInstance()->getDeviceContext()->OMSetBlendState(Graphics::getInstance()->getCommonStates()->NonPremultiplied(), nullptr, 0xffffffff);

	shadowBatch->Begin();
	shadowBatch->DrawIndexed(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		g_indexes, 6,	// ���_�̏���
		g_vertexes, 4	// ���_�̏��
		);
	shadowBatch->End();
}
/**
* �r���{�[�h�}�g���N�X���v�Z
*/
Matrix Billboard::CreateBillboardMatrix(Matrix view)
{
	// �]�u�s������߂�
	Matrix m = view.Transpose();
	// ���s�ړ��v�f�͖�����
	m._41 = m._42 = m._43 = 0.0f;
	m._44 = 1.0f;

	return m;
}
