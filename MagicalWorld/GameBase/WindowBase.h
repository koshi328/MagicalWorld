/**
* File: WindowBase.cpp
*
* ウィンドウ処理
*
* Date: 2016.11.30
* Author: koshi ito
*/
#pragma once

#include <windows.h>
#include <memory>

class WindowBase
{
public:
	// コンストラクタ
	WindowBase();
	// デストラクタ
	~WindowBase();
	// 初期化
	HRESULT Initialize();
	// ハンドルの取得
	HWND getWindowHandle();
	// 幅の取得
	const int& getWidth();
	// 高さの取得
	const int& getHeight();
private:
	class Impl;
	std::unique_ptr<Impl> _pImpl;
};