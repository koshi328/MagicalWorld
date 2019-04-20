/**
* File: Item.h
*
* �A�C�e���N���X
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once
#include "..\Collision\Collision.h"
#include "Actor.h"
class Player;

class Item : public Actor
{
private:
	Sphere collision_;	// �R���C�_�[
	Player* owner_;		// �擾��
	int lifeTime_;		// ��������
	int type_;			// �A�C�e���̎��
public:
	// �R���X�g���N�^
	Item(DirectX::SimpleMath::Vector3 position, int lifeTime);
	// �f�X�g���N�^
	~Item();

	// �Փ˔���ւ̃A�N�Z�X
	Collision* getCollision();
	// �A�N�Z�b�T
	inline const int& getType() { return type_; }
	inline void setOwner(Player* owner) {if(!owner_) owner_ = owner; }
	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Draw();
};