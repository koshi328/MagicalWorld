/**
* File: SceneManager.cpp
*
* ���ׂẴV�[�����Ǘ�����N���X
*
* Date: 2016.11.1
* Author: koshi ito
*/
#include "SceneManager.h"
#include <vector>
#include "TitleScene.h"
#include "GameScene.h"
#include "BlackFade.h"
/**
* static�ϐ�
*/
SceneManager* SceneManager::instance_ = nullptr;

/**
* SceneManager�̃C���v�������g�N���X
*/
class SceneManager::Impl
{
public:
	Scene* mainScene_;	// ���s���V�[��
	Scene* effectScene_;// �؂�ւ��p�V�[��
	Scene* nextScene_;	// ���̃V�[��
public:
	/**
	* �R���X�g���N�^
	*/
	Impl()
	{

	}
	/**
	* �f�X�g���N�^
	*/
	~Impl()
	{
		// �V�[���̍폜
		delete SceneFactory(TITLE);
		delete SceneFactory(PLAY);

		// �G�t�F�N�g�̍폜
		delete ChangeEffectFactory(BKACK_FADE);
	}
	/**
	* ���̃V�[����ݒ肷��
	* @param id -- ���̃V�[����ID
	*/
	void setNextScene(SCENE_ID id)
	{
		nextScene_ = SceneFactory(id);
	}
	/**
	* �V�[����؂�ւ���
	* @param type -- �؂�ւ����̉��o��ID�Ŏw��
	*/
	void ChangeScene(CHANGE_TYPE type)
	{
		if (!nextScene_) return;
		// ���o���w�肵�Ȃ���΂����ɐ؂�ւ���
		if (type == NONE)
		{
			mainScene_ = nextScene_;
			mainScene_->Initialize();
			nextScene_ = nullptr;
		}
		// ���o���g���ꍇ�G�t�F�N�g�𐶐�����
		// �V�[���̐؂�ւ��͉��o�p�V�[���ōs��
		else
		{
			effectScene_ = ChangeEffectFactory(type);
			effectScene_->Initialize();
		}
	}
	/**
	* �X�V
	*/
	void Update()
	{
		if (mainScene_)
			mainScene_->Update();
		if (effectScene_)
			effectScene_->Update();
	}
	/**
	* �`��
	*/
	void Render()
	{
		if (mainScene_)
			mainScene_->Render();
		if (effectScene_)
			effectScene_->Render();
	}
	/**
	* �V�[���̐���
	* @param  id     -- �V�[��ID
	* @return Scene* -- �������ꂽ�V�[��
	*/
	Scene* SceneFactory(SCENE_ID id)
	{
		static TitleScene* title = new TitleScene();
		static GameScene* game = new GameScene();

		Scene* scene = nullptr;
		switch (id)
		{
		case TITLE:
			scene = title;
			break;
		case PLAY:
			scene = game;
			break;
		}

		return scene;
	}
	/**
	* �G�t�F�N�g�̐���
	* @param  type   -- �؂�ւ����oID
	* @return Scene* -- �������ꂽ�V�[��
	*/
	Scene* ChangeEffectFactory(CHANGE_TYPE type)
	{
		static BlackFade* blackFade = new BlackFade();
		Scene* fade = nullptr;
		switch (type)
		{
		case BKACK_FADE:
			fade = blackFade;
			break;
		}

		return fade;
	}
	/**
	* �؂�ւ����o���L������Ԃ�
	* @return bool -- �؂�ւ����o���Ȃ�True
	*/
	bool getEffectActive()
	{
		return effectScene_ ? true : false;
	}
};

/**
* �V���O���g��
*/
SceneManager* SceneManager::getInstance()
{
	if (!instance_)
		instance_ = new SceneManager;
	return instance_;
}
/**
* �R���X�g���N�^
*/
SceneManager::SceneManager() : pImpl_(new Impl())
{

}
/**
* �f�X�g���N�^
*/
SceneManager::~SceneManager()
{
	instance_ = nullptr;
}
/**
* �X�V
*/
void SceneManager::Update()
{
	pImpl_->Update();
}
/**
* �`��
*/
void SceneManager::Render()
{
	pImpl_->Render();
}
/**
* �V�[���̐؂�ւ�
* @param type -- �؂�ւ����oID
*/
void SceneManager::ChangeScene(CHANGE_TYPE type)
{
	pImpl_->ChangeScene(type);
}
/**
* ���̃V�[����ݒ�
* @param id -- ���̃V�[����ID
*/
void SceneManager::setNextScene(SCENE_ID id)
{
	pImpl_->setNextScene(id);
}
/**
* �؂�ւ����o��j������
*/
void SceneManager::DisposeEffect()
{
	pImpl_->effectScene_ = nullptr;
}
/**
* �؂�ւ����o���L�������ׂ�
* @return bool -- �L���Ȃ�True
*/
bool SceneManager::getEffectActive()
{
	return pImpl_->getEffectActive();
}