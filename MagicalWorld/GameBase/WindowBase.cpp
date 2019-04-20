/**
* File: WindowBase.cpp
*
* �E�B���h�E����
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
* �E�B���h�E�̃C���v�������g�N���X
*/
class WindowBase::Impl
{
public:
	/**
	* �R���X�g���N�^
	*/
	Impl()
	{
		_szWindowClass = L"Class";
		_width = 1280;
		_height = 720;
		WINDOW_STYLE = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	}


	/**
	* �E�B���h�E�o�^
	*/
	LRESULT Register()
	{
		// �E�C���h�E�N���X��o�^����
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
	* ������
	*/
	HRESULT Initialize()
	{
		if (_hWnd) return S_OK;

		// �E�C���h�E�N���X�̓o�^
		if (FAILED(Register())) return E_FAIL;
		// �E�C���h�E���쐬����
		HINSTANCE hInstance = GetModuleHandle(nullptr);
		RECT rc = { 0, 0, _width, _height };	// ���E�C���h�E�T�C�Y
		AdjustWindowRect(&rc, WINDOW_STYLE, FALSE);
		_hWnd = CreateWindow(_szWindowClass, L"3D�v���O���~���O��b", WINDOW_STYLE,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
			nullptr);
		if (!_hWnd)
			return E_FAIL;

		// �E�C���h�E�̕\��
		ShowWindow(_hWnd, SW_SHOW);

		return S_OK;
	}
public:
	DWORD WINDOW_STYLE;		// �E�B���h�E�X�^�C��
	HINSTANCE _hInst;		// �C���X�^���X�n���h��
	LPCUWSTR _szWindowClass;// �E�B���h�E���ʎq
	HWND _hWnd;				// �E�C���h�E�n���h��
	int _width;				// ��
	int _height;			// ����
};

/**
* �E�B���h�E�v���V�[�W��
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:		// �E�C���h�E���j�����ꂽ���b�Z�[�W
		PostQuitMessage(0);	// WM_QUIT���b�Z�[�W�𑗏o����i���C�����[�v�𔲂���j
		break;

	case WM_ACTIVATEAPP:	// �E�C���h�E���A�N�e�B�u���A��A�N�e�B�u�����鎞�ɗ��郁�b�Z�[�W
	case WM_KEYDOWN:		// �L�[���͊֌W�̃��b�Z�[�W
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	case WM_INPUT:			// �}�E�X�֌W�̃��b�Z�[�W
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
		// �����ŏ������Ȃ����b�Z�[�W��DefWindowProc�֐��ɔC����
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/**
* �R���X�g���N�^
*/
WindowBase::WindowBase() :_pImpl(new Impl())
{

}
/**
* �f�X�g���N�^
*/
WindowBase::~WindowBase()
{
	BOOL bResult;
	// �E�B���h�E��j������
	bResult = ::DestroyWindow(_pImpl->_hWnd);
	// �E�B���h�E�N���X��j������
	bResult = ::UnregisterClass(_pImpl->_szWindowClass, _pImpl->_hInst);
}
/**
* ������
*/
HRESULT WindowBase::Initialize()
{
	return _pImpl->Initialize();
}
/**
* �E�B���h�E�n���h���̎擾
*/
HWND WindowBase::getWindowHandle()
{
	return _pImpl->_hWnd;
}
/**
* �E�B���h�E���̎擾
*/
const int& WindowBase::getWidth()
{
	return _pImpl->_width;
}
/**
* �E�B���h�E�����̎擾
*/
const int& WindowBase::getHeight()
{
	return _pImpl->_height;
}