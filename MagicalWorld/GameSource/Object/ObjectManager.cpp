/**
* File: ObjectManager.cpp
*
* すべてのオブジェクトを管理
*
* Date: 2016.12.10
* Author: koshi ito
*/
#include "ObjectManager.h"
#include <map>

/**
* 名前空間
*/
using namespace std;

// スタティック変数
ObjectManager* ObjectManager::_instance = nullptr;

/**
* オブジェクトマネージャーのインプリメントクラス
*/
class ObjectManager::Impl
{
public:
	map<wstring, list<Actor*>> _actorList;	// Actorを管理するリスト
	list<Terrain*> _terrainList;			// 地形を管理するリスト
	list<GameObject*> _otherList;			// その他のオブジェクトを管理するリスト
	
	/**
	* 更新
	*/
	void Update()
	{
		// マップ内の全要素を更新する
		map<wstring, list<Actor*>>::iterator mp = _actorList.begin();
		for (; mp != _actorList.end(); mp++)
		{
			list<Actor*>::iterator it = mp->second.begin();
			for (; it != mp->second.end();)
			{
				(*it)->Update();
				if ((*it)->DestroyMessage())
				{
					delete *it;
					it = mp->second.erase(it);
					continue;
				}
				++it;
			}
		}
		// その他のオブジェクトの更新
		list<GameObject*>::iterator it = _otherList.begin();
		for (; it != _otherList.end();)
		{
			(*it)->Update();
			if ((*it)->DestroyMessage())
			{
				delete *it;
				it = _otherList.erase(it);
				continue;
			}
			++it;
		}

		// 地形の描画
		list<Terrain*>::iterator terrain = _terrainList.begin();
		for (; terrain != _terrainList.end(); terrain++)
		{
			(*terrain)->Update();
		}
	}
	/**
	* 描画
	*/
	void Render()
	{
		// マップ内の全要素を描画
		map<wstring, list<Actor*>>::iterator mp = _actorList.begin();
		for (; mp != _actorList.end(); mp++)
		{
			list<Actor*>::iterator it = mp->second.begin();
			for (; it != mp->second.end(); it++)
			{
				(*it)->Draw();
			}
		}
		// その他のオブジェクトの描画
		list<GameObject*>::iterator other = _otherList.begin();
		for (; other != _otherList.end();other++)
		{
			(*other)->Draw();
		}

		// 地形の描画
		list<Terrain*>::iterator terrain = _terrainList.begin();
		for (; terrain != _terrainList.end(); terrain++)
		{
			(*terrain)->Draw();
		}
	}

	/**
	* 削除
	*/
	void Clear()
	{
		// マップ内の全要素を削除
		map<wstring, list<Actor*>>::iterator mp = _actorList.begin();
		for (; mp != _actorList.end(); mp++)
		{
			list<Actor*>::iterator it = mp->second.begin();
			for (; it != mp->second.end();)
			{
				delete *it;
				it = mp->second.erase(it);
			}
		}
		// その他のオブジェクトの削除
		list<GameObject*>::iterator other = _otherList.begin();
		for (; other != _otherList.end();)
		{
			delete *other;
			other = _otherList.erase(other);
		}

		// 地形の削除
		list<Terrain*>::iterator terrain = _terrainList.begin();
		for (; terrain != _terrainList.end();)
		{
			delete *terrain;
			terrain = _terrainList.erase(terrain);
		}
	}
};
/**
* インスタンスの取得
*/
ObjectManager* ObjectManager::getInstance()
{
	if (!_instance)
		_instance = new ObjectManager();
	return _instance;
}

/**
* コンストラクタ
*/
ObjectManager::ObjectManager() : _pImpl(new Impl())
{

}
/**
* デストラクタ
*/
ObjectManager::~ObjectManager()
{
	_pImpl->Clear();
}

/**
* オブジェクトの登録
* @param pObject --- Terrainのポインタ
*/
Terrain* ObjectManager::Add(Terrain* pObject)
{
	if (!pObject) return nullptr;
	_pImpl->_terrainList.push_back(pObject);
	return pObject;
}
/**
* オブジェクトの登録
* @param pObject --- GameObjectのポインタ
*/
GameObject* ObjectManager::Add(GameObject* pObject)
{
	if (!pObject) return nullptr;
	_pImpl->_otherList.push_back(pObject);
	return pObject;
}
/**
* オブジェクトの登録
* @param name    --- 登録するグループに任意に名前を付ける
* @param pObject --- Actorのポインタ
*/
Actor* ObjectManager::Add(const wchar_t* name, Actor* pObject)
{
	if (!pObject) return nullptr;
	_pImpl->_actorList[name].push_back(pObject);
	return pObject;
}
/**
* 地形のリストを取得
*/
std::list<Terrain*>& ObjectManager::getTerrainList()
{
	return _pImpl->_terrainList;
}
/**
* Actorのリストを取得
* @param name --- 取得したいグループの名前を指定
*/
std::list<Actor*>& ObjectManager::getActorList(const wchar_t* name)
{
	return _pImpl->_actorList[name];
}
/**
* すべてのActorのリストを取得
*/
 std::map<std::wstring, std::list<Actor*>>& ObjectManager::getActors()
{
	return _pImpl->_actorList;
}
/**
* 更新
*/
void ObjectManager::Update()
{
	_pImpl->Update();
}
/**
* 描画
*/
void ObjectManager::Render()
{
	_pImpl->Render();
}
/**
* 削除
*/
void ObjectManager::Clear()
{
	_pImpl->Clear();
}