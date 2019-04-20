/**
* File: RollAttack.h
* �X�L�� RollAttack(��F��]�U��)
*
* Data:   2017.1.10
* Author: Koshi ito
*/
#pragma once
#include "EnemySkill.h"

class RollAttack : public EnemySkill
{
private:
	float saveAngle_;
public:
	RollAttack(Enemy* subject);
	~RollAttack();

	void Entry();
	bool Execute();
	void Exit();
};