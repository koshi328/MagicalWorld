/**
* File: TitleScene.cpp
*
* �^�C�g���V�[��
*
* Date: 2016.11.1
* Author: koshi ito
*/
#include "TitleScene.h"
#include "GameScene.h"
#include "..\..\Utility\Key.h"
#include "..\..\Utility\Mouse.h"
#include "..\..\Utility\MeshManager.h"
#include "..\Scene\SceneManager.h"
#include "..\..\GameBase\Graphics.h"
#include "..\UI\GUI.h"
#include "..\Camera\CameraManager.h"
#include <SimpleMath.h>
#include "..\Object\Stage.h"
// �T�E���h
#include "..\..\Sound\ADX2Le.h"
#include "..\..\Resources\Music\WorkUnit_0\CueSheet_0.h"
/**
* �O���[�o���֐�
*/
extern MeshManager* getMeshManager();
/**
* ���O���
*/
using namespace DirectX::SimpleMath;
/**
* �R���X�g���N�^
*/
TitleScene::TitleScene()
{
}
/**
* �f�X�g���N�^
*/
TitleScene::~TitleScene()
{

}
/**
* ������
*/
void TitleScene::Initialize()
{
	StageManager::getInstance()->Initialize();
	StageManager::getInstance()->setMaxTime(60.f);
	Mouse::getInstance()->setMode(Mouse::MODE_ABSOLUTE);
	CameraManager::getInstance()->setMode(nullptr);
	count_ = 0.f;
	ADX2Le::Play(CRI_CUESHEET_0_BGM01);
}
/**
* �X�V
*/
void TitleScene::Update()
{
	count_++;
}
/**
* �`��
*/
void TitleScene::Render()
{
	// �ϐ����`
	StageManager* stageM = StageManager::getInstance();
	float width = 1280.f;
	float hight = 720.f;
	float flameW = 256.f;
	float flameH = 64.f;
	// �w�i�̐����ƕ`��
	GUI::Sprite titleBack(L"Resources/Texture/titleBack.png", 0, 0, width, hight);
	titleBack.Draw();
	// �^�C�g�������̉��o
	float loopEffect = sinf(count_ * 3.14f / 180);
	// �����̖��邳��ϓ�������
	float light = 1.f + fabs(loopEffect) * 5;
	// �^�C�g�������𐶐��`��
	GUI::Sprite title(L"Resources/Texture/Title.png", 0, loopEffect * 10, width, hight);
	title.Draw(Color(light, light, light, 1.f));
	// �X�^�[�g�{�^����`��
	GUI::Button StartButton(L"Resources/Texture/startButton.png", width / 2 - flameW / 2, hight / 2 + flameH, flameW, flameH);
	// �N���b�N���ꂽ���̏���
	if (StartButton.ClickCheck() == 1 && !SceneManager::getInstance()->getEffectActive())
	{
		SceneManager::getInstance()->setNextScene(SceneManager::SCENE_ID::PLAY);
		SceneManager::getInstance()->ChangeScene(SceneManager::CHANGE_TYPE::BKACK_FADE);
		stageM->setTime(stageM->getMaxTime());
		ADX2Le::Stop();
		ADX2Le::Play(CRI_CUESHEET_0_SE_HIT);
	}
	if (StartButton.ClickCheck() == 2)
		StartButton.Draw(Color(1.f, 1.f, 0.f, 1.f));
	else
		StartButton.Draw();
	// ���Ԑݒ�
	GUI::Button TimeFlame(L"Resources/Texture/Flame.png", width / 2 - flameW / 2, hight / 2 - flameH, flameW, flameH);
	GUI::Button LeftButton(L"Resources/Texture/leftButton.png", width / 2 - flameW / 2 - 32, hight / 2 - flameH, 32, flameH);
	GUI::Button RightButton(L"Resources/Texture/rightButton.png", width / 2 - flameW / 2 + flameW, hight / 2 - flameH, 32, flameH);
	TimeFlame.Draw();
	// �N���b�N���ꂽ���̏���
	if (LeftButton.ClickCheck() == 1)
	{
		if (stageM->getMaxTime() > 60)
		{
			stageM->addMaxTime(-60);
			ADX2Le::Play(CRI_CUESHEET_0_SE_GET);
		}
	}
	if (LeftButton.ClickCheck() == 2)
		LeftButton.Draw(Color(1.f, 1.f, 0.f, 1.f));
	else
		LeftButton.Draw();
	if (RightButton.ClickCheck() == 1)
	{
		if (stageM->getMaxTime() <= 60 * 9)
		{
			stageM->addMaxTime(60);
			ADX2Le::Play(CRI_CUESHEET_0_SE_GET);
		}
	}
	if (RightButton.ClickCheck() == 2)
		RightButton.Draw(Color(1.f, 1.f, 0.f, 1.f));
	else
		RightButton.Draw();
	wchar_t buf[16];
	swprintf_s(buf, 8, L"%.1f", stageM->getMaxTime());
	Graphics::getInstance()->getSpriteFont()->DrawString(Graphics::getInstance()->getSpriteBatch(),
		buf, Vector2(width / 2 - 64, hight / 2 - flameH), Color(0, 0, 0, 1), 0.f, Vector2::Zero, 1.5f);
}