/**
* File: Scene.h
*
* �V�[���̊��N���X
*
* Date: 2016.11.1
* Author: koshi ito
*/
#pragma once

class Scene
{
public:
	// �R���X�g���N�^
	Scene() = default;
	// �f�X�g���N�^
	virtual ~Scene() = default;
	// ������
	virtual void Initialize() = 0;
	// �X�V
	virtual void Update() = 0;
	// �`��
	virtual void Render() = 0;
};