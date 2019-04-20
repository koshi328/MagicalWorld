/**
* File: UseSkill.h
* �s����ԁF�X�L�����g�p����
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#pragma once
#include "AIState.h"

// �O���錾
class Enemy;
class EnemySkill;

class UseSkill : public AIState
{
private:
	Enemy* subject_;
	EnemySkill* skill_;
public:
	UseSkill(Enemy* subject);
	~UseSkill();

	void Entry();
	void Execute();
	void Exit();

	EnemySkill* createSkill(int id);
};