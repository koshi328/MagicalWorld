/**
* File: GameBase.cpp
*
* �Q�[���̊�b�����̏���
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
* �Q�[���x�[�X�̃C���v�������g�N���X
*/
class GameBase::Impl
{
public:
	std::unique_ptr<WindowBase> _pWindow;	// �E�B���h�E�̃|�C���^
	std::unique_ptr<Graphics> _pGraphics;	// �O���t�B�b�N�̃|�C���^
	GameBase* _pGame;						// �h���N���X�̃|�C���^
	/**
	* �R���X�g���N�^
	*/
	Impl::Impl()
	{

	}
	/**
	* �f�X�g���N�^
	*/
	Impl::~Impl()
	{

	}
	/**
	* ������
	*/
	void Initialize()
	{
		_pWindow.reset(new WindowBase());
		_pWindow->Initialize();
		_pGraphics.reset(Graphics::getInstance());
		_pGraphics->Initialize(_pWindow->getWindowHandle());
	}
	/**
	* �X�V
	*/
	void Update()
	{

	}
	/**
	* �`��
	*/
	void Render()
	{

	}
	/**
	* �I��
	*/
	void Finalize()
	{

	}
	/**
	* ���s
	*/
	void Run()
	{
		// 1�b�Ԃ�60�t���[���ŌŒ肷��
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

				// �o�b�N�o�b�t�@�̃N���A
				_pGraphics->getDeviceContext()->ClearRenderTargetView(_pGraphics->getRenderTargetView(), DirectX::Colors::Black);
				// �[�x�o�b�t�@�̃N���A
				_pGraphics->getDeviceContext()->ClearDepthStencilView(_pGraphics->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
				// �`��J�n
				_pGraphics->getSpriteBatch()->Begin(
					DirectX::SpriteSortMode::SpriteSortMode_Deferred,
					_pGraphics->getCommonStates()->NonPremultiplied(),
					_pGraphics->getCommonStates()->PointClamp());

				// FPS�̕\��
				//Graphics::getInstance()->getSpriteFont()->DrawString(Graphics::getInstance()->getSpriteBatch(), buf, Vector2(0, 0));
				_pGame->Render();

				// �`��I��
				_pGraphics->getSpriteBatch()->End();
				// ��ʍX�V��҂�
				fpsTimer.WaitFrame();
				// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���
				_pGraphics->getSwapChain()->Present(0, 0);
			}
		}
		_pGame->Finalize();
		// �f�o�C�X�̉��
		_pGraphics->Finalize();
	}
};

/**
* �R���X�g���N�^
*/
GameBase::GameBase(GameBase* pGame) : _pImpl(new Impl())
{
	_pImpl->_pGame = pGame;
}
/**
* �f�X�g���N�^
*/
GameBase::~GameBase()
{

}
/**
* ������
*/
void GameBase::Initialize()
{
	_pImpl->Initialize();
}
/**
* �X�V
*/
void GameBase::Update()
{
	_pImpl->Update();
}
/**
* �`��
*/
void GameBase::Render()
{
	_pImpl->Render();
}
/**
* �I��
*/
void GameBase::Finalize()
{
	_pImpl->Finalize();
}
/**
* ���s
*/
void GameBase::Run()
{
	_pImpl->Run();
}