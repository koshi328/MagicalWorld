/**
* File: CollisionManager.cpp
*
* �܂Ƃ߂����Փ˔�����Ǘ�����
*
* Date: 2016.11.18
* Author: koshi ito
*/

#include "CollisionManager.h"
#include "..\Object\ObjectManager.h"
#include "Collision.h"
#include "Terrain.h"
#include "..\Object\Actor.h"
#include <SimpleMath.h>

using namespace std;
using namespace DirectX::SimpleMath;

// static�̎��̐錾
CollisionManager* CollisionManager::instance_;

/**
* �X�V
*/
void CollisionManager::Update()
{
	using ACTOR_MAP = map<wstring, list<Actor*>>;
	ACTOR_MAP actors = ObjectManager::getInstance()->getActors();
	ACTOR_MAP::iterator it = actors.begin();
	for (; it != actors.end(); it++)
	{
		list<Actor*>::iterator object = (*it).second.begin();
		for (; object != (*it).second.end(); object++)
		{
			if (!(*object)->getCollision()) continue;
			// �n�`���[�v
			list<Terrain*> terrainList = ObjectManager::getInstance()->getTerrainList();
			list<Terrain*>::iterator terrain = terrainList.begin();
			for (; terrain != terrainList.end(); terrain++)
			{
				Sphere col;
				col.Center = (*object)->Position();
				col.Radius = (*object)->getCollision()->getMaxRange();
				Vector3 reject;
				if ((*terrain)->IntersectSphere(col, &reject))
				{
					(*object)->Position((*object)->Position() + reject);
					(*object)->setFall(false);
				}
				else
				{
					(*object)->setFall(true);
				}
			}
		}
	}
}
