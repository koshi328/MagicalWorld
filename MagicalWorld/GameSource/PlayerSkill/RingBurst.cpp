/**
* File: RingBurst.cpp
*
* �X�L���F����
*
* Date: 2017.1.17
* Author: koshi ito
*/

#include "RingBurst.h"
#include "..\Object\Actor.h"
#include "..\Collision\Collision2D.h"
#include "..\Object\ObjectManager.h"
#include "..\..\Utility\TaskManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "..\Effect\Spherical.h"
#include "..\Effect\Radiation.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"
#include "..\..\Sound\ADX2Le.h"
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
* �萔�錾
*/
#define RANGE (150)
/**
* �R���X�g���N�^
* @param subject -- �Ώ�Actor�̃|�C���^
*/
RingBurst::RingBurst(Actor* subject)
{
	subject_ = subject;
	recastTime_ = 30;
	castTime_ = 90;
}
/**
* �f�X�g���N�^
*/
RingBurst::~RingBurst()
{

}
/**
* �o�^
*/
void RingBurst::Entry()
{
	// �G�t�F�N�g�̓o�^
	getTaskManager()->Add<Radiation>()->Initialize(getParticleManager(), subject_->Position(), RANGE);
}

/**
* ���s
* @return(bool) ���s����true,�I������false��Ԃ�
*/
bool RingBurst::Execute()
{
	if (--castTime_ > 0) return true;
	getTaskManager()->Add<Spherical>()->Initialize(getParticleManager(), subject_->Position(), RANGE, 60);
	ADX2Le::Play(CRI_CUESHEET_0_SE_WATER02);
	// �͈͂𐶐�
	Circle range;
	range._position = subject_->Position();
	range._radius = RANGE;
	// �͈͓�����
	for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Enemy"))
	{
		Circle tmp;
		tmp._position = object->Position();
		tmp._radius = object->getCollision()->getMaxRange();
		if (Circle2Circle(tmp, range))
		{
			//�_���[�W���b�Z�[�W�𑗐M
			object->ReceiveDamage(subject_, subject_->getAttack() * 5);
		}
	}

	return false;
}
/**
* �I��
*/
void RingBurst::Exit()
{

}