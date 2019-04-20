/**
* File: Mouse.h
* マウスクラス
*
* Date: 2016.12.5
* Author: koshi ito
*/
#include "Mouse.h"

/**
* static変数
*/
Mouse* Mouse::_instance = nullptr;
/**
* Mouseのインプリメントクラス
*/
class Mouse::Impl
{
public:
	// 入力状態（2フレーム分）
	struct Button
	{
		bool now = false;
		bool old = false;
	};

	Button _left;			// 左ボタン
	Button _right;			// 右ボタン
	Button _middle;			// ミドルボタン
	POINT _point;			// カーソル位置
	int _scrollWheelValue;	// スクロール値
	Mode _mode;				// モード

	/**
	* コンストラクタ
	*/
	Impl()
	{
		_scrollWheelValue = 0;
		_point = { 0,0 };
		_mode = MODE_ABSOLUTE;
	}
	/**
	* デストラクタ
	*/
	~Impl()
	{

	}
	/**
	* 更新
	* @param hwnd -- ウィンドウハンドル
	*/
	void Update(HWND hwnd)
	{
		_left.old = _left.now;
		_right.old = _right.now;
		_middle.old = _middle.now;
		_left.now = GetKeyState(VK_LBUTTON) & 0x80 ? true : false;
		_right.now = GetKeyState(VK_RBUTTON) & 0x80 ? true : false;
		_middle.now = GetKeyState(VK_MBUTTON) & 0x80 ? true : false;

		if (_mode == Mode::MODE_ABSOLUTE)
		{
			GetCursorPos(&_point);
			ScreenToClient(hwnd, &_point);
			while (ShowCursor(1) < 0);
		}
		else if (_mode == Mode::MODE_RELATIVE)
		{
			GetCursorPos(&_point);
			RECT rc;
			GetWindowRect(hwnd, &rc);
			POINT origin = { (rc.right - rc.left) / 2,(rc.bottom - rc.top) / 2 };
			ClientToScreen(hwnd, &origin);
			_point.x -= origin.x;
			_point.y -= origin.y;
			SetCursorPos(origin.x, origin.y);
			while (ShowCursor(0) > 0);
		}
	}
};
/**
* シングルトン
*/
Mouse* Mouse::getInstance()
{
	if (!_instance)
		_instance = new Mouse;
	return _instance;
}
/**
* コンストラクタ
*/
Mouse::Mouse() : _pImpl(new Impl())
{
	
}

/**
* デストラクタ
*/
Mouse::~Mouse()
{
	_instance = nullptr;
}
/**
* 更新
* @param hwnd -- ウィンドウハンドル
*/
void Mouse::Update(HWND hwnd)
{
	_pImpl->Update(hwnd);
}
/**
* ホイール値の更新
* @param value -- 増えたホイール値
*/
void Mouse::WheelValueUpdate(int value)
{
	_pImpl->_scrollWheelValue += value;
}
/**
* ホイール値の取得
*/
int Mouse::getWheelValue()
{
	return _pImpl->_scrollWheelValue;
}
/**
* 押していない間
* @param t     -- 監視したいボタンのTYPE
* @return bool -- 押していない間はtrue
*/
bool Mouse::Up(TYPE t)
{
	bool result = false;
	switch (t)
	{
	case LEFT: result = !_pImpl->_left.old && !_pImpl->_left.now ? true : false; break;
	case RIGHT: result = !_pImpl->_right.old && !_pImpl->_right.now ? true : false; break;
	case MIDDLE: result = !_pImpl->_middle.old && !_pImpl->_middle.now ? true : false; break;
	}
	return result;
}
/**
* 押した時
* @param t     -- 監視したいボタンのTYPE
* @return bool -- 押した時はtrue
*/
bool Mouse::Touch(TYPE t)
{
	bool result = false;
	switch (t)
	{
	case LEFT: result = !_pImpl->_left.old && _pImpl->_left.now ? true : false; break;
	case RIGHT: result = !_pImpl->_right.old && _pImpl->_right.now ? true : false; break;
	case MIDDLE: result = !_pImpl->_middle.old && _pImpl->_middle.now ? true : false; break;
	}
	return result;
}
/**
* 押している間
* @param t     -- 監視したいボタンのTYPE
* @return bool -- 押している間はtrue
*/
bool Mouse::Press(TYPE t)
{
	bool result = false;
	switch (t)
	{
	case LEFT: result = _pImpl->_left.old && _pImpl->_left.now ? true : false; break;
	case RIGHT: result = _pImpl->_right.old && _pImpl->_right.now ? true : false; break;
	case MIDDLE: result = _pImpl->_middle.old && _pImpl->_middle.now ? true : false; break;
	}
	return result;
}
/**
* 離した時
* @param t     -- 監視したいボタンのTYPE
* @return bool -- 離した時はtrue
*/
bool Mouse::Release(TYPE t)
{
	bool result = false;
	switch (t)
	{
	case LEFT: result = _pImpl->_left.old && !_pImpl->_left.now ? true : false; break;
	case RIGHT: result = _pImpl->_right.old && !_pImpl->_right.now ? true : false; break;
	case MIDDLE: result = _pImpl->_middle.old && !_pImpl->_middle.now ? true : false; break;
	}
	return result;
}
/**
* モードの設定
* @param mode -- 使いたいMode
*/
void Mouse::setMode(Mode mode)
{
	_pImpl->_mode = mode;
}
/**
* モードの取得
*/
Mouse::Mode Mouse::getMode()
{
	return _pImpl->_mode;
}
/**
* X値の取得
*/
const int& Mouse::X()
{
	return _pImpl->_point.x;
}
/**
* Y値の取得
*/
const int& Mouse::Y()
{
	return _pImpl->_point.y;
}