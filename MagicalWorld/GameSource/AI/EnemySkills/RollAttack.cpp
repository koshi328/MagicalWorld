/**
* File: RollAttack.cpp
* �X�L�� RollAttack(��F��]�U��)
*
* Data:   2017.1.10
* Author: Koshi ito
*/

#include "RollAttack.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Collision\Collision2D.h"
#include "..\..\Object\Enemy.h"
#include "..\..\..\Utility\MeshManager.h"
#include "..\..\..\Particle\ParticleManager.h"
#include "..\..\..\Utility\TaskManager.h"
#include "..\..\Effect\CircleEffect.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

extern MeshManager* getMeshManager();
extern ParticleManager* getParticleManager();
extern TaskManager* getTaskManager();

#define CAST_TIME (120)

/**
* �R���X�g���N�^
* @param subject -> ����ΏۃI�u�W�F�N�g�̃|�C���^
*/
RollAttack::RollAttack(Enemy* subject)
{
	// �ΏۃI�u�W�F�N�g��ݒ�
	subject_ = subject;
	castTime_ = CAST_TIME;
	saveAngle_ = subject->Rotation().y;
}

/**
* �f�X�g���N�^
*/
RollAttack::~RollAttack()
{

}

/**
* ������
*/
void RollAttack::Entry()
{
}

/**
* ���s
* @return true  --- ���s��
* @return false --- ���s�I��
*/
bool RollAttack::Execute()
{
	castTime_--;
	if (castTime_ > 30) return true;
	float angle = 360.f * 3.14f / 180.f / (CAST_TIME - 90);
	Vector3 rot = subject_->Rotation();
	angle = angle * ((CAST_TIME - 90) - castTime_) + saveAngle_;
	rot.y = angle;
	subject_->Rotation(rot);
	if (castTime_ > 0) return true;
	Circle range;	// ���m�͈͗p
	range._position = subject_->Position();
	range._radius = 50.0f;
	for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Player"))
	{
		Circle tmp;
		tmp._position = object->Position();
		tmp._radius = object->getCollision()->getMaxRange();
		if (Circle2Circle(tmp, range))
		{
			object->ReceiveDamage(subject_, subject_->getAttack() / 10);
		}
	}
	return false;
}

/**
* �I��
*/
void RollAttack::Exit()
{
}
