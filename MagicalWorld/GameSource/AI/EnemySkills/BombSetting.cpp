/**
* File: BombSetting.cpp
* スキル BombSetting(訳：爆弾設置)
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
* コンストラクタ
* @param subject -> 操作対象オブジェクトのポインタ
*/
BombSetting::BombSetting(Enemy* subject)
{
	// 対象オブジェクトを設定
	subject_ = subject;
	castTime_ = 180;
}

/**
* デストラクタ
*/
BombSetting::~BombSetting()
{

}

/**
* 初期化
*/
void BombSetting::Entry()
{
	// ターゲット位置の周辺の座標をランダムに決める
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
* 実行
* @return true  --- 実行中
* @return false --- 実行終了
*/
bool BombSetting::Execute()
{
	// 範囲表示を登録する
	for (int i = 0; i < 3; i++)
	{
		Circle circle;
		circle._position = pos[i];
		circle._radius = 50.f;
		RangeMap::getInstance()->Add(&circle);
	}
	// キャストタイムが終わったら爆弾を生成する
	if (--castTime_ >= 0) return true;
	if (subject_->getTargetInfo().empty()) return false;
	for (int i = 0; i < 3; i++)
	{
		ObjectManager::getInstance()->Add(L"Item", new Bomb(pos[i], 30))->Initialize();
	}
	return false;
}

/**
* 終了
*/
void BombSetting::Exit()
{
}
