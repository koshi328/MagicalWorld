/**
* File: Game.h
*
* �Q�[���̃��C������
*
* Date: 2016.11.30
* Author: koshi ito
*/
#pragma once

#include <memory>
#include "GameBase.h"
#include "Graphics.h"

class Game : public GameBase
{
public:
	// �R���X�g���N�^
	Game();
	// �f�X�g���N�^
	~Game();
	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Render();
	// �I��
	void Finalize();
private:
	class Impl;
	std::unique_ptr<Impl> _pImpl;
};