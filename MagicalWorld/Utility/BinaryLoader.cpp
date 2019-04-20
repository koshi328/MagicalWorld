/**
* File: BinaryLoader.h
*
* �ǂݍ��݃N���X
*
* Date: 2016.12.25
* Author: koshi ito
*/
#include "BinaryLoader.h"
/**
* ���O���
*/
using namespace std;

namespace
{
	size_t FileSize(istream& fs)
	{
		fs.seekg(0, ios::end);
		size_t size = (size_t)fs.tellg();
		fs.seekg(0, ios::beg);

		return size;
	}
}
/**
* �R���X�g���N�^
*/
BinaryLoader::BinaryLoader()
{
}
/**
* �f�X�g���N�^
*/
BinaryLoader::~BinaryLoader()
{
}
/**
* �ǂݍ��݊֐�
* @param filename -- �t�@�C���p�X
*/
bool BinaryLoader::Load(const char* filename)
{
	auto fs = ifstream(filename, ios::in | ios::binary);

	return Load(fs, FileSize(fs));
}
/**
* �ǂݍ��݊֐�
* @param filename -- �t�@�C���p�X
*/
bool BinaryLoader::Load(const wchar_t* filename)
{
	auto fs = ifstream(filename, ios::in | ios::binary);

	return Load(fs, FileSize(fs));
}
/**
* �ǂݍ��݊֐�
* @param fs   -- �ǂݍ��ނ�fileStream
* @param size -- �t�@�C���T�C�Y
* @return bool -- �ǂݍ��߂Ȃ�������false
*/
bool BinaryLoader::Load(std::istream& fs,size_t size)
{
	if (!fs) return false;

	buffer.resize(size);

	fs.read(buffer.data(), buffer.size());

	return true;
}
/**
* �ǂݍ���data��Ԃ�
* @return char* -- �ǂݍ��񂾃f�[�^
*/
char* BinaryLoader::Data()
{
	if (buffer.empty()) return nullptr;

	return buffer.data();
}
/**
* �ǂݍ��񂾃t�@�C���̃T�C�Y��Ԃ�
* @return size_t -- �t�@�C���T�C�Y
*/
size_t BinaryLoader::Size()
{
	return buffer.size();
}