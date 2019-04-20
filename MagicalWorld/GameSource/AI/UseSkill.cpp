/**
* File: UseSkill.cpp
* �s����ԁF�X�L�����g�p����
*
* Data:   2016.12.9
* Author: Koshi ito
*/
#include "UseSkill.h"
#include "Follow.h"
#include "..\Object\Enemy.h"
#include <SimpleMath.h>

// �X�L���ꗗ
#include "EnemySkills\HammerOfAnger.h"
#include "EnemySkills\Cleave.h"
#include "EnemySkills\RollAttack.h"
#include "EnemySkills\BombSetting.h"
#include "EnemySkills\Rush.h"

using namespace std;
using namespace DirectX::SimpleMath;

/**
* �R���X�g���N�^
* @param subject -- ����Ώ�
*/
UseSkill::UseSkill(Enemy* subject)
	:subject_(subject), skill_(nullptr)
{
	
}

/**
* �f�X�g���N�^
*/
UseSkill::~UseSkill()
{
	if (skill_)
		delete skill_;
}

/**
* ������
*/
void UseSkill::Entry()
{
	subject_->setUseSkillID((subject_->getUseSkillID() + 1) % (subject_->getSkillList().size()));
	skill_ = createSkill(subject_->getSkillList()[subject_->getUseSkillID()]);
	if(skill_)
		skill_->Entry();
}

/**
* ���s
*/
void UseSkill::Execute()
{
	// �X�L���̎��s���I������܂ŌĂяo��
	if (skill_->Execute()) return;
	// �f�B���C�^�C����ݒ肷��
	subject_->setSkillDelay(skill_->getDelayTime());
	// ���̍s�����I������
	subject_->ChangeState(new Follow(subject_));
}

/**
* �I��
*/
void UseSkill::Exit()
{
	// �g�p�����X�L���̔j��
	skill_->Exit();
	delete skill_;
	skill_ = nullptr;
}

/**
* �X�L���𐶐�����֐�
* @param id -- �����������X�L����ID
* @return(EnemySkill*) -- ���������X�L��
*/
EnemySkill* UseSkill::createSkill(int id)
{
	EnemySkill* tmp = nullptr;
	switch (id)
	{
	case 0:
		tmp = new HammerOfAnger(subject_);
		break;
	case 1:
		tmp = new RollAttack(subject_);
		break;
	case 2:
		tmp = new BombSetting(subject_);
		break;
	case 3: tmp = new Rush(subject_);
		break;
	}
	return tmp;
}