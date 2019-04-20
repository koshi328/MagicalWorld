/**
* File: Enemy.cpp
*
* �G�l�~�[�̋��ʃA���S���Y��
*
* Date: 2016.12.20
* Author: koshi ito
*/

#include "Enemy.h"
#include "..\AI\Waiting.h"
#include "..\Object\ObjectManager.h"
#include "..\Collision\Collision2D.h"
#include "..\Collision\Collision.h"
#include "..\..\Utility\MeshManager.h"
#include "..\..\GameBase\Graphics.h"
#include "EnemyCreator.h"
#include "..\Camera\CameraManager.h"
#include "..\UI\GUI.h"
#include "..\Effect\Afterimage.h"
#include "..\..\Utility\TaskManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "Item.h"
#include "Stage.h"

#include <fstream>
#include <sstream>

/**
* ���O���
*/
using namespace std;
using namespace DirectX::SimpleMath;

/**
* �N���[�o���֐�
*/
extern MeshManager* getMeshManager();
extern TaskManager* getTaskManager();
extern ParticleManager* getParticleManager();
extern float randomRange(float min, float max);

/**
* �R���X�g���N�^
*/
Enemy::Enemy(Enemy* copy)
{
	state_ = nullptr;
	skillID_ = 0;
	skillDelay_ = 0;
	seachRange_ = copy->seachRange_;
	position_ = copy->position_;
	modelID_ = copy->modelID_;

	maxHp_ = copy->maxHp_;
	attack_ = copy->attack_;
	defense_ = copy->defense_;
	skillNumberList_ = copy->skillNumberList_;
	point_ = copy->point_;
}

/**
* �f�t�H���g�R���X�g���N�^
*/
Enemy::Enemy()
{
	state_ = nullptr;
	skillID_ = 0;
	skillDelay_ = 0;
	seachRange_ = 0.0f;
	modelID_ = 0;
	maxHp_ = 0;
	attack_ = 0;
	defense_ = 0;
	point_ = 0;
	position_ = Vector3(0, 0, 0);
}

/**
* �f�X�g���N�^
*/
Enemy::~Enemy()
{
	for (std::list<TARGET*>::iterator it = targetList_.begin(); 
	it != targetList_.end();)
	{
		delete *it;
		it = targetList_.erase(it);
	}

	if (myCreator_)
		myCreator_->addCreateNum(-1);
}
/**
* ����������
*/
void Enemy::Initialize()
{
	collision_.Radius = 10.0f;
	ChangeState(new Waiting(this));

	hp_ = maxHp_;

	if (myCreator_)
		position_ = myCreator_->Position();
}

/**
* �X�V����
*/
void Enemy::Update()
{
	if (hp_ > 0)
	{
		// ���t���[������������
		velocity_.x = 0.0f;
		velocity_.z = 0.0f;

		if (skillDelay_ != 0) --skillDelay_;
		// �����̎��͂ɂ���U���Ώۂ�T��
		AmbientSearch();

		// �K�p����Ă���s���̎��s
		if (state_)
		{
			state_->Execute();
		}
		// �ړ��ʂ����W�ɓK�p����
		useGravity();
		position_ += velocity_;

		// �Փ˔���̍��W�X�V
		collision_.Center = position_;
	}
	else
	{
		float x = randomRange(-10.f, 10.f);
		float y = randomRange(-10.f, 10.f);
		float z = randomRange(-10.f, 10.f);
		getTaskManager()->Add<Afterimage>()->Initialize(getParticleManager(), position_ + Vector3(x, y, z), 5.f);
		scale_ *= 0.98f;
		if (scale_.x <= 0.05f)
		{
			StageManager::getInstance()->addKillEnemyNum(1);
			if(rand() % 3 == 0)
				ObjectManager::getInstance()->Add(L"Item", new Item(position_, 1200))->Initialize();
			Destroy(this);
		}
	}

	if (!damageFlag_)
		unrivaledTime_ = 20;
	else
	{
		unrivaledTime_--;
		if (unrivaledTime_ <= 0)
			damageFlag_ = false;
	}
	if(unrivaledTime_ % 2 == 0)
	// ���f���̕`��o�^
	getMeshManager()->Entry(modelID_, CreateWorldMatrix());

	// �G�l�~�[���m�Ŕ���������
	if (hp_ > 0)
		EnemysRebound();
}

