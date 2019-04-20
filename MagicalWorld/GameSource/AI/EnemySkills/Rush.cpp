/**
* File: Rush.cpp
* �X�L�� Rush(��F�ːi)
*
* Data:   2017.1.10
* Author: Koshi ito
*/

#include "Rush.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Collision\Collision2D.h"
#include <SimpleMath.h>
#include "..\..\Object\Enemy.h"
#include "..\..\..\Utility\MeshManager.h"
#include "..\..\..\Particle\ParticleManager.h"
#include "..\..\..\Utility\TaskManager.h"
#include "..\..\Effect\Gether.h"
#include "..\..\..\RangeMap\RangeMap.h"

using namespace DirectX::SimpleMath;

extern MeshManager* getMeshManager();
extern ParticleManager* getParticleManager();
extern TaskManager* getTaskManager();
extern float randomRange(float min, float max);

#define LENGTH (200)

/**
* �R���X�g���N�^
* @param subject -> ����ΏۃI�u�W�F�N�g�̃|�C���^
*/
Rush::Rush(Enemy* subject)
{
	// �ΏۃI�u�W�F�N�g��ݒ�
	subject_ = subject;
	castTime_ = 120;
}

/**
* �f�X�g���N�^
*/
Rush::~Rush()
{

}

/**
* ������
*/
void Rush::Entry()
{
}

/**
* ���s
* @return true  --- ���s��
* @return false --- ���s�I��
*/
bool Rush::Execute()
{
	// �Ώۂ����Ȃ���ΏI��
	if (subject_->getTargetInfo().empty()) return false;
	castTime_--;
	if (castTime_ > 60)
	{
		Square square;
		square._position = subject_->Position() + subject_->MoveForward(LENGTH / 2.f);
		square._angle = subject_->Rotation().y;
		square._x = subject_->getCollision()->getMaxRange() * 4;
		square._z = LENGTH;
		RangeMap::getInstance()->Add(&square);

		for (int i = 0; i < 20; i++)
			getTaskManager()->Add<Gether>()->Initialize(getParticleManager(), subject_->Position(),
				subject_->getCollision()->getMaxRange() * subject_->getCollision()->getMaxRange(),
				1.f, 0.f, 0.f, 1.f);
		return true;
	}
	else if(castTime_ < 40)
	{
		subject_->setVelocityX(subject_->MoveForward(LENGTH).x / 40);
		subject_->setVelocityZ(subject_->MoveForward(LENGTH).z / 40);

		Circle range;	// ���m�͈�
		range._position = subject_->Position();
		range._radius = subject_->getCollision()->getMaxRange();
		for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Player"))
		{
			Circle tmp;
			tmp._position = object->Position();
			tmp._radius = object->getCollision()->getMaxRange();
			if (Circle2Circle(tmp, range))
			{
				object->ReceiveDamage(subject_, subject_->getAttack());
			}
		}
	}

	if (castTime_ > 0) return true;
	return false;
}

/**
* �I��
*/
void Rush::Exit()
{
}
