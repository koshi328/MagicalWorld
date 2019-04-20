/**
* File: GameObject.h
*
* �Q�[���ɓo�ꂷ��I�u�W�F�N�g�̊��N���X
*
* Date: 2016.10.15
* Author: koshi ito
*/
#pragma once
#include <Simplemath.h>
class Collision;

class GameObject
{
protected:
	DirectX::SimpleMath::Vector3 position_;	// ���W
	DirectX::SimpleMath::Vector3 rotation_;	// ��]
	DirectX::SimpleMath::Vector3 scale_;	// �g�k

	// �I�u�W�F�N�g�̏��Ńt���O
	bool isDestroy;
public:
	// �R���X�g���N�^
	GameObject();
	// �f�X�g���N�^
	virtual ~GameObject();

	// �A�N�Z�b�T
	DirectX::SimpleMath::Vector3& Position() { return position_; }
	void Position(DirectX::SimpleMath::Vector3& position) { position_ = position; }
	DirectX::SimpleMath::Vector3& Rotation() { return rotation_; }
	void Rotation(DirectX::SimpleMath::Vector3& rotation) { rotation_ = rotation; }
	DirectX::SimpleMath::Vector3& Scale() { return scale_; }
	void Scale(DirectX::SimpleMath::Vector3& scale) { scale_ = scale; }

	// �Փ˔���ւ̃A�N�Z�X
	virtual Collision* getCollision() { return nullptr; }

	// �I�u�W�F�N�g�̏���
	bool DestroyMessage() { return isDestroy; }
	void Destroy(GameObject* p) { p->isDestroy = true; }
	// ������
	virtual void Initialize() = 0;
	// �X�V
	virtual void Update() = 0;
	// �`��
	virtual void Draw() = 0;
};