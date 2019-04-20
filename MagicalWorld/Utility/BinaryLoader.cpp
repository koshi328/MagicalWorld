/**
* File: BinaryLoader.h
*
* 読み込みクラス
*
* Date: 2016.12.25
* Author: koshi ito
*/
#include "BinaryLoader.h"
/**
* 名前空間
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
* コンストラクタ
*/
BinaryLoader::BinaryLoader()
{
}
/**
* デストラクタ
*/
BinaryLoader::~BinaryLoader()
{
}
/**
* 読み込み関数
* @param filename -- ファイルパス
*/
bool BinaryLoader::Load(const char* filename)
{
	auto fs = ifstream(filename, ios::in | ios::binary);

	return Load(fs, FileSize(fs));
}
/**
* 読み込み関数
* @param filename -- ファイルパス
*/
bool BinaryLoader::Load(const wchar_t* filename)
{
	auto fs = ifstream(filename, ios::in | ios::binary);

	return Load(fs, FileSize(fs));
}
/**
* 読み込み関数
* @param fs   -- 読み込むんだfileStream
* @param size -- ファイルサイズ
* @return bool -- 読み込めなかったらfalse
*/
bool BinaryLoader::Load(std::istream& fs,size_t size)
{
	if (!fs) return false;

	buffer.resize(size);

	fs.read(buffer.data(), buffer.size());

	return true;
}
/**
* 読み込んだdataを返す
* @return char* -- 読み込んだデータ
*/
char* BinaryLoader::Data()
{
	if (buffer.empty()) return nullptr;

	return buffer.data();
}
/**
* 読み込んだファイルのサイズを返す
* @return size_t -- ファイルサイズ
*/
size_t BinaryLoader::Size()
{
	return buffer.size();
}