/**
* File: BinaryLoader.h
*
* 読み込みクラス
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
	// コンストラクタ
	BinaryLoader();
	// デストラクタ
	virtual ~BinaryLoader();
	// データ
	std::vector<char> buffer;

	// データの読み込み
	bool Load(const char* filename);
	bool Load(const wchar_t* filename);
	bool Load(std::istream& stream,size_t size);

	// データの取得
	char* Data();

	// データのサイズ
	size_t Size();
};