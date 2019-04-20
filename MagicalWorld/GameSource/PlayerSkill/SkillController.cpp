/**
* File: SkillController.cpp
*
* �v���C���[�̃X�L���̎��s���Ǘ�����N���X
*
* Date: 2016.12.5
* Author: koshi ito
*/

#include "SkillController.h"
#include "Skill.h"
#include "..\..\Utility\Mouse.h"
#include "..\..\Sound\ADX2Le.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"
// �X�L���h���N���X�̃C���N���[�h
#include "Swing.h"
#include "Heel.h"
#include "OverRay.h"
#include "RingBurst.h"
/**
* �R���X�g���N�^
* @param subject -- �Ώ�Actor�̃|�C���^
*/
SkillController::SkillController(Actor* subject)
{
	nowSkillNum_ = 0;
	ZeroMemory(skills_, sizeof(Skill*) * SKILL_NUM);
	Cursor_ = 0;
	subject_ = subject;
	executeSkill_ = -1;
	setSkill(10, 0);
}
/**
* �f�X�g���N�^
*/
SkillController::~SkillController()
{
	for (int i = 0; i < SKILL_NUM; i++)
	{
		if (!skills_[i]) continue;
		delete skills_[i];
	}
}
/**
* �w��X���b�g�Ɏw��̃X�L����ݒ肷��
* @param number -- �X���b�g�̔ԍ�
* @param id     -- �o�^�������X�L����ID
*/
void SkillController::setSkill(int number, int id)
{
	if (skills_[number])
	{
		delete skills_[number];
	}
	skills_[number] = createSkill(id);
}
/**
* �w��X���b�g�̃X�L�������s
* @param number -- ���s�������X���b�g�̔ԍ�
*/
void SkillController::setExecute(int number)
{
	executeSkill_ = number;
	// �w�肵���ԍ��ɃX�L��������������
	if (!skills_[executeSkill_])
	{
		executeSkill_ = -1;
		return;
	}
	skills_[executeSkill_]->Entry();
}
/**
* �w��X���b�g�̃X�L��ID�̎擾
* @param number -- �X���b�g�̔ԍ�
* @return const int& -- �X�L����ID
*/
const int& SkillController::getID(int number)
{
	if (!skills_[number]) return -1;
	return skills_[number]->getID();
}
/**
* �w��ID�̃X�L���𐶐����ĕԂ�
* @param id      -- �X�L��ID
* @return Skill* -- �������ꂽ�X�L���̃|�C���^
*/
Skill* SkillController::createSkill(int id)
{
	Skill* skill = nullptr;
	switch (id)
	{
	case 0:
		skill = new Swing(subject_);
		break;
	case 1:
		skill = new Heel(subject_);
		break;
	case 2:
		skill = new RingBurst(subject_);
		break;
	case 3:
		skill = new OverRay(subject_);
		break;
	default:
		skill = new Swing(subject_);
		break;
	}
	skill->setID(id);
	return skill;
}

/**
* �J�[�\���̈ʒu���擾
*/
int SkillController::getCursor()
{
	return Cursor_;
}
/**
* �w��ԍ��̃X�L�����擾
* @param  number -- �X���b�g�̔ԍ�
* @return Skill* -- �w��X���b�g�̃X�L���̃|�C���^
*/
Skill* SkillController::getSkill(const int& number)
{
	return skills_[number];
}
/**
* �X�L���X���b�g���w��ʒu���獶�l�߂ɂ���
* @param begin -- �\�[�g�̊�ʒu
*/
void SkillController::sortSkill(int begin)
{
	int i;
	for (i = begin; i < SKILL_NUM - 2; i++)
	{
		skills_[i] = skills_[i + 1];
	}
	skills_[SKILL_NUM - 2] = nullptr;
}
/**
* �擾���Ă���X�L���̐����擾
* @return int -- �X�L���̐�
*/
int SkillController::getNowSkillNum()
{
	return nowSkillNum_;
}
/**
* �X�L���̐���ݒ肷��
*/
void SkillController::setNowSkillNum(const int& num)
{
	nowSkillNum_ = num;
}

/**
* ���s
*/
bool SkillController::Run()
{
	int i;
	// ���L���X�g�^�C���̌v�Z
	for (i = 0; i < SKILL_NUM; i++)
	{
		if (!skills_[i]) continue;
		int recast = skills_[i]->getRecastTime();
		if (recast != 0)
		{
			skills_[i]->setRecastTime(recast - 1);
		}
	}
	// �g�p�����X�L���ɃJ�[�\�������킹��
	int tmp = Cursor_;
	Cursor_ = Mouse::getInstance()->getWheelValue() % 10;
	Cursor_ = abs(Cursor_);
	// �J�[�\�����ړ������特��炷
	if (tmp != Cursor_)
	{
		ADX2Le::Play(CRI_CUESHEET_0_SE_GET);
	}
	// ���N���b�N�Œʏ�U���X�L���𔭓�
	if (Mouse::getInstance()->Touch(Mouse::LEFT) && executeSkill_ == -1)
	{
		if (skills_[SKILL_NUM - 1])
		{
			if (skills_[SKILL_NUM - 1]->getRecastTime() == 0)
				setExecute(SKILL_NUM - 1);
		}
	}
	// �E�N���b�N�œ���X�L���𔭓�
	if (Mouse::getInstance()->Touch(Mouse::RIGHT) && executeSkill_ == -1)
	{
		if (skills_[Cursor_])
		{
			if (skills_[Cursor_]->getRecastTime() == 0)
				setExecute(Cursor_);
		}
	}
	// �X�L�����g�p���Ă��Ȃ���
	if (executeSkill_ == -1) return false;

	// �X�L���̎��s���I��������
	if (!skills_[executeSkill_]->Execute())
	{
		skills_[executeSkill_]->Exit();
		if (executeSkill_ != SKILL_NUM - 1)
		{
			sortSkill(executeSkill_);
			nowSkillNum_--;
		}

		executeSkill_ = -1;
		return false;
	}
	// �X�L�������s���������ꍇ
	return true;
}