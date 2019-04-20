/**
* File: RangeMap.h
*
* �U���\���͈͂�\������N���X
*
* Date: 2017.2.1
* Author: koshi ito
*/
#pragma once
#include <memory>
#include <d3d11.h>
#include <functional>
#include <CommonStates.h>
#include <SimpleMath.h>

/**
* �O���錾
*/
class Circle;
class Square;
class FanShape;

class RangeMap
{
public:
	// �V���O���g��
	static RangeMap* getInstance()
	{
		if (!instance_)
			instance_ = new RangeMap();
		return instance_;
	}
	// �f�X�g���N�^
	~RangeMap();
	// �\���͈͂�ǉ�����
	void Add(Circle* circle);
	void Add(Square* square);
	void Add(FanShape* fanShape);
	// �\���͈͂�`�����e�N�X�`���𐶐�����
	void CreateTexture(ID3D11DeviceContext* context, DirectX::CommonStates* state);
	// DirectX::Model��CustomState��ݒ肷��֐�
	std::function<void(void)> CustomState(ID3D11DeviceContext* context, DirectX::CommonStates* state);
	// �R���X�^���g�o�b�t�@��ݒ肷��
	void setWorldViewProjection(const DirectX::SimpleMath::Matrix& wvp);
	void setAboveWorldViewProjection(const DirectX::SimpleMath::Matrix& wvp);
	// �͈̓e�N�X�`�����擾����
	ID3D11ShaderResourceView* getResourceView();
private:
	// �C���v�������g�N���X
	class Impl;
	std::unique_ptr<Impl> impl_;
	// �R���X�g���N�^
	RangeMap();
	static RangeMap* instance_; // �C���X�^���X
};

