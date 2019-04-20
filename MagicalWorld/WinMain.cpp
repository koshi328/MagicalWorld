/**
* File: WinMain.cpp
*
* メインプログラム
*
* Date: 2016.10.30
* Author: koshi ito
*/
#include <windows.h>
#include "GameBase\Game.h"
#include <memory>
#include <time.h>
/**
* 乱数を取得する関数
* @param min -- 最小値
* @param max -- 最大値
*/
float randomRange(float min, float max)
{
	return static_cast<float>(rand()) / RAND_MAX * (max - min) + min;
}
/**
* メイン関数
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned int)time(NULL));
	// ゲームの生成
	std::unique_ptr<Game> game = std::make_unique<Game>();
	// ゲームの実行
	game->Run();

	return 0;
}
