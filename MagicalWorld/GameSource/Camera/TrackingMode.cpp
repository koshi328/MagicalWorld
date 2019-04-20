/**
* File: TrackingMode.cpp
*
* �Ώۂ�ǔ�����J�������[�h
*
* Date: 2016.11.10
* Author: koshi ito
*/
#include "TrackingMode.h"
#include "..\..\Utility\Mouse.h"
#include "Camera.h"
#include <SimpleMath.h>
#include "..\..\Utility\Key.h"

using namespace DirectX::SimpleMath;

TrackingMode::TrackingMode(GameObject* target)
{
	target_ = target;
	Mouse::getInstance()->setMode(Mouse::MODE_RELATIVE);
	x_ = 0.0f;
	y_ = -30 * 3.14f / 180;
	distance_ = 100.0f;
}

TrackingMode::~TrackingMode()
{
}

void TrackingMode::Update()
{
	// �J�����̋�������
	if (Key::getInstance()->Press(VK_UP))
	{
		distance_ += 0.1f;
	}
	if (Key::getInstance()->Press(VK_DOWN))
	{
		distance_ -= 0.1f;
	}
	// �^�[�Q�b�g�����Ȃ��ꍇ�����𒆒f
	if (!target_) return;
	// �^�[�Q�b�g�����ł��鎞�̃��b�Z�[�W���󂯎��
	if (target_->DestroyMessage())
	{
		target_ = nullptr;
	}
	// �}�E�X�����]�ʂ��擾
	x_ -= Mouse::getInstance()->X() * 0.001f;
	y_ -= Mouse::getInstance()->Y() * 0.001f;

	// ����
	if (y_ >= 1.57f) y_ = 1.57f;
	if (y_ <= -1.57f) y_ = -1.57f;
	// ��]�ʂ�K�p�����ʒu���Z�o
	Vector3 pos(0, 0, distance_);
	Matrix rotX = Matrix::CreateRotationX(y_);
	Matrix rotY = Matrix::CreateRotationY(x_);
	Matrix rot = rotX * rotY;
	pos = Vector3::Transform(pos, rot);

	// �J�����ɓK�p
	Vector3 offset(0, distance_ * 0.3f, 0);
	camera_->SetRefpos(target_->Position() + offset);
	camera_->SetEyepos(target_->Position() + pos + offset);
}
