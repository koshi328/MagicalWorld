/**
* File: EnemySkill.cpp
* エネミーのスキルの基底クラス
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#include "EnemySkill.h"

EnemySkill::EnemySkill()
:subject_(nullptr),castTime_(0),delayTime_(0)
{

}

EnemySkill::~EnemySkill()
{

}