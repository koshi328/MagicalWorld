/**
* File: SceneManager.h
*
* ���ׂẴV�[�����Ǘ�����N���X
*
* Date: 2016.11.1
* Author: koshi ito
*/
#pragma once

#include "Scene.h"
#include <memory>

class SceneManager
{
public:
	// �V�[���̎��
	enum SCENE_ID
	{
		TITLE,
		PLAY,
	};
	// �؂�ւ����o�̎��
	enum CHANGE_TYPE
	{
		NONE,
		BKACK_FADE,
	};
public:
	// �V���O���g��
	static SceneManager* getInstance();
	// �f�X�g���N�^
	~SceneManager();
	// ���̃V�[�������߂�
	void setNextScene(SCENE_ID id);
	// �V�[����؂�ւ���
	void ChangeScene(CHANGE_TYPE type = NONE);
	// �؂�ւ����o��j������
	void DisposeEffect();
	// �؂�ւ����o��L���ɂ���
	bool getEffectActive();
	// �X�V
	void Update();
	// �`��
	void Render();
private:
	// �R���X�g���N�^
	SceneManager();
	// �C���v�������g�N���X
	class Impl;
	std::unique_ptr<Impl> pImpl_;
	// �C���X�^���X
	static SceneManager* instance_;
};