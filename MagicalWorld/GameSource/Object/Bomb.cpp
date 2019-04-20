/**
* File: Bomb.cpp
*
* ���e�N���X
*
* Date: 2016.12.25
* Author: koshi ito
*/
#include "Bomb.h"
#include "..\..\Utility\MeshManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "..\..\Utility\TaskManager.h"
#include "..\Effect\Explosion.h"
#include "..\Effect\CircleEffect.h"
#include "..\Camera\CameraManager.h"
#include "..\Object\ObjectManager.h"
#include "..\Collision\Collision2D.h"
#include "..\..\Sound\ADX2Le.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"

using namespace DirectX::SimpleMath;

extern MeshManager* getMeshManager();
extern ParticleManager* getParticleManager();
extern TaskManager* getTaskManager();

/**
* �R���X�g���N�^
* @param position -> ���W
*/
Bomb::Bomb(Vector3 position, int lifeTime)
{
	position_ = position;
	lifeTime_ = lifeTime;
	effectDilay_ = 60;
}

/**
* �f�X�g���N�^
*/
Bomb::~Bomb()
{

}

/**
* ������
*/
void Bomb::Initialize()
{
	// �X�P�[���̐ݒ�
	collision_.Radius = 6.0f;
	attack_ = 200;

	hp_ = maxHp_;
	// �R���C�_�[�̐ݒ�
	collision_.Center = position_;
}

/**
* �X�V
*/
void Bomb::Update()
{
	// �ړ��ʂ����W�ɓK�p����
	useGravity();
	position_ += velocity_;
	
	getMeshManager()->Entry(8, CreateWorldMatrix());
	Vector3 pos = position_;
	pos.y -= getCollision()->getMaxRange();
	Matrix world = Matrix::Identity;
	Matrix trans = Matrix::CreateTranslation(pos);
	Matrix scale = Matrix::CreateScale(50.0f);
	world = scale * trans;
	getMeshManager()->Entry(5, scale);

	if (--lifeTime_ <= 0)
	{
		Destroy(this);
		getTaskManager()->Add<Explosion>()->Initialize(getParticleManager(), position_, 100);
		CameraManager::getInstance()->ShakingMode(20, 3);
		ADX2Le::Play(CRI_CUESHEET_0_SE_EXPLOSION);

		Circle range;	// ���m�͈͗p
		range._position = position_;
		range._radius = 50.0f;
		for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Player"))
		{
			Circle tmp;
			tmp._position = object->Position();
			tmp._radius = object->getCollision()->getMaxRange();
			if (Circle2Circle(tmp, range))
			{
				object->ReceiveDamage(this, attack_);
			}
		}
	}

	collision_.Center = position_;
}

/**
* �`��
*/
void Bomb::Draw()
{

}

/**
* �R���W�������̎擾
*/
Collision* Bomb::getCollision()
{
	return &collision_;
}
