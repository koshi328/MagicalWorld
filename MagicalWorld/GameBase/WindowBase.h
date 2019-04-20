/**
* File: WindowBase.cpp
*
* �E�B���h�E����
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
	// �R���X�g���N�^
	WindowBase();
	// �f�X�g���N�^
	~WindowBase();
	// ������
	HRESULT Initialize();
	// �n���h���̎擾
	HWND getWindowHandle();
	// ���̎擾
	const int& getWidth();
	// �����̎擾
	const int& getHeight();
private:
	class Impl;
	std::unique_ptr<Impl> _pImpl;
};