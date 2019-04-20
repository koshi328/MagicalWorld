/**
* File: ObjectManager.h
*
* すべてのオブジェクトを管理
*
* Date: 2016.12.10
* Author: koshi ito
*/
#pragma once

#include "Actor.h"
#include "..\Collision\Terrain.h"
#include <memory>
#include <list>

class ObjectManager
{
public:
	// シングルトン
	static ObjectManager* getInstance();
	// デストラクタ
	~ObjectManager();
	// 追加関数
	Terrain* Add(Terrain* pObject);
	GameObject* Add(GameObject* pObject);
	Actor* Add(const wchar_t* name, Actor* pObject);
	// 取得関数
	std::list<Terrain*>& getTerrainList();
	std::list<Actor*>& getActorList(const wchar_t* name);
	std::map<std::wstring, std::list<Actor*>>& getActors();
	// 更新
	void Update();
	// 描画
	void Render();
	// 削除
	void Clear();
private:
	// コンストラクタ
	ObjectManager();
	// インスタンス
	static ObjectManager* _instance;

	class Impl;
	std::unique_ptr<Impl> _pImpl;
};