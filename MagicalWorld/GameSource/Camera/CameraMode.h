/**
* File: CameraMode.h
*
* �J�����̏�Ԃ̊��N���X
*
* Date: 2016.11.10
* Author: koshi ito
*/
#pragma once
// ��s�錾
class Camera;

class CameraMode
{
protected:
	Camera* camera_;	// ���삷��J�����̃|�C���^
public:
	// �R���X�g���N�^
	CameraMode() = default;
	// �f�X�g���N�^
	virtual ~CameraMode() = default;

	// �X�V
	virtual void Update() = 0;
	// �A�N�Z�b�T
	void setCamera(Camera* camera)
	{
		camera_ = camera;
	}
};