/**
* File: RenderTarget.h
*
* �����_�[�^�[�Q�b�g
*
* Date: 2017.1.15
* Author: koshi ito
*/
#pragma once

#include <memory>
#include <SimpleMath.h>

class RenderTarget
{
public:
	// �R���X�g���N�^
	RenderTarget();
	// �f�X�g���N�^
	~RenderTarget();

	using Color = DirectX::SimpleMath::Color;
	// �I�u�W�F�N�g�̍쐬
	void Create(ID3D11Device * device, UINT width, UINT height, DXGI_FORMAT targetFormat, DXGI_FORMAT depthFormat);
	// �����_�[�^�[�Q�b�g�̕ύX
	void Begin(ID3D11DeviceContext *context);
	// �����_�[�^�[�Q�b�g�̕ύX�ƃN���A
	void BeginAndClear(ID3D11DeviceContext *context, Color color);
	// �����_�[�^�[�Q�b�g�����ɖ߂�
	void End(ID3D11DeviceContext *context);
	// �e�N�X�`�����擾
	ID3D11ShaderResourceView* GetResourceView();

private:
	// �C���v�������g�N���X
	class Impl;
	std::unique_ptr<Impl> pImpl_;
};