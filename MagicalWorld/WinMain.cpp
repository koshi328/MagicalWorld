/**
* File: WinMain.cpp
*
* ���C���v���O����
*
* Date: 2016.10.30
* Author: koshi ito
*/
#include <windows.h>
#include "GameBase\Game.h"
#include <memory>
#include <time.h>
/**
* �������擾����֐�
* @param min -- �ŏ��l
* @param max -- �ő�l
*/
float randomRange(float min, float max)
{
	return static_cast<float>(rand()) / RAND_MAX * (max - min) + min;
}
/**
* ���C���֐�
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned int)time(NULL));
	// �Q�[���̐���
	std::unique_ptr<Game> game = std::make_unique<Game>();
	// �Q�[���̎��s
	game->Run();

	return 0;
}
