/**
* File: GameScene.cpp
*
* �Q�[���V�[���̏���
*
* Date: 2016.12.1
* Author: koshi ito
*/

#include "GameScene.h"
#include "..\UI\GUI.h"
#include "..\..\GameBase\Graphics.h"
#include "..\..\Utility\MeshManager.h"
#include "..\Object\Player.h"
#include "..\Object\Enemy.h"
#include "..\Collision\Terrain.h"
#include <SimpleMath.h>
#include "..\..\Utility\TaskManager.h"
#include "..\..\Utility\Key.h"
#include "..\Scene\SceneManager.h"
#include "..\..\RangeMap\RangeMap.h"
// �T�E���h
#include "..\..\Sound\ADX2Le.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"

#include "..\Collision\Collision2D.h"
/**
* �O���[�o���֐�
*/
extern MeshManager* getMeshManager();
extern TaskManager* getTaskManager();
/**
* ���O���
*/
using namespace DirectX::SimpleMath;
using namespace GUI;
/**
* �R���X�g���N�^
*/
GameScene::GameScene()
{
	objectManager_.reset(ObjectManager::getInstance());
	collisionManager_.reset(CollisionManager::getInstance());
	stageManager_.reset(StageManager::getInstance());
	rangeMap_.reset(RangeMap::getInstance());
}
/**
* �f�X�g���N�^
*/
GameScene::~GameScene()
{

}
/**
* ������
*/
void GameScene::Initialize()
{
	// �I�u�W�F�N�g�̏�����
	getTaskManager()->Claer();
	objectManager_->Clear();
	// �X�e�[�W�̐���
	stageManager_->LoadStageInfo(L"Resources/Data/Stage/stage1.txt");
	// �v���C���[�̐���(�X�N���v�g���\��)
	objectManager_->Add(L"Player", new Player(Vector3(0, 0, 0)))->Initialize();
	// BGM�̍Đ�
	ADX2Le::Play(CRI_CUESHEET_0_BGM_WAR);
}
/**
* �X�V
*/
void GameScene::Update()
{
	if (SceneManager::getInstance()->getEffectActive()) return;
	// �w�i�p�V���̓o�^
	getMeshManager()->Entry(3, Matrix::Identity);
	// �I�u�W�F�N�g�̍X�V�������Ăяo��
	objectManager_->Update();
	// �Փ˔�����X�V
	collisionManager_->Update();
	// �X�e�[�W�����X�V
	stageManager_->Update();
	// �_���[�W�T�E���h�̑��d�Đ��h�~�i���j
	Actor::SoundReset();
}
/**
* �`��
*/
void GameScene::Render()
{
	Graphics* graphics = Graphics::getInstance();
	wchar_t buf[32];
	swprintf_s(buf, 32, L"KILL:%d", stageManager_->getKillEnemyNum());
	// �I�u�W�F�N�g�̕`����Ăяo��
	objectManager_->Render();

	// �v���C���[�����Ȃ��ꍇ���U���g��\������
	for each(Player* obj in ObjectManager::getInstance()->getActorList(L"Player"))
	{
		if (obj->getHP() <= 0)
		{
			Sprite result(L"Resources/Texture/Result.png", 0, 0, 1280, 720);
			result.Draw();
			graphics->getSpriteFont()->DrawString(graphics->getSpriteBatch(), buf, Vector2(1280 / 3, 720 / 2), Color(1, 0, 0, 1), 0.f, Vector2::Zero, 2.0f);
			// ���U���g��ʂ��o�Ă��鎞�X�y�[�X�L�[�������ƃ^�C�g���ɑJ��
			if (Key::getInstance()->Touch(VK_SPACE))
			{
				SceneManager::getInstance()->setNextScene(SceneManager::TITLE);
				SceneManager::getInstance()->ChangeScene(SceneManager::BKACK_FADE);
			}
		}
	}
	// ���Ԑ؂�N���A
	if (stageManager_->getTime() <= 0)
	{
		for each(GameObject* obj in ObjectManager::getInstance()->getActorList(L"Enemy"))
		{
			obj->Destroy(obj);
		}
		Sprite result(L"Resources/Texture/Clear.png", 0, 0, 1280, 720);
		result.Draw();
		graphics->getSpriteFont()->DrawString(graphics->getSpriteBatch(), buf, Vector2(1280 / 3, 720 / 2), Color(1, 0, 0, 1), 0.f, Vector2::Zero, 2.0f);
		// ���U���g��ʂ��o�Ă��鎞�X�y�[�X�L�[�������ƃ^�C�g���ɑJ��
		if (Key::getInstance()->Touch(VK_SPACE))
		{
			SceneManager::getInstance()->setNextScene(SceneManager::TITLE);
			SceneManager::getInstance()->ChangeScene(SceneManager::BKACK_FADE);
		}
	}
	// �X�R�A�\��
	graphics->getSpriteFont()->DrawString(graphics->getSpriteBatch(), buf, Vector2(), Color(1, 0, 0, 1), 0.f, Vector2::Zero, 1.5f);

	// ���v�\��
	float x = 1280 / 2 - 32.f;
	float rotation = 6.14f - stageManager_->getTime() / stageManager_->getMaxTime() * 6.28f;
	Sprite TimerFlame(L"Resources/Texture/TimerFlame.png", x, 0, 128, 128);
	Sprite Timer(L"Resources/Texture/Timer.png", x, 0, 128, 128);
	TimerFlame.Draw();
	Timer.Draw(Color(1, 1, 1, 1), rotation);
	//ShadowMap::getInstance()->Render();

}