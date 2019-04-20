/**
* File: key.cpp
* 読み込みクラス
*
* Date: 2016.12.5
* Author: koshi ito
*/
#include "Key.h"
/**
* static変数
*/
Key* Key::_instance = nullptr;
/**
* Keyのインプリメントクラス
*/
class Key::Impl
{
public:
	// 入力データ(2フレーム分)
	struct 
	{
		BYTE key[256];
		operator PBYTE() { return key; }
	}now, old;
};
/**
* シングルトン
*/
Key* Key::getInstance()
{
	if (!_instance)
		_instance = new Key();
	return _instance;
}
/**
* コンストラクタ
*/
Key::Key() : _pImpl(new Impl())
{
	// 初期化
	ZeroMemory(&_pImpl->now, sizeof(_pImpl->now));
	ZeroMemory(&_pImpl->old, sizeof(_pImpl->old));
}
/**
* デストラクタ
*/
Key::~Key()
{
	_instance = nullptr;
}
/**
* 更新
*/
void Key::Update()
{
	// キー情報の更新
	_pImpl->old = _pImpl->now;
	GetKeyboardState(_pImpl->now);
}
/**
* 押された時
* @param k -- 監視したいKeyID
* @return bool 押された時にTrue
*/
bool Key::Touch(BYTE k)
{
	bool nowKey = _pImpl->now[k] & 0x80 ? true : false;
	bool oldKey = _pImpl->old[k] & 0x80 ? true : false;
	return (nowKey && !oldKey) ? true : false;
}
/**
* 押されている時
* @param k -- 監視したいKeyID
* @return bool 押されている時にTrue
*/
bool Key::Press(BYTE k)
{
	return _pImpl->now[k] & 0x80 ? true : false;
}
/**
* 離れた時
* @param k -- 監視したいKeyID
* @return bool 離れた時にTrue
*/
bool Key::Release(BYTE k)
{
	bool nowKey = _pImpl->now[k] & 0x80 ? true : false;
	bool oldKey = _pImpl->old[k] & 0x80 ? true : false;
	return (!nowKey && oldKey) ? true : false;
}