/**
* File: Actor.cpp
*
* �Q�[���ɓo�ꂷ��L�����N�^�[�̊��
*
* Date: 2016.11.20
* Author: koshi ito
*/

#include "Actor.h"
#include <SimpleMath.h>
#include "..\Camera\CameraManager.h"
#include "..\Camera\Camera.h"
#include "..\..\Utility\TaskManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "..\Effect\MessageUI.h"
#include "..\Effect\HitEffect.h"
#include "Player.h"
#include "Enemy.h"
// �T�E���h
#include "..\..\Sound\ADX2Le.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"

/**
* ���O���
*/
using namespace DirectX::SimpleMath;
/**
* �O���[�o���֐�
*/
extern TaskManager* getTaskManager();
extern ParticleManager* getParticleManager();

/**
* static�ϐ�
*/
bool Actor::soundFlag_ = false;

/**
* �ŒZ�p�x�����߂�
* @param start -- ���݂̊p�x
* @param end   -- �ڕW�p�x
* @return(float) -- �ŒZ�p�x
*/
float shortAngle(float start, float end)
{
	float angle = (end - start) * 180.0f / 3.14f;
	if (angle >= 180.0f)
	{
		angle -= 360.0f;
	}
	return angle * 3.14f / 180.0f;
}
/**
* �R���X�g���N�^
*/
Actor::Actor()
{
}
/**
* �f�X�g���N�^
*/
Actor::~Actor()
{
}

/**
* �O���ړ�
* @param speed -- �ړ����x
* @return(Vector3) �ړ���
*/
const Vector3 Actor::MoveForward(float speed)
{
	// ��]��
	Matrix rot = Matrix::CreateRotationY(rotation_.y);
	// �ړ���
	Vector3 vel(0, 0, speed);
	vel = Vector3::Transform(vel, rot);
	return vel;
}

/**
* ��������
* @param ����
* @return(float) �d�͂ɂ������x
*/
void Actor::useGravity()
{
	if (!fallFlag_)
		velocity_.y = 0.0f;
	else
	{
		if (velocity_.y >= -3.0f)
			velocity_.y -= 0.098f;
	}
}

/**
* �_���[�W���󂯂鏈��
* @param sender -- �U������������
* @param attack -- �U����
* @param hate   -- �U���ɂ���đ�����G��
*/
void Actor::ReceiveDamage(Actor* sender, const int& attack, const int& hate)
{
	if (damageFlag_) return;
	velocity_.y = 2.0f;
	position_.y += velocity_.y;
	fallFlag_ = true;
	damageFlag_ = true;
	int damage = (attack - defense_);
	if (damage < 0) damage = 1;
	damage += (int)(damage * ((rand() / 100) * 0.01f));

	hp_ -= damage;
	if (hp_ <= 0) hp_ = 0;

	// �_���[�W�����̕\���o�^
	Camera* camera = CameraManager::getInstance()->getCamera();
	wchar_t buf[32];
	swprintf_s(buf, 32, L"%d", damage);

	if (Player* p = dynamic_cast<Player*>(sender))
		getTaskManager()->Add<MessageUI>()->Initialize(camera->GetViewmat(), camera->GetProjmat(), 1280, 720, this->Position(), 120, buf, Color(1, 1, 0, 1));
	if (Enemy* p = dynamic_cast<Enemy*>(sender))
		getTaskManager()->Add<MessageUI>()->Initialize(camera->GetViewmat(), camera->GetProjmat(), 1280, 720, this->Position(), 120, buf, Color(1, 0, 0, 1));
	getTaskManager()->Add<HitEffect>()->Initialize(getParticleManager(), position_, getCollision()->getMaxRange() * 5, 20);
	if(!soundFlag_)
		ADX2Le::Play(CRI_CUESHEET_0_SE_HIT);
	soundFlag_ = true;
}

/**
* World�s��𐶐�����
* @return(Matrix) -- World�s��
*/
const DirectX::SimpleMath::Matrix Actor::CreateWorldMatrix()
{
	Matrix world = Matrix::Identity;
	Matrix t = Matrix::CreateTranslation(position_);
	Matrix r = Matrix::CreateFromYawPitchRoll(rotation_.y, rotation_.x, rotation_.z);
	Matrix s = Matrix::CreateScale(scale_);
	world = s * r * t;
	return world;
}

void Actor::SoundReset()
{
	soundFlag_ = false;
}