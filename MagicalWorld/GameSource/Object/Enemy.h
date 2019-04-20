/**
* File: Enemy.h
*
* �G�l�~�[�̋��ʃA���S���Y��
*
* Date: 2016.12.20
* Author: koshi ito
*/
#pragma once
#include "Actor.h"
#include "..\AI\AIState.h"
#include "..\Collision\Collision.h"
#include <list>
#include <vector>

class EnemyCreator;
/**
* �G���Ǘ��\����
*/
struct TARGET
{
	GameObject* target;	// �Ώ�
	int hate;			// �G��
};

class Enemy : public Actor
{
private:
	std::list<TARGET*> targetList_;		// �G�����X�g
	std::vector<int> skillNumberList_;	// �g�p����X�L���̔ԍ����X�g
	int skillDelay_;					// �X�L���d��
	unsigned int skillID_;				// �g�p���Ă���X�L����ID
	Sphere collision_;					// �Փ˔���R���C�_�[
	AIState* state_;					// AI�̏��
	float seachRange_;					// ���G�͈�
	int modelID_;						// �\�����郂�f����ID
	EnemyCreator* myCreator_;			// �쐬��
	int point_;							// �X�R�A
	int unrivaledTime_;
public:
	Enemy(Enemy* copy);
	Enemy();
	~Enemy();
	// �^�[�Q�b�g���X�V
	void AmbientSearch();

	// State�̕ύX
	void ChangeState(AIState* state);

	// �Փ˔���ւ̃A�N�Z�X
	inline Collision* getCollision();
	// �^�[�Q�b�g���փA�N�Z�X
	inline std::list<TARGET*>& getTargetInfo() { return targetList_; }
	// �X�L���ɃA�N�Z�X
	inline void setSkillDelay(const int& time) { skillDelay_ = time; }
	inline const int& getSkillDelay() { return skillDelay_; }
	inline void setUseSkillID(const int& id) { skillID_ = id; }
	inline const unsigned int& getUseSkillID() { return skillID_; }
	inline std::vector<int>& getSkillList() { return skillNumberList_; }

	void Initialize();
	void Update();
	void Draw();

	bool LoadStatus(const wchar_t* filename);
	void setCreator(EnemyCreator* creator);

private:
	void EnemysRebound();
};