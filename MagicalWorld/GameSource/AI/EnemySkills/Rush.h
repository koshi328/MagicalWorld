/**
* File: Rush.h
* スキル Rush(訳：突進)
*
* Data:   2017.1.10
* Author: Koshi ito
*/
#pragma once
#include "EnemySkill.h"

class Rush : public EnemySkill
{
private:

public:
	Rush(Enemy* subject);
	~Rush();

	void Entry();
	bool Execute();
	void Exit();
};