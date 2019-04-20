/**
* File: Game.cpp
*
* ゲームのメイン処理
*
* Date: 2016.11.30
* Author: koshi ito
*/

#include "Game.h"
#include <SimpleMath.h>
#include "..\GameSource\Camera\CameraManager.h"
#include "..\GameSource\Camera\Camera.h"
#include "..\GameSource\Scene\SceneManager.h"
#include "..\GameSource\Scene\TitleScene.h"
#include "..\Utility\Key.h"
#include "..\Utility\Mouse.h"
#include "..\Utility\MeshManager.h"
#include "..\Utility\TaskManager.h"
#include "..\Particle\ParticleManager.h"
#include <fstream>
// サウンド
#include "..\Sound\ADX2Le.h"
#include "..\Resources\Music\MagicalWorld_acf.h"
#include "..\Resources\Music\WorkUnit_0\CueSheet_0.h"

/**
* 名前空間
*/
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

/**
* グローバル関数
*/
/**
* メッシュマネージャーを取得
*/
MeshManager* getMeshManager()
{
	static MeshManager* instance = new MeshManager;
	return instance;
}
/**
* タスクマネージャーを取得
*/
TaskManager* getTaskManager()
{
	static TaskManager* instance = new TaskManager;
	return instance;
}
/**
* パーティクルマネージャーを取得
*/
ParticleManager* getParticleManager()
{
	static ParticleManager* instance = new ParticleManager(Graphics::getInstance()->getDeviceContext());
	return instance;
}

/**
* ゲームクラスのインプリメント
*/
class Game::Impl
{
public:
	Graphics* _pGraphics;						// グラフィック
	unique_ptr<CameraManager> _pCameraManager;	// カメラマネージャー
	unique_ptr<SceneManager> _pSceneManager;	// シーンマネージャー
public:
	/**
	* コンストラクタ
	*/
	Impl()
	{
		_pSceneManager.reset(SceneManager::getInstance());
		_pCameraManager.reset(CameraManager::getInstance());
	}
	/**
	* デストラクタ
	*/
	~Impl()
	{

	}
	/**
	* 初期化
	*/
	void Initialize()
	{
		// グラフィックデバイスの生成
		_pGraphics = Graphics::getInstance();
		// エフェクトファクトリーの生成
		unique_ptr<DGSLEffectFactory> factory(new DGSLEffectFactory(_pGraphics->getDevice()));
		factory->SetDirectory(L"Resources/cModels");
		// オーディオ設定
		ADX2Le::Initialize("Resources/Music/MagicalWorld.acf");
		ADX2Le::LoadAcb("Resources/Music/WorkUnit_0/CueSheet_0.acb", "Resources/Music/WorkUnit_0/CueSheet_0.awb");

		// メッシュマネージャーの設定
		std::string str;
		std::ifstream ifs("Resources/Data/ModelList.txt");
		getMeshManager()->Initialize(50, _pGraphics->getDeviceContext(), _pGraphics->getDevice());
		int i = 0;
		while (getline(ifs, str))
		{
			wchar_t buf[256];
			size_t ret;
			mbstowcs_s(&ret, buf, 256, str.c_str(), _TRUNCATE);
			getMeshManager()->LoadModel(i, buf, factory.get());
			++i;
		}
		ifs.close();

		// パーティクルマネージャーの設定
		getParticleManager()->Initialize();
		getParticleManager()->LoadTexture(L"Resources/Media/bit");
		getParticleManager()->SetAutoBillboard();

		// 初期シーンの生成
		_pSceneManager->setNextScene(SceneManager::SCENE_ID::TITLE);
		_pSceneManager->ChangeScene(SceneManager::CHANGE_TYPE::BKACK_FADE);
	}
	/**
	* 更新
	*/
	void Update()
	{
		// 描画モデルのリセット
		getMeshManager()->Reset();
		// オーディオ更新
		ADX2Le::Update();
		// パーティクルマネージャーの更新
		getParticleManager()->Update();
		// カメラの更新
		_pCameraManager->Update();
		// シーンの更新
		_pSceneManager->Update();
		// 終了処理
		if (Key::getInstance()->Touch(VK_ESCAPE))
		{
			Mouse::getInstance()->setMode(Mouse::MODE_ABSOLUTE);
			if (MessageBox(NULL, TEXT("終了しますか？"),
				TEXT("確認ウィンドウ"), MB_YESNO) == IDYES)
			{
				PostQuitMessage(0);	// メインループを抜ける
			}
			else
			{
				Mouse::getInstance()->setMode(Mouse::MODE_RELATIVE);
			}
		}
	}
	/**
	* 描画
	*/
	void Render()
	{
		// タスクマネージャーの描画
		getTaskManager()->Run();
		// シーンの描画
		_pSceneManager->Render();
		Camera* pCamera = _pCameraManager->getCamera();
		getMeshManager()->Render(_pGraphics->getCommonStates(), pCamera->GetViewmat(), pCamera->GetProjmat());
		// パーティクルの描画
		getParticleManager()->Draw(pCamera->GetViewmat(), pCamera->GetProjmat());
	}
	/**
	* 終了
	*/
	void Finalize()
	{
		// 解放
		ADX2Le::Finalize();
	}
};

/**
* コンストラクタ
*/
Game::Game()
	:GameBase(this), _pImpl(new Impl())
{

}
/**
* デストラクタ
*/
Game::~Game()
{
	delete getMeshManager();
	delete getParticleManager();
	delete getTaskManager();
}
/**
* 初期化
*/
void Game::Initialize()
{
	GameBase::Initialize();
	_pImpl->Initialize();
}
/**
* 更新
*/
void Game::Update()
{
	_pImpl->Update();
	GameBase::Update();
}
/**
* 描画
*/
void Game::Render()
{
	_pImpl->Render();
	GameBase::Render();
}
/**
* 終了
*/
void Game::Finalize()
{
	_pImpl->Finalize();
	GameBase::Finalize();
}