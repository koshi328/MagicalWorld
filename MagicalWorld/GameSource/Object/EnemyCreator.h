/**
* File: EnemyCreator.h
*
* �G�l�~�[�̐����|�C���g
*
* Date: 2016.12.20
* Author: koshi ito
*/
#pragma once
#include "Actor.h"
#include <vector>
#include "Enemy.h"

class EnemyCreator : public Actor
{
public:
	// �R���X�g���N�^
	EnemyCreator();
	// �f�X�g���N�^
	~EnemyCreator();
	
	// ������
	void Initialize() override;
	// �X�V
	void Update() override;
	// �`��
	void Draw() override;
	// �ő吔�̐ݒ�
	void setMaxCreateNum(int num);
	// �����p�x�̐ݒ�
	void setFrequency(int num);
	// ��������G�l�~�[�̃v���g�^�C�v��o�^
	void setEnemyPrototype(Enemy* enemy);
	// ���������J�E���g
	void addCreateNum(int num);
private:
	int maxCreateNum_;					// �ő吔
	int nowCreateNum_;					// ���݂̐�
	int frequency_;						// �����p�x
	int time_;							// �o�ߎ���
	std::vector<Enemy*> prototypeList_;	// �v���g�^�C�v�̃��X�g
};