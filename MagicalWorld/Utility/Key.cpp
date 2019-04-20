/**
* File: key.cpp
* �ǂݍ��݃N���X
*
* Date: 2016.12.5
* Author: koshi ito
*/
#include "Key.h"
/**
* static�ϐ�
*/
Key* Key::_instance = nullptr;
/**
* Key�̃C���v�������g�N���X
*/
class Key::Impl
{
public:
	// ���̓f�[�^(2�t���[����)
	struct 
	{
		BYTE key[256];
		operator PBYTE() { return key; }
	}now, old;
};
/**
* �V���O���g��
*/
Key* Key::getInstance()
{
	if (!_instance)
		_instance = new Key();
	return _instance;
}
/**
* �R���X�g���N�^
*/
Key::Key() : _pImpl(new Impl())
{
	// ������
	ZeroMemory(&_pImpl->now, sizeof(_pImpl->now));
	ZeroMemory(&_pImpl->old, sizeof(_pImpl->old));
}
/**
* �f�X�g���N�^
*/
Key::~Key()
{
	_instance = nullptr;
}
/**
* �X�V
*/
void Key::Update()
{
	// �L�[���̍X�V
	_pImpl->old = _pImpl->now;
	GetKeyboardState(_pImpl->now);
}
/**
* �����ꂽ��
* @param k -- �Ď�������KeyID
* @return bool �����ꂽ����True
*/
bool Key::Touch(BYTE k)
{
	bool nowKey = _pImpl->now[k] & 0x80 ? true : false;
	bool oldKey = _pImpl->old[k] & 0x80 ? true : false;
	return (nowKey && !oldKey) ? true : false;
}
/**
* ������Ă��鎞
* @param k -- �Ď�������KeyID
* @return bool ������Ă��鎞��True
*/
bool Key::Press(BYTE k)
{
	return _pImpl->now[k] & 0x80 ? true : false;
}
/**
* ���ꂽ��
* @param k -- �Ď�������KeyID
* @return bool ���ꂽ����True
*/
bool Key::Release(BYTE k)
{
	bool nowKey = _pImpl->now[k] & 0x80 ? true : false;
	bool oldKey = _pImpl->old[k] & 0x80 ? true : false;
	return (!nowKey && oldKey) ? true : false;
}