/**
* �`�揈��
*/
void Enemy::Draw()
{

	// ���ݒu�v���O����
	float w = 1280 / 2;
	float h = 720 / 2;
	Matrix viewPort = {
		w,  0, 0, 0,
		0, -h, 0, 0,
		0,  0, 1, 0,
		w,  h, 0, 1
	};
	Vector3 tmp = position_;
	tmp.y += collision_.getMaxRange() * 2;
	tmp = Vector3::Transform(tmp, CameraManager::getInstance()->getCamera()->GetViewmat());
	tmp = Vector3::Transform(tmp, CameraManager::getInstance()->getCamera()->GetProjmat());
	tmp /= tmp.z;
	tmp = Vector3::Transform(tmp, viewPort);

	float gaugeWidth = (float)hp_ / (float)maxHp_ * 128.f;
	GUI::Sprite flame(L"Resources/Texture/whiteRect.png", tmp.x - 64.f, tmp.y - 16, 128.f, 16);
	GUI::Sprite gauge(L"Resources/Texture/whiteRect.png", tmp.x - 64.f, tmp.y - 16, gaugeWidth, 16);

	flame.Draw(Color(0.5f, 0.5f, 0.5f, 0.5f));
	gauge.Draw(Color(0.6f, 0, 0, 0.5f));
}

/**
* �I�u�W�F�N�g���������Ă���R���C�_�[���擾
* @param ����
* @return(Collision*) �I�u�W�F�N�g����������R���C�_�[
*/
Collision* Enemy::getCollision()
{
	return &collision_;
}

/**
* ���s����s���̕ύX
* @param state -> AIState�̔h���N���X���w��
* @return(void)
*/
void Enemy::ChangeState(AIState* state)
{
	if (state_)
	{
		state_->Exit();
		delete state_;
	}
	state_ = state;
	state_->Entry();
}

/**
* ���̃I�u�W�F�N�g�̎��ӂɂ���U���Ώۂ�T��
* @param ����
* @return(void)
*/
void Enemy::AmbientSearch()
{
	seachRange_ = 400.0f;
	Circle detection;	// ���m�͈͗p
	detection._position = position_;
	detection._radius = seachRange_;
	for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Player"))
	{
		// ���ɓo�^����Ă��Ȃ������ׂ�
		bool exist = false;
		for each (TARGET* list in targetList_)
		{
			if (list->target == object)
			{
				exist = true;
				break;
			}
		}
		if (exist) continue;
		// �o�^����Ă��Ȃ������ꍇ�V�K�o�^
		Circle targetRange;
		targetRange._position = object->Position();
		targetRange._radius = object->getCollision()->getMaxRange();
		if (Circle2Circle(detection, targetRange))
		{
			TARGET* target = new TARGET;
			target->target = object;
			target->hate = 1;
			targetList_.push_back(target);
		}
	}

	// �ΏۊO�ɂȂ����I�u�W�F�N�g�����X�g����O������
	for (list<TARGET*>::iterator it = targetList_.begin(); it != targetList_.end();)
	{
		// �f���[�g���b�Z�[�W�𑗐M���Ă���I�u�W�F�N�g�̓��X�g����O��
		if ((*it)->target->DestroyMessage())
		{
			delete *it;
			it = targetList_.erase(it);
			continue;
		}
		// ��苗�����ꂽ�I�u�W�F�N�g�̓��X�g����O��
		Vector3 range = position_ - (*it)->target->Position();
		float distance = range.Length();
		if (distance >= seachRange_)
		{
			delete *it;
			it = targetList_.erase(it);
			continue;
		}
		it++;
	}
}

/**
* �p�����[�^���X�N���v�g����擾
* @param filename --- �X�e�[�^�X�t�@�C���̃A�h���X
*/
bool Enemy::LoadStatus(const wchar_t* filename)
{
	std::string str;
	std::ifstream ifs(filename);
	if (ifs.fail()) return false;
	while (getline(ifs, str))
	{
		std::string type;
		std::string data;
		std::istringstream stream(str);
		getline(stream, type, ',');
		getline(stream, data, ',');
		if (type == "HP")
			maxHp_ = atoi(data.c_str());
		else if (type == "ATTACK")
			attack_ = atoi(data.c_str());
		else if (type == "DEFENSE")
			defense_ = atoi(data.c_str());
		else if (type == "MODEL")
			modelID_ = atoi(data.c_str());
		else if (type == "SKILL")
			skillNumberList_.push_back(atoi(data.c_str()));
		else if (type == "RANGE")
			collision_.Radius = atoi(data.c_str()) * 1.f;
		else if (type == "POINT")
			point_ = atoi(data.c_str());
	}
	ifs.close();
	return true;
}

/**
* ���g�̏�������EnemyCreator��ݒ肷��
* @param creator --- EnemyCreator�̃|�C���^
*/
void Enemy::setCreator(EnemyCreator* creator)
{
	myCreator_ = creator;
}

/**
* �G�l�~�[���m�����������ďd�Ȃ�Ȃ��悤�ɂ���
*/
void Enemy::EnemysRebound()
{
	for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Enemy"))
	{
		if (object == this) continue;
		Sphere sphere;
		sphere.Center = object->Position();
		sphere.Radius = object->getCollision()->getMaxRange();
		Vector3 inter;
		if (CheckSphere2Sphere(collision_, sphere, &inter))
		{
			Vector3 move = position_ - inter;
			position_ += move * 0.1f;
			move = object->Position() - inter;
			object->Position(object->Position() + move * 0.1f);
		}
	}
}