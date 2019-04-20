/**
* File: Mouse.h
* �}�E�X�N���X
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
	// �}�E�X�̎g�p���@Type
	enum Mode
	{
		MODE_ABSOLUTE = 0,
		MODE_RELATIVE,
	};
	// �N���b�NType
	enum TYPE
	{
		LEFT,
		RIGHT,
		MIDDLE
	};
	// �V���O���g��
	static Mouse* getInstance();
	// �f�X�g���N�^
	~Mouse();
	// �X�V
	void Update(HWND hwnd);
	// �z�C�[���l�̍X�V
	void WheelValueUpdate(int value);

	// �����Ă��Ȃ���
	bool Up(TYPE t);
	// ��������
	bool Touch(TYPE t);
	// �����Ă����
	bool Press(TYPE t);
	// �b������
	bool Release(TYPE t);
	// �z�C�[���l�̎擾
	int getWheelValue();
	// �}�E�X�̃��[�h�ύX
	void setMode(Mode mode);
	// ���[�h�擾
	Mode getMode();
	// X�l�擾
	const int& X();
	// Y�l�擾
	const int& Y();
private:
	// �R���X�g���N�^
	Mouse();
	// �C���v�������g�N���X
	class Impl;
	std::unique_ptr<Impl> _pImpl;
	// �C���X�^���X
	static Mouse* _instance;
};