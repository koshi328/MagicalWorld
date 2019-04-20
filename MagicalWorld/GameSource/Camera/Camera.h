/**
* File: Camera.h
*
* �J�����̎p�����i��
*
* Date: 2016.11.10
* Author: koshi ito
*/
#pragma once
#include <SimpleMath.h>
class Camera
{
protected:
	// ���_
	DirectX::SimpleMath::Vector3 _eye;
	// �^�[�Q�b�g
	DirectX::SimpleMath::Vector3 _target;
	// ������x�N�g��
	DirectX::SimpleMath::Vector3 _up;
	// view�s��
	DirectX::SimpleMath::Matrix _view; 
	// proj�s��
	DirectX::SimpleMath::Matrix _proj;
	// �c��������p
	float _fovY;
	// �A�X�y�N�g��
	float _aspect;
	// �j�A�N���b�v
	float _nearClip;
	// �t�@�[�N���b�v
	float _farClip;
public:
	Camera();
	virtual ~Camera();
	void Update();

	// �A�N�Z�b�T
	void SetEyepos(const DirectX::SimpleMath::Vector3 pos) { _eye = pos; }
	void SetRefpos(const DirectX::SimpleMath::Vector3 pos) { _target = pos; }
	void SetUpvec(const DirectX::SimpleMath::Vector3 vec) { _up = vec; }
	const DirectX::SimpleMath::Vector3& GetEyepos() const { return _eye; }
	const DirectX::SimpleMath::Vector3& GetRefpos() const { return _target; }
	const DirectX::SimpleMath::Vector3& GetUpvec() const { return _up; }
	const DirectX::SimpleMath::Matrix& GetViewmat() const { return _view; }
	const DirectX::SimpleMath::Matrix&GetProjmat()const { return _proj; }

	void SetFovY(float fovY) { _fovY = fovY; }
	void SetAspect(float aspect) { _aspect = aspect; }
	void SetNearClip(float nearClip) { _nearClip = nearClip; }
	void SetFarClip(float FarClip) { _nearClip = FarClip; }
};