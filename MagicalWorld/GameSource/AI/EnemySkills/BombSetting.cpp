/**
* File: BombSetting.cpp
* �X�L�� BombSetting(��F���e�ݒu)
*
* Data:   2017.1.10
* Author: Koshi ito
*/
#include "BombSetting.h"
#include "..\..\Object\ObjectManager.h"
#include <SimpleMath.h>
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Bomb.h"
#include "..\..\Object\Enemy.h"
#include "..\..\..\Utility\MeshManager.h"
#include "..\..\Collision\Collision2D.h"
#include "..\..\..\RangeMap\RangeMap.h"

using namespace DirectX::SimpleMath;

extern MeshManager* getMeshManager();

const int BombSetting::RANGE = 200;

/**
* �R���X�g���N�^
* @param subject -> ����ΏۃI�u�W�F�N�g�̃|�C���^
*/
BombSetting::BombSetting(Enemy* subject)
{
	// �ΏۃI�u�W�F�N�g��ݒ�
	subject_ = subject;
	castTime_ = 180;
}

/**
* �f�X�g���N�^
*/
BombSetting::~BombSetting()
{

}

/**
* ������
*/
void BombSetting::Entry()
{
	// �^�[�Q�b�g�ʒu�̎��ӂ̍��W�������_���Ɍ��߂�
	TARGET* target = *(subject_->getTargetInfo().begin());
	for each(TARGET* obj in subject_->getTargetInfo())
	{
		if (target->hate < obj->hate)
			target = obj;
	}
	for (int i = 0; i < 3; i++)
	{
		float x = rand() % RANGE - RANGE / 2.f;
		float z = rand() % RANGE - RANGE / 2.f;
		pos[i] = target->target->Position() + Vector3(x, 10, z);
	}
}

/**
* ���s
* @return true  --- ���s��
* @return false --- ���s�I��
*/
bool BombSetting::Execute()
{
	// �͈͕\����o�^����
	for (int i = 0; i < 3; i++)
	{
		Circle circle;
		circle._position = pos[i];
		circle._radius = 50.f;
		RangeMap::getInstance()->Add(&circle);
	}
	// �L���X�g�^�C�����I������甚�e�𐶐�����
	if (--castTime_ >= 0) return true;
	if (subject_->getTargetInfo().empty()) return false;
	for (int i = 0; i < 3; i++)
	{
		ObjectManager::getInstance()->Add(L"Item", new Bomb(pos[i], 30))->Initialize();
	}
	return false;
}

/**
* �I��
*/
void BombSetting::Exit()
{
}
