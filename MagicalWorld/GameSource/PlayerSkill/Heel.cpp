/**
* File: heel.cpp
*
* �Ώۂ��񕜂�����X�L��
*
* Date: 2016.12.13
* Author: koshi ito
*/
#include "Heel.h"
#include "..\Object\Actor.h"
#include <SimpleMath.h>
#include "..\..\Particle\ParticleManager.h"
#include "..\..\Utility\TaskManager.h"
#include "..\Effect\HeelEffect.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"
#include "..\..\Sound\ADX2Le.h"
/**
* ���O���
*/
using namespace DirectX::SimpleMath;
/**
* �O���[�o���֐�
*/
extern ParticleManager* getParticleManager();
extern TaskManager* getTaskManager();
/**
* �R���X�g���N�^
* @param subject -- �Ώ�Actor�̃|�C���^
*/
Heel::Heel(Actor* subject)
{
	subject_ = subject;
	recastTime_ = 120;
}
/**
* �f�X�g���N�^
*/
Heel::~Heel()
{

}
/**
* �o�^
*/
void Heel::Entry()
{

}

/**
* ���s
* @return(bool) ���s����true,�I������false��Ԃ�
*/
bool Heel::Execute()
{
	subject_->setHP(subject_->getHP() + subject_->getMaxHP() / 10);
	if (subject_->getMaxHP() < subject_->getHP()) subject_->setHP(subject_->getMaxHP());

	getTaskManager()->Add<HeelEffect>()->Initialize(getParticleManager(), subject_->Position());
	ADX2Le::Play(CRI_CUESHEET_0_SE_HEEL);
	recastTime_ = 120;
	return false;
}
/**
* �I��
*/
void Heel::Exit()
{

}