/**
* File: TrackingMode.cpp
*
* �Ώۂ�ǔ�����J�������[�h
*
* Date: 2016.11.10
* Author: koshi ito
*/
#pragma once

#include "CameraMode.h"
#include "..\Object\GameObject.h"

class TrackingMode : public CameraMode
{
private:
	GameObject* target_;	// �ǔ��Ώ�
	float x_;				// X����]��
	float y_;				// Y����]��
	float distance_;		// �ΏۂƂ̋���
public:
	// �R���X�g���N�^
	TrackingMode(GameObject* target);
	// �f�X�g���N�^
	~TrackingMode();
	// �X�V
	void Update() override;
};