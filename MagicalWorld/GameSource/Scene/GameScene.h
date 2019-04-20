/**
* File: GameScene.h
*
* �Q�[���V�[���̏���
*
* Date: 2016.12.1
* Author: koshi ito
*/
#pragma once

#include "Scene.h"
#include "..\Object\ObjectManager.h"
#include "..\Collision\CollisionManager.h"
#include "..\Object\Stage.h"
#include "..\..\RangeMap\RangeMap.h"
#include "..\..\Sound\ADX2Le.h"

class GameScene : public Scene
{
public:
	// �R���X�g���N�^
	GameScene();
	// �f�X�g���N�^
	~GameScene();

	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Render();
private:
	// �I�u�W�F�N�g�}�l�[�W���[
	std::unique_ptr<ObjectManager> objectManager_;
	// �R���W�����}�l�[�W���[
	std::unique_ptr<CollisionManager> collisionManager_;
	std::unique_ptr<StageManager> stageManager_;
	std::unique_ptr<RangeMap> rangeMap_;
	ADX2Le_Player* bgm_;
};