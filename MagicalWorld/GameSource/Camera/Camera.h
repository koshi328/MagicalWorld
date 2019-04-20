/**
* File: Camera.h
*
* カメラの姿勢を司る
*
* Date: 2016.11.10
* Author: koshi ito
*/
#pragma once
#include <SimpleMath.h>
class Camera
{
protected:
	// 視点
	DirectX::SimpleMath::Vector3 _eye;
	// ターゲット
	DirectX::SimpleMath::Vector3 _target;
	// 上方向ベクトル
	DirectX::SimpleMath::Vector3 _up;
	// view行列
	DirectX::SimpleMath::Matrix _view; 
	// proj行列
	DirectX::SimpleMath::Matrix _proj;
	// 縦方向視野角
	float _fovY;
	// アスペクト比
	float _aspect;
	// ニアクリップ
	float _nearClip;
	// ファークリップ
	float _farClip;
public:
	Camera();
	virtual ~Camera();
	void Update();

	// アクセッサ
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