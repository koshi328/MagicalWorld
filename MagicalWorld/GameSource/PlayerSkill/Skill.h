/**
* File: Skill.h
* �v���C���[���g���X�L���̊��N���X
*
* Date: 2016.11.17
* Author: koshi ito
*/

#pragma once
/**
* �O���錾
*/
class Actor;

class Skill
{
protected:
	int id_;		// �X�L��ID
	int recastTime_;// ���L���X�g����
	Actor* subject_;// �Ώێ�
public:
	// �R���X�g���N�^
	Skill() = default;
	// �f�X�g���N�^
	virtual ~Skill() = default;

	// �o�^
	virtual void Entry() = 0;
	// ���s
	virtual bool Execute() = 0;
	// �I��
	virtual void Exit() = 0;

	// �A�N�Z�b�T
	inline void setID(const int& n) { id_ = n; }
	inline const int& getID() { return id_; }
	inline const int& getRecastTime() { return recastTime_; }
	inline void setRecastTime(int time) { recastTime_ = time; }
};