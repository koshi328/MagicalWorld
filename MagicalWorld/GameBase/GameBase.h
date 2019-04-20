/**
* File: GameBase.h
*
* �Q�[���̊�b�����̏���
*
* Date: 2016.11.30
* Author: koshi ito
*/
#pragma once

#include <memory>

class GameBase
{
public:
	// �R���X�g���N�^
	GameBase(GameBase* pGame);
	// �f�X�g���N�^
	~GameBase();
	// ������
	virtual void Initialize();
	// �X�V
	virtual void Update();
	// �`��
	virtual void Render();
	// �I��
	virtual void Finalize();
	// ���s
	void Run();
private:
	class Impl;
	std::unique_ptr<Impl> _pImpl;
};