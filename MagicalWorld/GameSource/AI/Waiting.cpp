/**
* File: Waiting.cpp
* �s����ԁF�U���Ώۂ�T���ē������
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#include "Waiting.h"
#include <SimpleMath.h>
#include "Follow.h"

using namespace std;
using namespace DirectX::SimpleMath;
/**
* �R���X�g���N�^
* @param subject -> ���̃N���X�̍s�������s���������I�u�W�F�N�g(Enemy*)
*/
Waiting::Waiting(Enemy* subject)
:subject_(subject)
{
	count_ = 0;
	rotation_ = 0.f;
}
/**
* �f�X�g���N�^
*/
Waiting::~Waiting()
{

}
/**
* ����������
* @param ����
* @return(void)
*/
void Waiting::Entry()
{
	count_ = 0;
	rotation_ = 0.f;
}
/**
* �G�Ӄ��X�g��ʂ̃I�u�W�F�N�g��Ǐ]����
* @param ����
* @return(void)
*/
void Waiting::Execute()
{
	count_++;
	if (count_ >= 60)
	{
		count_ = 0;
		rotation_ = (rand() % 180 - 180) * 1.f;
		rotation_ = rotation_ * 3.14f / 180;
	}
	Vector3 rot = subject_->Rotation();
	rot.y = shortAngle(rot.y, rotation_);
	subject_->Rotation(rot);
	subject_->setVelocity(subject_->MoveForward(1.f));
	for each (TARGET* target in subject_->getTargetInfo())
	{
		if (target->hate != 0)
		{
			subject_->ChangeState(new Follow(subject_));
			return;
		}
	}
}
/**
* �I������
* @param ����
* @return(void)
*/
void Waiting::Exit()
{

}
