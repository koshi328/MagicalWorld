/**
* File: Clave.cpp
* �X�L�� Clave(��F�S��)
*
* Data:   2017.1.10
* Author: Koshi ito
*/
#include "Cleave.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Collision\Collision2D.h"
#include <SimpleMath.h>
#include "..\..\Object\Enemy.h"
#include "..\..\..\Utility\MeshManager.h"
#include "..\..\..\Particle\ParticleManager.h"
#include "..\..\..\Utility\TaskManager.h"
#include "..\..\Effect\CircleEffect.h"

using namespace DirectX::SimpleMath;
/**
* �O���[�o���֐�
*/
extern MeshManager* getMeshManager();
extern ParticleManager* getParticleManager();
extern TaskManager* getTaskManager();

/**
* �R���X�g���N�^
* @param subject -> ����ΏۃI�u�W�F�N�g�̃|�C���^
*/
Cleave::Cleave(Enemy* subject)
{
	// �ΏۃI�u�W�F�N�g��ݒ�
	subject_ = subject;
	castTime_ = 120;
	delayTime_ = 30;
	angle_ = 0.0f;
}

/**
* �f�X�g���N�^
*/
Cleave::~Cleave()
{

}

/**
* ������
*/
void Cleave::Entry()
{
}

/**
* ���s
* @return true  --- ���s��
* @return false --- ���s�I��
*/
bool Cleave::Execute()
{
	if (--effectDilay_ <= 0)
	{
		effectDilay_ = 60;
		Vector3 p = subject_->Position();
		p.y -= subject_->getCollision()->getMaxRange() / 2;
		getTaskManager()->Add<CircleEffect>()->Initialize(getParticleManager(), p, 50);
	}
	// �X�L�����������̏���
	if (castTime_ <= 40) angle_ += 360 * 3.14f / 180 / 40;
	Matrix Lworld = Matrix::Identity;
	Matrix Rworld = Matrix::Identity;
	Matrix Ltrans = Matrix::CreateTranslation(Vector3(2, 0, 0));
	Matrix Rtrans = Matrix::CreateTranslation(Vector3(-2, 0, 0));
	Matrix rot = Matrix::CreateRotationY(angle_);
	Matrix scale = Matrix::CreateScale(0.4f);
	Lworld = scale * Ltrans * rot * subject_->CreateWorldMatrix();
	Rworld = scale * Rtrans * rot * subject_->CreateWorldMatrix();

	getMeshManager()->Entry(2, Lworld);
	getMeshManager()->Entry(2, Rworld);

	if (--castTime_ >= 0) return true;
	// �X�L������
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
			object->ReceiveDamage(subject_, subject_->getAttack());
		}
	}
	return false;
}

/**
* �I��
*/
void Cleave::Exit()
{
}
