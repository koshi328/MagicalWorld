/**
* File: SceneManager.cpp
*
* すべてのシーンを管理するクラス
*
* Date: 2016.11.1
* Author: koshi ito
*/
#include "SceneManager.h"
#include <vector>
#include "TitleScene.h"
#include "GameScene.h"
#include "BlackFade.h"
/**
* static変数
*/
SceneManager* SceneManager::instance_ = nullptr;

/**
* SceneManagerのインプリメントクラス
*/
class SceneManager::Impl
{
public:
	Scene* mainScene_;	// 実行中シーン
	Scene* effectScene_;// 切り替え用シーン
	Scene* nextScene_;	// 次のシーン
public:
	/**
	* コンストラクタ
	*/
	Impl()
	{

	}
	/**
	* デストラクタ
	*/
	~Impl()
	{
		// シーンの削除
		delete SceneFactory(TITLE);
		delete SceneFactory(PLAY);

		// エフェクトの削除
		delete ChangeEffectFactory(BKACK_FADE);
	}
	/**
	* 次のシーンを設定する
	* @param id -- 次のシーンのID
	*/
	void setNextScene(SCENE_ID id)
	{
		nextScene_ = SceneFactory(id);
	}
	/**
	* シーンを切り替える
	* @param type -- 切り替え中の演出をIDで指定
	*/
	void ChangeScene(CHANGE_TYPE type)
	{
		if (!nextScene_) return;
		// 演出を指定しなければすぐに切り替える
		if (type == NONE)
		{
			mainScene_ = nextScene_;
			mainScene_->Initialize();
			nextScene_ = nullptr;
		}
		// 演出を使う場合エフェクトを生成する
		// シーンの切り替えは演出用シーンで行う
		else
		{
			effectScene_ = ChangeEffectFactory(type);
			effectScene_->Initialize();
		}
	}
	/**
	* 更新
	*/
	void Update()
	{
		if (mainScene_)
			mainScene_->Update();
		if (effectScene_)
			effectScene_->Update();
	}
	/**
	* 描画
	*/
	void Render()
	{
		if (mainScene_)
			mainScene_->Render();
		if (effectScene_)
			effectScene_->Render();
	}
	/**
	* シーンの生成
	* @param  id     -- シーンID
	* @return Scene* -- 生成されたシーン
	*/
	Scene* SceneFactory(SCENE_ID id)
	{
		static TitleScene* title = new TitleScene();
		static GameScene* game = new GameScene();

		Scene* scene = nullptr;
		switch (id)
		{
		case TITLE:
			scene = title;
			break;
		case PLAY:
			scene = game;
			break;
		}

		return scene;
	}
	/**
	* エフェクトの生成
	* @param  type   -- 切り替え演出ID
	* @return Scene* -- 生成されたシーン
	*/
	Scene* ChangeEffectFactory(CHANGE_TYPE type)
	{
		static BlackFade* blackFade = new BlackFade();
		Scene* fade = nullptr;
		switch (type)
		{
		case BKACK_FADE:
			fade = blackFade;
			break;
		}

		return fade;
	}
	/**
	* 切り替え演出が有効かを返す
	* @return bool -- 切り替え演出中ならTrue
	*/
	bool getEffectActive()
	{
		return effectScene_ ? true : false;
	}
};

/**
* シングルトン
*/
SceneManager* SceneManager::getInstance()
{
	if (!instance_)
		instance_ = new SceneManager;
	return instance_;
}
/**
* コンストラクタ
*/
SceneManager::SceneManager() : pImpl_(new Impl())
{

}
/**
* デストラクタ
*/
SceneManager::~SceneManager()
{
	instance_ = nullptr;
}
/**
* 更新
*/
void SceneManager::Update()
{
	pImpl_->Update();
}
/**
* 描画
*/
void SceneManager::Render()
{
	pImpl_->Render();
}
/**
* シーンの切り替え
* @param type -- 切り替え演出ID
*/
void SceneManager::ChangeScene(CHANGE_TYPE type)
{
	pImpl_->ChangeScene(type);
}
/**
* 次のシーンを設定
* @param id -- 次のシーンのID
*/
void SceneManager::setNextScene(SCENE_ID id)
{
	pImpl_->setNextScene(id);
}
/**
* 切り替え演出を破棄する
*/
void SceneManager::DisposeEffect()
{
	pImpl_->effectScene_ = nullptr;
}
/**
* 切り替え演出が有効か調べる
* @return bool -- 有効ならTrue
*/
bool SceneManager::getEffectActive()
{
	return pImpl_->getEffectActive();
}