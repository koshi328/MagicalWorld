/**
* File: key.h
* 読み込みクラス
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
	// シングルトン
	static Key* getInstance();
	// デストラクタ
	~Key();
	// 押された時
	bool Touch(BYTE k);
	// 押されている時
	bool Press(BYTE k);
	// 離れた時
	bool Release(BYTE k);
	// 更新
	void Update();
private:
	// コンストラクタ
	Key();
	// インプリメントクラス
	class Impl;
	std::unique_ptr<Impl> _pImpl;
	// インスタンス
	static Key* _instance;
};