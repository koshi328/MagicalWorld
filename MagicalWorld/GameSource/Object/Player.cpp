/**
* File: Player.cpp
* �v���C���[����
*
* Data:   2016.10.30
* Author: Koshi ito
*/
#include "Player.h"
#include "ObjectManager.h"
#include "..\..\Utility\MeshManager.h"
#include "..\..\Utility\TaskManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "..\..\Utility\Key.h"
#include "..\Camera\CameraManager.h"
#include "..\Camera\TrackingMode.h"
#include "..\UI\Canvas.h"
#include "..\PlayerSkill\SkillController.h"
#include "..\Collision\Collision.h"
#include "Item.h"
#include "..\Effect\PowerUp.h"
#include <fstream>
#include <sstream>

#include "..\..\Sound\ADX2Le.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"

/**
* �O���[�o���֐�
*/
extern MeshManager* getMeshManager();
extern TaskManager* getTaskManager();
extern ParticleManager* getParticleManager();

/**
* ���O���
*/
using namespace std;
using namespace DirectX::SimpleMath;

#define TURN_SPEED (0.5f)
/**
* �R���X�g���N�^
* @param position -> ���W
*/
Player::Player(Vector3 position)
{
	canvas_ = make_unique<Canvas>();
	skillController_ = make_unique<SkillController>(this);
	collision_ = make_unique<Sphere>();
	position_ = position;
}

/**
* �f�X�g���N�^
*/
Player::~Player()
{

}

/**
* ������
*/
void Player::Initialize()
{
	// �X�e�[�^�X�̓ǂݍ���
	LoadStatus();
	hp_ = maxHp_;

	CameraManager::getInstance()->setMode(new TrackingMode(this));
	canvas_->Initialize(this, skillController_.get());
}

/**
* �X�V
*/
void Player::Update()
{
	// UI�X�V
	canvas_->Update();
	// ��������(Respawn)
	if (hp_ <= 0)
	{
		return;
	}

	// ���t���[������������
	velocity_.x = 0.0f;
	velocity_.z = 0.0f;

	if (!skillController_->Run())
	{
		// �ړ�����
		Move();
	}
	// �d�͂�K�p����
	useGravity();
	// �ړ��ʍX�V
	position_ += velocity_;
	// �R���C�_�[�̍X�V
	collision_->Center = position_;
	// ���f���`��̓o�^
	if(unrivaledTime_ % 2 == 0)
		getMeshManager()->Entry(0, CreateWorldMatrix());
	SearchItem();
	if (!damageFlag_)
		unrivaledTime_ = 60;
	else
	{
		unrivaledTime_--;
		if (unrivaledTime_ <= 0)
			damageFlag_ = false;
	}

}

/**
* �`��
*/
void Player::Draw()
{
	// UI�`��
	canvas_->Render();
}

/**
* �R���W�������̎擾
*/
Collision* Player::getCollision()
{
	return collision_.get();
}

/**
* �ړ��֌W�̏�����Z�߂Ă���
*/
void Player::Move()
{
	// ���ʂ̕������Z�o
	Vector3 dir = position_ - CameraManager::getInstance()->getCamera()->GetEyepos();
	float angle = atan2(dir.x, dir.z);
	bool check = false;
	if (Key::getInstance()->Press('W'))
	{
		check = true;
		rotation_.y += shortAngle(rotation_.y, angle) * TURN_SPEED;
		if (Key::getInstance()->Press('A'))
			rotation_.y += shortAngle(rotation_.y, angle + (45 * 3.14f / 180)) * TURN_SPEED;
		else if (Key::getInstance()->Press('D'))
			rotation_.y += shortAngle(rotation_.y, angle + (-45 * 3.14f / 180)) * TURN_SPEED;
	}
	else if (Key::getInstance()->Press('S'))
	{
		check = true;
		rotation_.y += shortAngle(rotation_.y, angle + (180 * 3.14f / 180)) * TURN_SPEED;
		if (Key::getInstance()->Press('A'))
			rotation_.y += shortAngle(rotation_.y, angle + (135 * 3.14f / 180)) * TURN_SPEED;
		else if (Key::getInstance()->Press('D'))
			rotation_.y += shortAngle(rotation_.y, angle + (-135 * 3.14f / 180)) * TURN_SPEED;
	}
	else if (Key::getInstance()->Press('A'))
	{
		check = true;
		rotation_.y += shortAngle(rotation_.y, angle + (90 * 3.14f / 180)) * TURN_SPEED;
	}
	else if (Key::getInstance()->Press('D'))
	{
		check = true;
		rotation_.y += shortAngle(rotation_.y, angle + (-90 * 3.14f / 180)) * TURN_SPEED;
	}
	// ������Ă���ꍇ�ړ��������s��
	if (check)
	{
		Vector3 tmpVel = MoveForward(2.0f);
		velocity_.x = tmpVel.x;
		velocity_.z = tmpVel.z;
		rotation_.x = 30 * 3.14f / 180.f;
	}
	else
	{
		rotation_.x = 0;
	}
}

bool Player::LoadStatus()
{
	std::string str;
	std::ifstream ifs("Resources/Data/User/Status.txt");
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
		else if (type == "RANGE")
			collision_->Radius = atoi(data.c_str()) * 1.f;
	}
	ifs.close();
	return true;
}

void Player::Respawn()
{
	LoadStatus();
	hp_ = maxHp_;
}

void Player::SearchItem()
{
	for each(Item* obj in ObjectManager::getInstance()->getActorList(L"Item"))
	{
		if (collision_->Check(obj->getCollision()))
		{
			if (skillController_->getNowSkillNum() > 9) return;
			obj->setOwner(this);
		}
	}
}

void Player::setSkill(int type)
{
	int num = skillController_->getNowSkillNum();
	if (num > 9) return;
	skillController_->setSkill(num, type);
	skillController_->setNowSkillNum(num + 1);
	getTaskManager()->Add<PowerUp>()->Initialize(getParticleManager(), position_, collision_->getMaxRange() * 2);
	ADX2Le::Play(CRI_CUESHEET_0_SE_DASH);
}