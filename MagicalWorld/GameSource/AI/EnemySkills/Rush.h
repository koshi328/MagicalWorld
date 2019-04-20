/**
* File: Rush.h
* �X�L�� Rush(��F�ːi)
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