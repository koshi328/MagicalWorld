/**
* File: Mouse.h
* �}�E�X�N���X
*
* Date: 2016.12.5
* Author: koshi ito
*/
#include "Mouse.h"

/**
* static�ϐ�
*/
Mouse* Mouse::_instance = nullptr;
/**
* Mouse�̃C���v�������g�N���X
*/
class Mouse::Impl
{
public:
	// ���͏�ԁi2�t���[�����j
	struct Button
	{
		bool now = false;
		bool old = false;
	};

	Button _left;			// ���{�^��
	Button _right;			// �E�{�^��
	Button _middle;			// �~�h���{�^��
	POINT _point;			// �J�[�\���ʒu
	int _scrollWheelValue;	// �X�N���[���l
	Mode _mode;				// ���[�h

	/**
	* �R���X�g���N�^
	*/
	Impl()
	{
		_scrollWheelValue = 0;
		_point = { 0,0 };
		_mode = MODE_ABSOLUTE;
	}
	/**
	* �f�X�g���N�^
	*/
	~Impl()
	{

	}
	/**
	* �X�V
	* @param hwnd -- �E�B���h�E�n���h��
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
* �V���O���g��
*/
Mouse* Mouse::getInstance()
{
	if (!_instance)
		_instance = new Mouse;
	return _instance;
}
/**
* �R���X�g���N�^
*/
Mouse::Mouse() : _pImpl(new Impl())
{
	
}

/**
* �f�X�g���N�^
*/
Mouse::~Mouse()
{
	_instance = nullptr;
}
/**
* �X�V
* @param hwnd -- �E�B���h�E�n���h��
*/
void Mouse::Update(HWND hwnd)
{
	_pImpl->Update(hwnd);
}
/**
* �z�C�[���l�̍X�V
* @param value -- �������z�C�[���l
*/
void Mouse::WheelValueUpdate(int value)
{
	_pImpl->_scrollWheelValue += value;
}
/**
* �z�C�[���l�̎擾
*/
int Mouse::getWheelValue()
{
	return _pImpl->_scrollWheelValue;
}
/**
* �����Ă��Ȃ���
* @param t     -- �Ď��������{�^����TYPE
* @return bool -- �����Ă��Ȃ��Ԃ�true
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
* ��������
* @param t     -- �Ď��������{�^����TYPE
* @return bool -- ����������true
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
* �����Ă����
* @param t     -- �Ď��������{�^����TYPE
* @return bool -- �����Ă���Ԃ�true
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
* ��������
* @param t     -- �Ď��������{�^����TYPE
* @return bool -- ����������true
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
* ���[�h�̐ݒ�
* @param mode -- �g������Mode
*/
void Mouse::setMode(Mode mode)
{
	_pImpl->_mode = mode;
}
/**
* ���[�h�̎擾
*/
Mouse::Mode Mouse::getMode()
{
	return _pImpl->_mode;
}
/**
* X�l�̎擾
*/
const int& Mouse::X()
{
	return _pImpl->_point.x;
}
/**
* Y�l�̎擾
*/
const int& Mouse::Y()
{
	return _pImpl->_point.y;
}