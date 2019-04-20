/**
* File: BinaryLoader.h
*
* �ǂݍ��݃N���X
*
* Date: 2016.12.25
* Author: koshi ito
*/
#pragma once
#include <vector>
#include <fstream>

class BinaryLoader
{
public:
	// �R���X�g���N�^
	BinaryLoader();
	// �f�X�g���N�^
	virtual ~BinaryLoader();
	// �f�[�^
	std::vector<char> buffer;

	// �f�[�^�̓ǂݍ���
	bool Load(const char* filename);
	bool Load(const wchar_t* filename);
	bool Load(std::istream& stream,size_t size);

	// �f�[�^�̎擾
	char* Data();

	// �f�[�^�̃T�C�Y
	size_t Size();
};