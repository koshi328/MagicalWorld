/**
* File: Enemy.cpp
*
* エネミーの共通アルゴリズム
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
* 名前空間
*/
using namespace std;
using namespace DirectX::SimpleMath;

/**
* クローバル関数
*/
extern MeshManager* getMeshManager();
extern TaskManager* getTaskManager();
extern ParticleManager* getParticleManager();
extern float randomRange(float min, float max);

/**
* コンストラクタ
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
* デフォルトコンストラクタ
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
* デストラクタ
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
* 初期化処理
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
* 更新処理
*/
void Enemy::Update()
{
	if (hp_ > 0)
	{
		// 毎フレーム初期化する
		velocity_.x = 0.0f;
		velocity_.z = 0.0f;

		if (skillDelay_ != 0) --skillDelay_;
		// 自分の周囲にいる攻撃対象を探す
		AmbientSearch();

		// 適用されている行動の実行
		if (state_)
		{
			state_->Execute();
		}
		// 移動量を座標に適用する
		useGravity();
		position_ += velocity_;

		// 衝突判定の座標更新
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
	// モデルの描画登録
	getMeshManager()->Entry(modelID_, CreateWorldMatrix());

	// エネミー同士で反発し合う
	if (hp_ > 0)
		EnemysRebound();
}

/**
* 描画処理
*/
void Enemy::Draw()
{

	// 仮設置プログラム
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
* オブジェクトが所持しているコライダーを取得
* @param 無し
* @return(Collision*) オブジェクトが所持するコライダー
*/
Collision* Enemy::getCollision()
{
	return &collision_;
}

/**
* 実行する行動の変更
* @param state -> AIStateの派生クラスを指定
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
* このオブジェクトの周辺にいる攻撃対象を探す
* @param 無し
* @return(void)
*/
void Enemy::AmbientSearch()
{
	seachRange_ = 400.0f;
	Circle detection;	// 検知範囲用
	detection._position = position_;
	detection._radius = seachRange_;
	for each (Actor* object in ObjectManager::getInstance()->getActorList(L"Player"))
	{
		// 既に登録されていないか調べる
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
		// 登録されていなかった場合新規登録
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

	// 対象外になったオブジェクトをリストから外す処理
	for (list<TARGET*>::iterator it = targetList_.begin(); it != targetList_.end();)
	{
		// デリートメッセージを送信しているオブジェクトはリストから外す
		if ((*it)->target->DestroyMessage())
		{
			delete *it;
			it = targetList_.erase(it);
			continue;
		}
		// 一定距離離れたオブジェクトはリストから外す
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
* パラメータをスクリプトから取得
* @param filename --- ステータスファイルのアドレス
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
* 自身の所属するEnemyCreatorを設定する
* @param creator --- EnemyCreatorのポインタ
*/
void Enemy::setCreator(EnemyCreator* creator)
{
	myCreator_ = creator;
}

/**
* エネミー同士を押し合って重ならないようにする
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