/**
* File: BlackFade.h
*
* �����t�F�[�h�����ʑJ�ډ��o
*
* Date: 2017.1.20
* Author: koshi ito
*/
#pragma once

#include "Scene.h"
#include "..\UI\GUI.h"

class BlackFade : public Scene
{
public:
	// �R���X�g���N�^
	BlackFade();
	// �f�X�g���N�^
	~BlackFade();
	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Render();
private:
	int time_;				// �t�F�[�h����
	float alpha_;			// �A���t�@�l
	GUI::Sprite* filter_;	// �e�N�X�`��
};
