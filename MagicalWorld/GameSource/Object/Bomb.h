/**
* File: Bomb.h
*
* ���e�N���X
*
* Date: 2016.12.25
* Author: koshi ito
*/
#pragma once
#include "..\Collision\Collision.h"
#include "Actor.h"

class Bomb : public Actor
{
private:
	Sphere collision_;	// �Փ˔���R���C�_�[
	int lifeTime_;		// �����\����
	int effectDilay_;	// �G�t�F�N�g�̊Ԋu
public:
	Bomb(DirectX::SimpleMath::Vector3 position, int lifeTime);
	~Bomb();

	// �Փ˔���ւ̃A�N�Z�X
	Collision* getCollision();

	void Initialize();
	void Update();
	void Draw();
};