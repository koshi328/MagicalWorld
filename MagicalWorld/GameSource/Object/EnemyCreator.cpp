/**
* File: EnemyCreator.cpp
*
* エネミーの生成ポイント
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
* コンストラクタ
*/
EnemyCreator::EnemyCreator()
{

}
/**
* デストラクタ
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
* 初期化
*/
void EnemyCreator::Initialize()
{

}
/**
* 更新
*/
void EnemyCreator::Update()
{
	for (int i = 0; i < 20; i++)
		getTaskManager()->Add<Gether>()->Initialize(getParticleManager(), position_, 20, 1, 1, 1, 1);
	if (prototypeList_.size() == 0) return;
	if (nowCreateNum_ >= maxCreateNum_) return;
	if (--time_ > 0) return;
	// 出現頻度の初期化
	time_ = frequency_;
	// 生成
	int id = rand() % prototypeList_.size();
	Enemy* enemy = new Enemy(prototypeList_[id]);
	ObjectManager::getInstance()->Add(L"Enemy", enemy);
	enemy->setCreator(this);
	enemy->Initialize();
	++nowCreateNum_;
}
/**
* 描画
*/
void EnemyCreator::Draw()
{

}
/**
* 最大数の設定
* @param num --- 最大生成数
*/
void EnemyCreator::setMaxCreateNum(int num)
{
	maxCreateNum_ = num;
}
/**
* 生成頻度の設定
* @param num --- 生成頻度
*/
void EnemyCreator::setFrequency(int num)
{
	frequency_ = num;
}
/**
* プロトタイプの登録
* @param enemy --- 生成したいエネミーのポインタ
*/
void EnemyCreator::setEnemyPrototype(Enemy* enemy)
{
	prototypeList_.push_back(enemy);
}
/**
* 生成数のカウント
* @param num --- 加算する量
*/
void EnemyCreator::addCreateNum(int num)
{
	nowCreateNum_ += num;
}
