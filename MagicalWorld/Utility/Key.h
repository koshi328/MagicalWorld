/**
* File: key.h
* �ǂݍ��݃N���X
*
* Date: 2016.12.5
* Author: koshi ito
*/
#pragma once

#include <windows.h>
#include <memory>

class Key
{
public:
	// �V���O���g��
	static Key* getInstance();
	// �f�X�g���N�^
	~Key();
	// �����ꂽ��
	bool Touch(BYTE k);
	// ������Ă��鎞
	bool Press(BYTE k);
	// ���ꂽ��
	bool Release(BYTE k);
	// �X�V
	void Update();
private:
	// �R���X�g���N�^
	Key();
	// �C���v�������g�N���X
	class Impl;
	std::unique_ptr<Impl> _pImpl;
	// �C���X�^���X
	static Key* _instance;
};