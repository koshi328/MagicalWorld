/**
* File: MeshManager.cpp
* ���f���̕`��Ǘ��N���X
*
* Date: 2016.12.10
* Author: koshi ito
*/

#include "MeshManager.h"
#include "..\GameBase\Graphics.h"
#include <DirectXColors.h>

using namespace DirectX;
/**
* static�ϐ�
*/
std::unique_ptr <std::unique_ptr<DirectX::Model>[]> MeshManager::modelList_;

/**
* �O���[�o���ϐ�
*/
float timeCount;

/**
* �f�t�H���g�R���X�g���N�^
*/
MeshManager::MeshManager()
{
	timeCount = 0.f;
	maxNum_ = 0;
	context_ = nullptr;
	device_ = nullptr;
}

/**
* �R���X�g���N�^
* @param num -- �ǂݍ��ރ��f���̍ő吔
* @param context -- �f�o�C�X�R���e�L�X�g
*/
MeshManager::MeshManager(UINT num, ID3D11DeviceContext* context, ID3D11Device* device)
{
	// �f�o�C�X�|�C���^�̊m��
	context_ = context;
	device_ = device;
	// �ǂݍ��݃��f���̔z����m�ۂ���
	if (!modelList_)
	{
		modelList_.reset(new std::unique_ptr<Model>[num]);
	}
	maxNum_ = num;
}

/**
* �f�X�g���N�^
*/
MeshManager::~MeshManager()
{
	drawList_.clear();
}

void MeshManager::Initialize(UINT num, ID3D11DeviceContext* context, ID3D11Device* device)
{
	// �f�o�C�X�|�C���^�̊m��
	context_ = context;
	device_ = device;
	// �ǂݍ��݃��f���̔z����m�ۂ���
	if (!modelList_)
	{
		modelList_.reset(new std::unique_ptr<Model>[num]);
	}

	maxNum_ = num;
}

/**
* ���f���̓ǂݍ���
* @param num     -- �o�^����ԍ�
* @param file    -- �t�@�C���p�X
* @param factory -- �G�t�F�N�g�t�@�N�g���[
* @return(bool) true  -- �ǂݍ��ݐ���
* @return(bool) false -- �ǂݍ��ݎ��s
*/
bool MeshManager::LoadModel(UINT num, wchar_t* file, DGSLEffectFactory* factory)
{
	// �m�ۂ����z��𒴂����w�肪��������I��
	if (num >= maxNum_) return false;
	// ���f���̓ǂݍ���
	modelList_[num] = Model::CreateFromCMO(device_, file, *factory);

	return true;
}

/**
* �`��̓o�^
* @param num   -- �`�悷�郂�f���ԍ�
* @param world -- World���W
* @param func  -- �`����@�̎w��(HLSL��)
*/
void MeshManager::Entry(UINT num, DirectX::SimpleMath::Matrix world, std::function<void()> func)
{
	ModelInfo info;
	info.lamda_ = func;
	info.world_ = world;
	info.model_ = num;
	// �ǉ�
	drawList_.push_back(info);
}

/**
* �`��
* @param state      -- CommonState
* @param view       -- View�s��
* @param projection -- Projection�s��
*/
void MeshManager::Render(CommonStates* state, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	for each (ModelInfo data in drawList_)
	{
		modelList_[data.model_]->Draw(context_, *state, data.world_, view, projection, false, data.lamda_);
	}
}

/**
* �`��̓o�^�����Z�b�g
*/
void MeshManager::Reset()
{
	drawList_.clear();
}