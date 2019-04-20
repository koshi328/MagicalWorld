/**
* File: SkillController.h
*
* �v���C���[�̃X�L���̎��s���Ǘ�����N���X
*
* Date: 2016.12.5
* Author: koshi ito
*/
#pragma once
/**
* �萔�錾
*/
#define SKILL_NUM (11)
#define SKILL_TYPE_NUM (4)
/**
* �O���錾
*/
class Skill;
class Actor;

class SkillController
{
private:
	Skill* skills_[SKILL_NUM];	// �X�L���̃X���b�g
	int executeSkill_;			// ���s���Ă���X�L����ID
	Actor* subject_;			// �Ώێ�
	int Cursor_;				// �X�L���X���b�g�̎w���Ă���ԍ�
	int nowSkillNum_;			// �擾���Ă���X�L���̐�
private:
	// 
	Skill* createSkill(int id);
public:
	// �R���X�g���N�^
	SkillController(Actor* subject);
	// �f�X�g���N�^
	~SkillController();

	// ���s
	bool Run();
	// �X�L�������l�߂ɂ���
	void sortSkill(int begin);
	// �A�N�Z�b�T
	void setSkill(int number, int id);
	const int& getID(int number);
	void setExecute(int number);
	Skill* getSkill(const int& number);
	int getCursor();
	int getNowSkillNum();
	void setNowSkillNum(const int& num);
};