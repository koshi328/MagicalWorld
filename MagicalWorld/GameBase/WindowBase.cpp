/**
* File: WindowBase.cpp
*
* ウィンドウ処理
*
* Date: 2016.11.30
* Author: koshi ito
*/

#include <windows.h>
#include "WindowBase.h"
#include <string>
#include "..\Utility\Mouse.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

/**
* ウィンドウのインプリメントクラス
*/
class WindowBase::Impl
{
public:
	/**
	* コンストラクタ
	*/
	Impl()
	{
		_szWindowClass = L"Class";
		_width = 1280;
		_height = 720;
		WINDOW_STYLE = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	}


	/**
	* ウィンドウ登録
	*/
	LRESULT Register()
	{
		// ウインドウクラスを登録する
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandle(nullptr);
		wcex.hIcon = LoadIcon(wcex.hInstance, (LPCTSTR)NULL);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = nullptr;
		wcex.lpszClassName = _szWindowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)NULL);
		if (!RegisterClassEx(&wcex))
			return E_FAIL;
		return S_OK;
	}

	/**
	* 初期化
	*/
	HRESULT Initialize()
	{
		if (_hWnd) return S_OK;

		// ウインドウクラスの登録
		if (FAILED(Register())) return E_FAIL;
		// ウインドウを作成する
		HINSTANCE hInstance = GetModuleHandle(nullptr);
		RECT rc = { 0, 0, _width, _height };	// ←ウインドウサイズ
		AdjustWindowRect(&rc, WINDOW_STYLE, FALSE);
		_hWnd = CreateWindow(_szWindowClass, L"3Dプログラミング基礎", WINDOW_STYLE,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
			nullptr);
		if (!_hWnd)
			return E_FAIL;

		// ウインドウの表示
		ShowWindow(_hWnd, SW_SHOW);

		return S_OK;
	}
public:
	DWORD WINDOW_STYLE;		// ウィンドウスタイル
	HINSTANCE _hInst;		// インスタンスハンドル
	LPCUWSTR _szWindowClass;// ウィンドウ識別子
	HWND _hWnd;				// ウインドウハンドル
	int _width;				// 幅
	int _height;			// 高さ
};

/**
* ウィンドウプロシージャ
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:		// ウインドウが破棄されたメッセージ
		PostQuitMessage(0);	// WM_QUITメッセージを送出する（メインループを抜ける）
		break;

	case WM_ACTIVATEAPP:	// ウインドウがアクティブ化、非アクティブ化する時に来るメッセージ
	case WM_KEYDOWN:		// キー入力関係のメッセージ
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	case WM_INPUT:			// マウス関係のメッセージ
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
		Mouse::getInstance()->WheelValueUpdate(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
		break;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	default:
		// 自分で処理しないメッセージはDefWindowProc関数に任せる
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/**
* コンストラクタ
*/
WindowBase::WindowBase() :_pImpl(new Impl())
{

}
/**
* デストラクタ
*/
WindowBase::~WindowBase()
{
	BOOL bResult;
	// ウィンドウを破棄する
	bResult = ::DestroyWindow(_pImpl->_hWnd);
	// ウィンドウクラスを破棄する
	bResult = ::UnregisterClass(_pImpl->_szWindowClass, _pImpl->_hInst);
}
/**
* 初期化
*/
HRESULT WindowBase::Initialize()
{
	return _pImpl->Initialize();
}
/**
* ウィンドウハンドルの取得
*/
HWND WindowBase::getWindowHandle()
{
	return _pImpl->_hWnd;
}
/**
* ウィンドウ幅の取得
*/
const int& WindowBase::getWidth()
{
	return _pImpl->_width;
}
/**
* ウィンドウ高さの取得
*/
const int& WindowBase::getHeight()
{
	return _pImpl->_height;
}