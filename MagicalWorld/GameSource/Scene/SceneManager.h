/**
* File: SceneManager.h
*
* すべてのシーンを管理するクラス
*
* Date: 2016.11.1
* Author: koshi ito
*/
#pragma once

#include "Scene.h"
#include <memory>

class SceneManager
{
public:
	// シーンの種類
	enum SCENE_ID
	{
		TITLE,
		PLAY,
	};
	// 切り替え演出の種類
	enum CHANGE_TYPE
	{
		NONE,
		BKACK_FADE,
	};
public:
	// シングルトン
	static SceneManager* getInstance();
	// デストラクタ
	~SceneManager();
	// 次のシーンを決める
	void setNextScene(SCENE_ID id);
	// シーンを切り替える
	void ChangeScene(CHANGE_TYPE type = NONE);
	// 切り替え演出を破棄する
	void DisposeEffect();
	// 切り替え演出を有効にする
	bool getEffectActive();
	// 更新
	void Update();
	// 描画
	void Render();
private:
	// コンストラクタ
	SceneManager();
	// インプリメントクラス
	class Impl;
	std::unique_ptr<Impl> pImpl_;
	// インスタンス
	static SceneManager* instance_;
};