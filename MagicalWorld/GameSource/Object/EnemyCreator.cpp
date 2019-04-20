/**
* File: EnemyCreator.cpp
*
* �G�l�~�[�̐����|�C���g
*
* Date: 2016.12.20
* Author: koshi ito
*/
#include "EnemyCreator.h"
#include "ObjectManager.h"
#include "..\..\Utility\TaskManager.h"
#include "..\..\Utility\TaskManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "..\Effect\Gether.h"

extern TaskManager* getTaskManager();
extern ParticleManager* getParticleManager();

/**
* �R���X�g���N�^
*/
EnemyCreator::EnemyCreator()
{

}
/**
* �f�X�g���N�^
*/
EnemyCreator::~EnemyCreator()
{
	std::vector<Enemy*>::iterator it = prototypeList_.begin();
	for (; it != prototypeList_.end();)
	{
		delete *it;
		it = prototypeList_.erase(it);
	}
}
/**
* ������
*/
void EnemyCreator::Initialize()
{

}
/**
* �X�V
*/
void EnemyCreator::Update()
{
	for (int i = 0; i < 20; i++)
		getTaskManager()->Add<Gether>()->Initialize(getParticleManager(), position_, 20, 1, 1, 1, 1);
	if (prototypeList_.size() == 0) return;
	if (nowCreateNum_ >= maxCreateNum_) return;
	if (--time_ > 0) return;
	// �o���p�x�̏�����
	time_ = frequency_;
	// ����
	int id = rand() % prototypeList_.size();
	Enemy* enemy = new Enemy(prototypeList_[id]);
	ObjectManager::getInstance()->Add(L"Enemy", enemy);
	enemy->setCreator(this);
	enemy->Initialize();
	++nowCreateNum_;
}
/**
* �`��
*/
void EnemyCreator::Draw()
{

}
/**
* �ő吔�̐ݒ�
* @param num --- �ő吶����
*/
void EnemyCreator::setMaxCreateNum(int num)
{
	maxCreateNum_ = num;
}
/**
* �����p�x�̐ݒ�
* @param num --- �����p�x
*/
void EnemyCreator::setFrequency(int num)
{
	frequency_ = num;
}
/**
* �v���g�^�C�v�̓o�^
* @param enemy --- �����������G�l�~�[�̃|�C���^
*/
void EnemyCreator::setEnemyPrototype(Enemy* enemy)
{
	prototypeList_.push_back(enemy);
}
/**
* �������̃J�E���g
* @param num --- ���Z�����
*/
void EnemyCreator::addCreateNum(int num)
{
	nowCreateNum_ += num;
}
