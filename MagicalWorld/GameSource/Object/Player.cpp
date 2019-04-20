/**
* File: Player.cpp
* プレイヤー制御
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
* グローバル関数
*/
extern MeshManager* getMeshManager();
extern TaskManager* getTaskManager();
extern ParticleManager* getParticleManager();

/**
* 名前空間
*/
using namespace std;
using namespace DirectX::SimpleMath;

#define TURN_SPEED (0.5f)
/**
* コンストラクタ
* @param position -> 座標
*/
Player::Player(Vector3 position)
{
	canvas_ = make_unique<Canvas>();
	skillController_ = make_unique<SkillController>(this);
	collision_ = make_unique<Sphere>();
	position_ = position;
}

/**
* デストラクタ
*/
Player::~Player()
{

}

/**
* 初期化
*/
void Player::Initialize()
{
	// ステータスの読み込み
	LoadStatus();
	hp_ = maxHp_;

	CameraManager::getInstance()->setMode(new TrackingMode(this));
	canvas_->Initialize(this, skillController_.get());
}

/**
* 更新
*/
void Player::Update()
{
	// UI更新
	canvas_->Update();
	// 復活処理(Respawn)
	if (hp_ <= 0)
	{
		return;
	}

	// 毎フレーム初期化する
	velocity_.x = 0.0f;
	velocity_.z = 0.0f;

	if (!skillController_->Run())
	{
		// 移動処理
		Move();
	}
	// 重力を適用する
	useGravity();
	// 移動量更新
	position_ += velocity_;
	// コライダーの更新
	collision_->Center = position_;
	// モデル描画の登録
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
* 描画
*/
void Player::Draw()
{
	// UI描画
	canvas_->Render();
}

/**
* コリジョン情報の取得
*/
Collision* Player::getCollision()
{
	return collision_.get();
}

/**
* 移動関係の処理を纏めている
*/
void Player::Move()
{
	// 正面の方向を算出
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
	// 押されている場合移動処理を行う
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