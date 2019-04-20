/**
* File: MeshManager.h
* ���f���̕`��Ǘ��N���X
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
	// �`��ɕK�v�ȏ���Z�߂��\����
	struct ModelInfo
	{
		UINT model_; // ���f���ԍ�
		DirectX::SimpleMath::Matrix world_; // World���W
		std::function<void()> lamda_; // �֐�
	};
private:
	// �f�o�C�X�R���e�L�X�g�̃|�C���^
	ID3D11DeviceContext* context_;
	// �f�o�C�X�̃|�C���^
	ID3D11Device* device_;

	// �`�悵�������f���̊m�۔z��
	static std::unique_ptr <std::unique_ptr<DirectX::Model>[]> modelList_;
	// �`�悷�郂�f���̔z��
	std::vector<ModelInfo> drawList_;
	// �o�^�ő吔
	UINT maxNum_;
public:
	// �f�t�H���g�R���X�g���N�^
	MeshManager();
	// �R���X�g���N�^
	MeshManager(UINT um, ID3D11DeviceContext* context, ID3D11Device* device);
	// �f�X�g���N�^
	~MeshManager();
	// ������
	void Initialize(UINT um, ID3D11DeviceContext* context, ID3D11Device* device);
	// ���f���ǂݍ���
	bool LoadModel(UINT num, wchar_t* file, DirectX::DGSLEffectFactory* factory);
	// ���f���o�^
	void Entry(UINT num, DirectX::SimpleMath::Matrix world, std::function<void()> func = nullptr);
	// �`��
	void Render(DirectX::CommonStates* state, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	// ���Z�b�g
	void Reset();
	// Draw���X�g�̎擾
	std::vector<ModelInfo>& getDrawList() { return drawList_; }
};
