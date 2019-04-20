/**
* File: ObjectManager.cpp
*
* ���ׂẴI�u�W�F�N�g���Ǘ�
*
* Date: 2016.12.10
* Author: koshi ito
*/
#include "ObjectManager.h"
#include <map>

/**
* ���O���
*/
using namespace std;

// �X�^�e�B�b�N�ϐ�
ObjectManager* ObjectManager::_instance = nullptr;

/**
* �I�u�W�F�N�g�}�l�[�W���[�̃C���v�������g�N���X
*/
class ObjectManager::Impl
{
public:
	map<wstring, list<Actor*>> _actorList;	// Actor���Ǘ����郊�X�g
	list<Terrain*> _terrainList;			// �n�`���Ǘ����郊�X�g
	list<GameObject*> _otherList;			// ���̑��̃I�u�W�F�N�g���Ǘ����郊�X�g
	
	/**
	* �X�V
	*/
	void Update()
	{
		// �}�b�v���̑S�v�f���X�V����
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
		// ���̑��̃I�u�W�F�N�g�̍X�V
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

		// �n�`�̕`��
		list<Terrain*>::iterator terrain = _terrainList.begin();
		for (; terrain != _terrainList.end(); terrain++)
		{
			(*terrain)->Update();
		}
	}
	/**
	* �`��
	*/
	void Render()
	{
		// �}�b�v���̑S�v�f��`��
		map<wstring, list<Actor*>>::iterator mp = _actorList.begin();
		for (; mp != _actorList.end(); mp++)
		{
			list<Actor*>::iterator it = mp->second.begin();
			for (; it != mp->second.end(); it++)
			{
				(*it)->Draw();
			}
		}
		// ���̑��̃I�u�W�F�N�g�̕`��
		list<GameObject*>::iterator other = _otherList.begin();
		for (; other != _otherList.end();other++)
		{
			(*other)->Draw();
		}

		// �n�`�̕`��
		list<Terrain*>::iterator terrain = _terrainList.begin();
		for (; terrain != _terrainList.end(); terrain++)
		{
			(*terrain)->Draw();
		}
	}

	/**
	* �폜
	*/
	void Clear()
	{
		// �}�b�v���̑S�v�f���폜
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
		// ���̑��̃I�u�W�F�N�g�̍폜
		list<GameObject*>::iterator other = _otherList.begin();
		for (; other != _otherList.end();)
		{
			delete *other;
			other = _otherList.erase(other);
		}

		// �n�`�̍폜
		list<Terrain*>::iterator terrain = _terrainList.begin();
		for (; terrain != _terrainList.end();)
		{
			delete *terrain;
			terrain = _terrainList.erase(terrain);
		}
	}
};
/**
* �C���X�^���X�̎擾
*/
ObjectManager* ObjectManager::getInstance()
{
	if (!_instance)
		_instance = new ObjectManager();
	return _instance;
}

/**
* �R���X�g���N�^
*/
ObjectManager::ObjectManager() : _pImpl(new Impl())
{

}
/**
* �f�X�g���N�^
*/
ObjectManager::~ObjectManager()
{
	_pImpl->Clear();
}

/**
* �I�u�W�F�N�g�̓o�^
* @param pObject --- Terrain�̃|�C���^
*/
Terrain* ObjectManager::Add(Terrain* pObject)
{
	if (!pObject) return nullptr;
	_pImpl->_terrainList.push_back(pObject);
	return pObject;
}
/**
* �I�u�W�F�N�g�̓o�^
* @param pObject --- GameObject�̃|�C���^
*/
GameObject* ObjectManager::Add(GameObject* pObject)
{
	if (!pObject) return nullptr;
	_pImpl->_otherList.push_back(pObject);
	return pObject;
}
/**
* �I�u�W�F�N�g�̓o�^
* @param name    --- �o�^����O���[�v�ɔC�ӂɖ��O��t����
* @param pObject --- Actor�̃|�C���^
*/
Actor* ObjectManager::Add(const wchar_t* name, Actor* pObject)
{
	if (!pObject) return nullptr;
	_pImpl->_actorList[name].push_back(pObject);
	return pObject;
}
/**
* �n�`�̃��X�g���擾
*/
std::list<Terrain*>& ObjectManager::getTerrainList()
{
	return _pImpl->_terrainList;
}
/**
* Actor�̃��X�g���擾
* @param name --- �擾�������O���[�v�̖��O���w��
*/
std::list<Actor*>& ObjectManager::getActorList(const wchar_t* name)
{
	return _pImpl->_actorList[name];
}
/**
* ���ׂĂ�Actor�̃��X�g���擾
*/
 std::map<std::wstring, std::list<Actor*>>& ObjectManager::getActors()
{
	return _pImpl->_actorList;
}
/**
* �X�V
*/
void ObjectManager::Update()
{
	_pImpl->Update();
}
/**
* �`��
*/
void ObjectManager::Render()
{
	_pImpl->Render();
}
/**
* �폜
*/
void ObjectManager::Clear()
{
	_pImpl->Clear();
}