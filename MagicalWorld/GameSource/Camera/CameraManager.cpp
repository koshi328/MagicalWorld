/**
* File: CameraManager.cpp
*
* �J�����𐧌䂷��}�l�[�W���[
*
* Date: 2016.11.10
* Author: koshi ito
*/
#include "CameraManager.h"
#include "CameraMode.h"

using namespace DirectX::SimpleMath;

// static�̎��̉�
CameraManager* CameraManager::instance_;

/**
* �R���X�g���N�^
*/
CameraManager::CameraManager()
{
	active_ = true;
	camera_ = new Camera;
	shaking_.reset(new Shaking);
}

/**
* �f�X�g���N�^
*/
CameraManager::~CameraManager()
{
	delete mode_;
	delete camera_;
	instance_ = nullptr;
}

/**
* �X�V
*/
void CameraManager::Update()
{
	// �A�N�e�B�u����
	if (!active_) return;
	// �G�t�F�N�g
	// �h��
	shaking_->Execute(camera_);
	// �J�����X�V
	camera_->Update();
	// �J�����̃��[�h
	if(mode_)
		mode_->Update();
}

/**
* �J�����̃��[�h�ύX
* @param mode --- CameraMode�̔h���N���X
*/
void CameraManager::setMode(CameraMode* mode)
{
	if (mode_)
		delete mode_;
	mode_ = mode;

	if(mode_)
		mode_->setCamera(camera_);
}

/**
* �h��G�t�F�N�g�̐ݒ�
*/
void CameraManager::ShakingMode(int time, float intensity)
{
	shaking_->Set(time, intensity);
}

/**
* �J�����G�t�F�N�g
*/

/**
* �h��G�t�F�N�g
*/
/**
* �R���X�g���N�^
*/
Shaking::Shaking()
{

}
/**
* �f�X�g���N�^
*/
Shaking::~Shaking()
{

}

/**
* �ݒ�
* @param time      --- ����
* @param intensity --- �K�̓��x��
*/
void Shaking::Set(int time, float intensity)
{
	time_ = time;
	intensity_ = intensity;
	flameWidth_ = 3.14f * 10 / time_;
	attenuation_ = intensity_ / time_;
}

/**
* ���s
* @param camera --- �K�p����J�����̃|�C���^
*/
bool Shaking::Execute(Camera* camera)
{
	if (--time_ < 0) return false;
	intensity_ -= attenuation_;
	float shak = sinf(flameWidth_ * time_) * intensity_;

	Vector3 eye = camera->GetEyepos();
	Vector3 ref = camera->GetRefpos();
	eye.y += shak;
	ref.y += shak;
	camera->SetEyepos(eye);
	camera->SetRefpos(ref);
}