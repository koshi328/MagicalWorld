/**
* File: GameBase.cpp
*
* ゲームの基礎部分の処理
*
* Date: 2016.11.30
* Author: koshi ito
*/

#include "GameBase.h"
#include "WindowBase.h"
#include "Graphics.h"
#include "..\Utility\Mouse.h"
#include "..\Utility\Key.h"
#include "..\ImaseLib\FPSTimer.h"
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

/**
* ゲームベースのインプリメントクラス
*/
class GameBase::Impl
{
public:
	std::unique_ptr<WindowBase> _pWindow;	// ウィンドウのポインタ
	std::unique_ptr<Graphics> _pGraphics;	// グラフィックのポインタ
	GameBase* _pGame;						// 派生クラスのポインタ
	/**
	* コンストラクタ
	*/
	Impl::Impl()
	{

	}
	/**
	* デストラクタ
	*/
	Impl::~Impl()
	{

	}
	/**
	* 初期化
	*/
	void Initialize()
	{
		_pWindow.reset(new WindowBase());
		_pWindow->Initialize();
		_pGraphics.reset(Graphics::getInstance());
		_pGraphics->Initialize(_pWindow->getWindowHandle());
	}
	/**
	* 更新
	*/
	void Update()
	{

	}
	/**
	* 描画
	*/
	void Render()
	{

	}
	/**
	* 終了
	*/
	void Finalize()
	{

	}
	/**
	* 実行
	*/
	void Run()
	{
		// 1秒間に60フレームで固定する
		ImaseLib::FPSTimer fpsTimer(60);
		_pGame->Initialize();
		MSG msg = { 0 };
		while (true)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) > 0)
			{
				if (msg.message == WM_QUIT)
					break;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Mouse::getInstance()->Update(_pWindow->getWindowHandle());
				Key::getInstance()->Update();
				_pGame->Update();

				wchar_t buf[256];
				swprintf_s(buf, 256, L"fps = %d", fpsTimer.GetNowFPS());

				// バックバッファのクリア
				_pGraphics->getDeviceContext()->ClearRenderTargetView(_pGraphics->getRenderTargetView(), DirectX::Colors::Black);
				// 深度バッファのクリア
				_pGraphics->getDeviceContext()->ClearDepthStencilView(_pGraphics->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
				// 描画開始
				_pGraphics->getSpriteBatch()->Begin(
					DirectX::SpriteSortMode::SpriteSortMode_Deferred,
					_pGraphics->getCommonStates()->NonPremultiplied(),
					_pGraphics->getCommonStates()->PointClamp());

				// FPSの表示
				//Graphics::getInstance()->getSpriteFont()->DrawString(Graphics::getInstance()->getSpriteBatch(), buf, Vector2(0, 0));
				_pGame->Render();

				// 描画終了
				_pGraphics->getSpriteBatch()->End();
				// 画面更新を待つ
				fpsTimer.WaitFrame();
				// バックバッファとフロントバッファを入れ替える
				_pGraphics->getSwapChain()->Present(0, 0);
			}
		}
		_pGame->Finalize();
		// デバイスの解放
		_pGraphics->Finalize();
	}
};

/**
* コンストラクタ
*/
GameBase::GameBase(GameBase* pGame) : _pImpl(new Impl())
{
	_pImpl->_pGame = pGame;
}
/**
* デストラクタ
*/
GameBase::~GameBase()
{

}
/**
* 初期化
*/
void GameBase::Initialize()
{
	_pImpl->Initialize();
}
/**
* 更新
*/
void GameBase::Update()
{
	_pImpl->Update();
}
/**
* 描画
*/
void GameBase::Render()
{
	_pImpl->Render();
}
/**
* 終了
*/
void GameBase::Finalize()
{
	_pImpl->Finalize();
}
/**
* 実行
*/
void GameBase::Run()
{
	_pImpl->Run();
}