/**
* File: Mouse.h
* マウスクラス
*
* Date: 2016.12.5
* Author: koshi ito
*/

#pragma once

#include <windows.h>
#include <memory>

class Mouse
{
public:
	// マウスの使用方法Type
	enum Mode
	{
		MODE_ABSOLUTE = 0,
		MODE_RELATIVE,
	};
	// クリックType
	enum TYPE
	{
		LEFT,
		RIGHT,
		MIDDLE
	};
	// シングルトン
	static Mouse* getInstance();
	// デストラクタ
	~Mouse();
	// 更新
	void Update(HWND hwnd);
	// ホイール値の更新
	void WheelValueUpdate(int value);

	// 押していない時
	bool Up(TYPE t);
	// 押した時
	bool Touch(TYPE t);
	// 押している間
	bool Press(TYPE t);
	// 話した時
	bool Release(TYPE t);
	// ホイール値の取得
	int getWheelValue();
	// マウスのモード変更
	void setMode(Mode mode);
	// モード取得
	Mode getMode();
	// X値取得
	const int& X();
	// Y値取得
	const int& Y();
private:
	// コンストラクタ
	Mouse();
	// インプリメントクラス
	class Impl;
	std::unique_ptr<Impl> _pImpl;
	// インスタンス
	static Mouse* _instance;
};