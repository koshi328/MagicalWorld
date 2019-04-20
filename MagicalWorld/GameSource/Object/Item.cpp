/**
* File: Item.cpp
*
* �A�C�e���N���X
*
* Date: 2017.1.10
* Author: koshi ito
*/
#include "Item.h"
#include "..\..\Utility\MeshManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "..\..\Utility\TaskManager.h"
#include "..\Collision\Collision.h"
#include "..\PlayerSkill\SkillController.h"
#include "..\Object\Player.h"
#include "..\..\Sound\ADX2Le.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"


using namespace DirectX::SimpleMath;

extern MeshManager* getMeshManager();
extern ParticleManager* getParticleManager();
extern TaskManager* getTaskManager();
extern float randomRange(float min, float max);

/**
* �R���X�g���N�^
* @param position -> ���W
*/
Item::Item(Vector3 position, int lifeTime)
{
	position_ = position;
	lifeTime_ = lifeTime;
}

/**
* �f�X�g���N�^
*/
Item::~Item()
{

}

/**
* ������
*/
void Item::Initialize()
{
	// �X�P�[���̐ݒ�
	collision_.Radius = 15.0f;

	// �R���C�_�[�̐ݒ�
	collision_.Center = position_;
	float angle = randomRange(0.f, 6.28f);
	velocity_ = Vector3(sinf(angle), 2.f, cosf(angle));
	position_.y += collision_.Radius;

	type_ = (int)randomRange(1.f, SKILL_TYPE_NUM);
	owner_ = nullptr;
	fallFlag_ = true;
}

/**
* �X�V
*/
void Item::Update()
{
	// �ړ��ʂ����W�ɓK�p����
	useGravity();
	position_ += velocity_;
	velocity_.x *= 0.98f;
	velocity_.z *= 0.98f;
	rotation_.y += 3.f * 3.14f / 180;

	if (owner_)
	{
		Destroy(this);
		owner_->setSkill(type_);
	}
	
	getMeshManager()->Entry(6, CreateWorldMatrix());

	collision_.Center = position_;
}

/**
* �`��
*/
void Item::Draw()
{

}

/**
* �R���W�������̎擾
*/
Collision* Item::getCollision()
{
	return &collision_;
}
