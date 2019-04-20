/**
* File: CameraManager.h
*
* �J�����𐧌䂷��}�l�[�W���[
*
* Date: 2016.11.10
* Author: koshi ito
*/
#pragma once

#include <memory>
#include "Camera.h"

// ��s�錾
class CameraMode;
class CameraEffect;
class Shaking;

class CameraManager
{
private:
	bool active_;
	Camera* camera_;					// �J����
	CameraMode* mode_;					// �J�����̏��
	static CameraManager* instance_;	// ���g�̃C���X�^���X

	// �G�t�F�N�g
	std::unique_ptr<Shaking> shaking_;
	// �R���X�g���N�^
	CameraManager();
public:
	// �V���O���g��
	static CameraManager* getInstance()
	{
		if (!instance_)
			instance_ = new CameraManager;
		return instance_;
	}
	// �f�X�g���N�^
	~CameraManager();
	// �X�V
	void Update();
	// �J�������擾
	Camera* getCamera() { return camera_; }
	// ���[�h��ύX
	void setMode(CameraMode* mode);

	// �J������Active��Ԃ𐧌�
	bool Active() { return active_; }
	void Active(bool input) { active_ = input; }

	// �G�t�F�N�g�̐ݒ�
	void ShakingMode(int time, float intensity);
};

// �J������U��������G�t�F�N�g
class Shaking
{
private:
	int time_;			// ����
	float intensity_;	// �K��
	float flameWidth_;	// �h�ꕝ
	float attenuation_;	// ������
public:
	// �R���X�g���N�^
	Shaking();
	// �f�X�g���N�^
	~Shaking();
	// �ݒ�
	void Set(int time, float intensity);
	// ���s
	bool Execute(Camera* camera);
};