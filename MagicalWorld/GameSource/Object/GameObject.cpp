/**
* File: GameObject.cpp
*
* �Q�[���ɓo�ꂷ��I�u�W�F�N�g�̊��N���X
*
* Date: 2016.10.15
* Author: koshi ito
*/
#include "GameObject.h"
/**
* ���O���
*/
using namespace DirectX::SimpleMath;

/**
* �R���X�g���N�^
*/
GameObject::GameObject()
:isDestroy(false), scale_(Vector3(1, 1, 1))
{
}
/**
* �f�X�g���N�^
*/
GameObject::~GameObject()
{
}
