/**
* File: CollisionManager.h
*
* まとめたい衝突判定を管理する
*
* Date: 2016.11.18
* Author: koshi ito
*/
#pragma once

class CollisionManager
{
private:
	static CollisionManager* instance_;	// インスタンス
	// コンストラクタ
	CollisionManager() = default;
public:
	// シングルトン
	static CollisionManager* getInstance()
	{
		if (!instance_)
		{
			instance_ = new CollisionManager;
		}
		return instance_;
	}
	// デストラクタ
	~CollisionManager()
	{
		instance_ = nullptr;
	}

	// 更新
	void Update();
};