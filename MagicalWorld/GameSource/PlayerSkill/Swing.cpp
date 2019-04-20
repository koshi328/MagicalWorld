/**
* File: Swing.cpp
*
* �X�L���F�ߐڊi���i3�i�K�U���j
*
* Date: 2016.12.1
* Author: koshi ito
*/
#include "Swing.h"
#include "..\..\Utility\Mouse.h"
#include "..\Object\Actor.h"
#include "..\Collision\Collision2D.h"
#include "..\Object\ObjectManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "..\..\Utility\TaskManager.h"
#include "..\Effect\Afterimage.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"
#include "..\..\Sound\ADX2Le.h"
#include <SimpleMath.h>
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
* �萔�錾
*/
#define CAST_TIME (10)
#define RECAST_TIME (30)
#define ATTACK_RANGE (20.0f)
#define NEXT_ATTACK_ENTRY_TIME (30)
/**
* �R���X�g���N�^
* @param subject -- �Ώ�Actor�̃|�C���^
*/
Swing::Swing(Actor* subject)
{
	subject_ = subject;
	recastTime_ = RECAST_TIME;
	castTime_ = CAST_TIME + NEXT_ATTACK_ENTRY_TIME;
	angle_ = 0.f;
}
/**
* �f�X�g���N�^
*/
Swing::~Swing()
{

}
/**
* �o�^
*/
void Swing::Entry()
{
	// �i�K�ɂ���ď��������@��ς���
	switch (attackNum_)
	{
	case 0:
		FirstInit();
		break;
	case 1:
		SecondInit();
		break;
	case 2:
		ThirdInit();
		break;
	default:
		break;
	}
}

/**
* ���s
* @return(bool) ���s����true,�I������false��Ԃ�
*/
bool Swing::Execute()
{
	// �Z�̎��Ԃ��v��
	castTime_--;
	// �L���X�g�^�C���̊ԃG�t�F�N�g�𐶐�����
	if (castTime_ >= NEXT_ATTACK_ENTRY_TIME)
	{
		// �i�K�ɂ���ĕʂ̍X�V�������s��
		switch (attackNum_)
		{
		case 0:
			FirstUpdate();
			break;
		case 1:
			SecondUpdate();
			break;
		case 2:
			ThirdUpdate();
			break;
		default:
			break;
		}
		Vector3 tmp = subject_->Rotation();
		tmp.x = 30.f * 3.14f / 180;
		subject_->Rotation(tmp);
	}
	// �ǌ����͂��󂯕t���鎞�Ԃ܂ł͂����ŏI��
	if (castTime_ > NEXT_ATTACK_ENTRY_TIME) return true;
	// �ǌ����͂̎�t
	else if (castTime_ < NEXT_ATTACK_ENTRY_TIME)
	{
		if (Mouse::getInstance()->Touch(Mouse::LEFT))
		{		
			attackNum_++;
			Entry();
		}
	}
	// �_���[�W�����t�F�[�Y
	else
	{
		FanShape range;	// ���m�͈�
		range._position = subject_->Position();
		range._radius = ATTACK_RANGE;
		range._forward = subject_->MoveForward(1.0f);
		range._rangeAngle = 180.f;
		for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Enemy"))
		{
			Circle tmp;
			tmp._position = object->Position();
			tmp._radius = object->getCollision()->getMaxRange();
			if (Circle2FanFhape(tmp, range))
			{
				//�_���[�W���b�Z�[�W�𑗐M
				object->ReceiveDamage(subject_, (subject_->getAttack() * (int)(1.f + attackNum_ * 0.5f)));
			}
		}
	}
	// �ǌ����͂��Ȃ��܂܂O�ɂȂ�����I������
	if (castTime_ > 0) return true;
	return false;
}
/**
* �I������
*/
void Swing::Exit()
{
	recastTime_ = RECAST_TIME;
	attackNum_ = 0;
}

/**
* �ŏ��̍U���̏�����
*/
void Swing::FirstInit()
{
	castTime_ = CAST_TIME + NEXT_ATTACK_ENTRY_TIME;
	angle_ = subject_->Rotation().y - (135 * 3.14f / 180.f);
	ADX2Le::Play(CRI_CUESHEET_0_SE_DASH);
}
/**
* 2���ڂ̒ǌ����s���ׂ̏�����
*/
void Swing::SecondInit()
{
	castTime_ = CAST_TIME + NEXT_ATTACK_ENTRY_TIME;
	angle_ = subject_->Rotation().y + (135 * 3.14f / 180.f);
	ADX2Le::Play(CRI_CUESHEET_0_SE_DASH);
}
/**
* 3���ڂ̒ǌ����s���ׂ̏�����
*/
void Swing::ThirdInit()
{
	castTime_ = CAST_TIME + NEXT_ATTACK_ENTRY_TIME;
	angle_ = 0.f;
	ADX2Le::Play(CRI_CUESHEET_0_SE_DASH);
}
/**
* 1���ڂ̍X�V����
*/
void Swing::FirstUpdate()
{
	float oneFlame = 180 * 3.14f / 180.f / CAST_TIME;
	int loop = 10;
	int i;
	for (i = 0; i < loop; i++)
	{
		angle_ += oneFlame / loop;
		Vector3 pos = Vector3(sinf(angle_) * ATTACK_RANGE, 0.f, cosf(angle_) * ATTACK_RANGE);
		getTaskManager()->Add<Afterimage>()->Initialize(getParticleManager(), subject_->Position() + pos, 5.0f);
	}
	subject_->Position(subject_->Position() + subject_->MoveForward(1.f));
}
/**
* 2���ڂ̍X�V����
*/
void Swing::SecondUpdate()
{
	float oneFlame = 180 * 3.14f / 180.f / CAST_TIME;
	int loop = 10;
	int i;
	for (i = 0; i < loop; i++)
	{
		angle_ -= oneFlame / loop;
		Vector3 pos = Vector3(sinf(angle_) * ATTACK_RANGE, 0.f, cosf(angle_) * ATTACK_RANGE);
		getTaskManager()->Add<Afterimage>()->Initialize(getParticleManager(), subject_->Position() + pos, 5.0f);
	}
	subject_->Position(subject_->Position() + subject_->MoveForward(1.f));
}
/**
* 3���ڂ̍X�V����
*/
void Swing::ThirdUpdate()
{
	float oneFlame = 180 * 3.14f / 180.f / CAST_TIME;
	int loop = 10;
	int i;
	for (i = 0; i < loop; i++)
	{
		angle_ += oneFlame / loop;
		Vector3 pos = Vector3(sinf(subject_->Rotation().y + angle_) * ATTACK_RANGE, 0.f, cosf(subject_->Rotation().y + angle_) * ATTACK_RANGE);
		getTaskManager()->Add<Afterimage>()->Initialize(getParticleManager(), subject_->Position() + pos, 5.0f);

		pos = Vector3(sinf(subject_->Rotation().y - angle_) * ATTACK_RANGE, 0.f, cosf(subject_->Rotation().y - angle_) * ATTACK_RANGE);
		getTaskManager()->Add<Afterimage>()->Initialize(getParticleManager(), subject_->Position() + pos, 5.0f);
	}
	subject_->Position(subject_->Position() + subject_->MoveForward(3.0f));
}