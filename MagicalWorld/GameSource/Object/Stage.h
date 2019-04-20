/**
* File: Stage.h
*
* �Q�[���̃X�e�[�W���Ǘ�����N���X
*
* Date: 2017.1.20
* Author: koshi ito
*/
#pragma once
#include <string>
#include <vector>
#include <SimpleMath.h>
class EnemyCreator;

class StageManager
{
public:
	// �V���O���g��
	static StageManager* getInstance()
	{
		if (!instance_)
			instance_ = new StageManager();
		return instance_;
	}
	// �f�X�g���N�^
	~StageManager()
	{
		instance_ = nullptr;
	}
	// ������
	void Initialize();
	// �X�V
	void Update();
	// �X�e�[�W�ǂݍ���
	bool LoadStageInfo(const wchar_t* filename);
	// �A�N�Z�b�T
	inline void setMaxTime(const float& num) { maxTime_ = num; }
	inline void addMaxTime(const float& num) { maxTime_ += num; }
	inline const float& getMaxTime() { return maxTime_; }
	inline void setTime(const float& num) { time_ = num; }
	inline const float& getTime() { return time_; }
	inline void addTime(const float& num) { time_ += num; }
	inline void setKillEnemyNum(const int& num) { killEnemyNum_ = num; }
	inline void addKillEnemyNum(const int& num) { killEnemyNum_ += num; }
	inline const int& getKillEnemyNum() { return killEnemyNum_; }
	inline void setLevel(const int& num) { stageLevel_ = num; }
	inline void addLevel(const int& num) { stageLevel_ += num; }
	inline const int& getLevel() { return stageLevel_; }
public:
	// �n�`�ݒ�
	bool TerrainSetting(const std::string& str);
	// �C�x���g�ݒ�
	bool IventSetting(const std::string& str);
	// �G�l�~�[�����ݒ�
	EnemyCreator* EnemyCreatorSetting(const std::string& str);
private:
	float maxTime_;		// �Q�[���̐�������
	float time_;		// �o�ߎ���
	int killEnemyNum_;	// �|�����G�l�~�[�̐�
	int stageLevel_;	// �X�e�[�W�̃��x��
	std::vector<DirectX::SimpleMath::Vector3> creatorPos_; // �G�l�~�[�����ʒu
	static StageManager* instance_; // �C���X�^���X
	// �R���X�g���N�^
	StageManager();
};