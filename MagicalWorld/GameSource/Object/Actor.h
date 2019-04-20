/**
* File: Actor.h
*
* �Q�[���ɓo�ꂷ��L�����N�^�[�̊��
*
* Date: 2016.11.20
* Author: koshi ito
*/
#pragma once
#include "GameObject.h"


class Actor : public GameObject
{
protected:
	int maxHp_;			// �ő�̗�
	int hp_;			// �̗�
	int attack_;		// �U����(����)
	int defense_;		// �h���(����)

	bool fallFlag_;		// �����t���O
	bool damageFlag_;	// �_���[�W���󂯂Ă���t���O
	DirectX::SimpleMath::Vector3 velocity_; // ���x
private:
	static bool soundFlag_;
public:
	// �R���X�g���N�^
	Actor();
	// �f�X�g���N�^
	virtual ~Actor();
	// �O�i�����ւ̃x�N�g�������߂�
	const DirectX::SimpleMath::Vector3 MoveForward(float speed);
	// �d�͂��|����
	void useGravity();
	// World���W���Z�o
	const DirectX::SimpleMath::Matrix CreateWorldMatrix();

	// �A�N�Z�b�T
	// MaxHP
	inline int getMaxHP() { return maxHp_; };
	inline void setMaxHP(int data) { maxHp_ = data; }
	// HP
	inline int getHP() { return hp_; };
	inline void setHP(int data) { hp_ = data; }
	// Attack
	inline int getAttack() { return attack_; }
	inline void setAttack(int data) { attack_ = data; }
	// Defense
	inline int getDefense() { return defense_; }
	inline void setDefense(int data) { defense_ = data; }
	// fallFlag
	inline bool getFall() { return fallFlag_; }
	inline void setFall(bool data) { fallFlag_ = data; }
	// Velocity
	inline const DirectX::SimpleMath::Vector3& getVelocity() { return velocity_; }
	inline void setVelocity(const DirectX::SimpleMath::Vector3& vel) { velocity_ = vel; }
	inline void setVelocityX(const float& speed) { velocity_.x = speed; }
	inline void setVelocityY(const float& speed) { velocity_.y = speed; }
	inline void setVelocityZ(const float& speed) { velocity_.z = speed; }

	// �_���[�W���󂯂�
	virtual void ReceiveDamage(Actor* sender, const int& attack, const int& hate = 0);

	static void SoundReset();
};
// �ŒZ�p�x�����߂�
float shortAngle(float start, float end);