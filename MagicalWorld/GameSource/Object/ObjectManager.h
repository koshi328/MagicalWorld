/**
* File: ObjectManager.h
*
* ���ׂẴI�u�W�F�N�g���Ǘ�
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
	// �V���O���g��
	static ObjectManager* getInstance();
	// �f�X�g���N�^
	~ObjectManager();
	// �ǉ��֐�
	Terrain* Add(Terrain* pObject);
	GameObject* Add(GameObject* pObject);
	Actor* Add(const wchar_t* name, Actor* pObject);
	// �擾�֐�
	std::list<Terrain*>& getTerrainList();
	std::list<Actor*>& getActorList(const wchar_t* name);
	std::map<std::wstring, std::list<Actor*>>& getActors();
	// �X�V
	void Update();
	// �`��
	void Render();
	// �폜
	void Clear();
private:
	// �R���X�g���N�^
	ObjectManager();
	// �C���X�^���X
	static ObjectManager* _instance;

	class Impl;
	std::unique_ptr<Impl> _pImpl;
};