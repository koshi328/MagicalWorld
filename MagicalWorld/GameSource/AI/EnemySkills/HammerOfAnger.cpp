/**
* File: HammerOfAnger.cpp
* �X�L�� HammerOfAnger(��F�{��̓S��)
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#include "HammerOfAnger.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Collision\Collision2D.h"
#include "..\..\Camera\CameraManager.h"
#include "..\..\Object\Enemy.h"
#include "..\..\..\Utility\MeshManager.h"
#include "..\..\..\Particle\ParticleManager.h"
#include "..\..\..\Utility\TaskManager.h"
#include "..\..\Effect\Explosion.h"
#include "..\..\..\RangeMap\RangeMap.h"
#include "..\..\..\Sound\ADX2Le.h"
#include "..\..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"
using namespace DirectX::SimpleMath;

/**
* �O���[�o���֐�
*/
extern MeshManager* getMeshManager();
extern ParticleManager* getParticleManager();
extern TaskManager* getTaskManager();

const float HammerOfAnger::RANGE = 50.f;

/**
* �R���X�g���N�^
* @param subject -> ����ΏۃI�u�W�F�N�g�̃|�C���^
*/
HammerOfAnger::HammerOfAnger(Enemy* subject)
{
	// �ΏۃI�u�W�F�N�g��ݒ�
	subject_ = subject;
	castTime_ = 120;
	delayTime_ = 120;
}

/**
* �f�X�g���N�^
*/
HammerOfAnger::~HammerOfAnger()
{

}

/**
* ������
*/
void HammerOfAnger::Entry()
{
}

/**
* ���s
* @return true  --- ���s��
* @return false --- ���s�I��
*/
bool HammerOfAnger::Execute()
{
	float motion = castTime_ - 20.f;
	// �X�L�����������̏���
	Matrix world;
	Matrix transChild = Matrix::CreateTranslation(Vector3(0, 0, subject_->getCollision()->getMaxRange() * 2));
	Matrix scale = Matrix::CreateScale(1.0f);
	Matrix rot = Matrix::Identity;
	if(motion >= 0)
		rot = Matrix::CreateRotationX(-90 * 3.14f / 180 + (3.14f / motion));
	if (motion == 0)
	{
		CameraManager::getInstance()->ShakingMode(60, 3);
	}

	world = scale * transChild * rot * subject_->CreateWorldMatrix();

	getMeshManager()->Entry(4, world);
	// �͈͂̐���
	Square range;
	range._angle = subject_->Rotation().y;
	range._x = RANGE;
	range._z = RANGE;
	range._position = subject_->Position() + subject_->MoveForward(RANGE / 2);
	RangeMap::getInstance()->Add(&range);

	// �X�L������
	if (--castTime_ >= 0) return true;
	// �ڐG����
	for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Player"))
	{
		Circle tmp;
		tmp._position = object->Position();
		tmp._radius = object->getCollision()->getMaxRange();
		if (Circle2Square(tmp, range))
		{
			// �_���[�W�𑗐M
			object->ReceiveDamage(subject_, subject_->getAttack());
			getTaskManager()->Add<Explosion>()->Initialize(getParticleManager(), object->Position(), 30);
		}
	}
	return false;
}

/**
* �I��
*/
void HammerOfAnger::Exit()
{
}
