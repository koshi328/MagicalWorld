/**
* File: TitleScene.h
*
* �^�C�g���V�[��
*
* Date: 2016.11.1
* Author: koshi ito
*/
#pragma once

#include "Scene.h"

class TitleScene : public Scene
{
public:
	// �R���X�g���N�^
	TitleScene();
	// �f�X�g���N�^
	~TitleScene();
	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Render();
private:
	float count_; // �o�ߎ���
};