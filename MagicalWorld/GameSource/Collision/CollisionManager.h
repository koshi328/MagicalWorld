/**
* File: CollisionManager.h
*
* �܂Ƃ߂����Փ˔�����Ǘ�����
*
* Date: 2016.11.18
* Author: koshi ito
*/
#pragma once

class CollisionManager
{
private:
	static CollisionManager* instance_;	// �C���X�^���X
	// �R���X�g���N�^
	CollisionManager() = default;
public:
	// �V���O���g��
	static CollisionManager* getInstance()
	{
		if (!instance_)
		{
			instance_ = new CollisionManager;
		}
		return instance_;
	}
	// �f�X�g���N�^
	~CollisionManager()
	{
		instance_ = nullptr;
	}

	// �X�V
	void Update();
};