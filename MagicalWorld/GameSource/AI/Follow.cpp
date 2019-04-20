/**
* File: Follow.cpp
* �s����ԁF�Ώۂ�ǐՂ���
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#include "Follow.h"
#include <SimpleMath.h>
#include "Waiting.h"
#include "UseSkill.h"

using namespace DirectX::SimpleMath;
/**
* �R���X�g���N�^
* @param subject -> ���̃N���X�̍s�������s���������I�u�W�F�N�g(Enemy*)
*/
Follow::Follow(Enemy* subject)
:subject_(subject)
{

}

/**
* �f�X�g���N�^
*/
Follow::~Follow()
{

}

/**
* ����������
* @param ����
* @return(void)
*/
void Follow::Entry()
{

}

/**
* �G�Ӄ��X�g��ʂ̃I�u�W�F�N�g��Ǐ]����
* @param ����
* @return(void)
*/
void Follow::Execute()
{
	// �ł��G�����Ă���I�u�W�F�N�g�̍��W���擾
	Vector3 tophatePosition = subject_->Position();
	int topHate = 0;
	for each (TARGET* target in subject_->getTargetInfo())
	{
		if (topHate < target->hate)
		{
			topHate = target->hate;
			tophatePosition = target->target->Position();
		}
	}
	// �_���G��������Ȃ������ꍇ
	if (topHate == 0)
	{
		subject_->ChangeState(new Waiting(subject_));
		return;
	}
	// ���W����p�x���Z�o
	Vector3 length = tophatePosition - subject_->Position();
	float angle = atan2(length.x, length.z);
	// �p�x��K�p
	Vector3 rot = subject_->Rotation();
	rot.y = angle;
	subject_->Rotation(rot);
	
	if (length.Length() >= subject_->getCollision()->getMaxRange() * 2.0f)
	{
		subject_->setVelocityX(subject_->MoveForward(1.0f).x);
		subject_->setVelocityZ(subject_->MoveForward(1.0f).z);
	}


	if (subject_->getSkillList().empty()) return;
	// �X�L���f�B���C���O�Ȃ�X�L�����g�p����
	if (subject_->getSkillDelay() == 0)
	{
		subject_->ChangeState(new UseSkill(subject_));
	}
}

/**
* �I������
* @param ����
* @return(void)
*/
void Follow::Exit()
{

}
