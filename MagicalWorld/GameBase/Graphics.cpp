/**
* File: Graphics.cpp
*
* 描画関連
*
* Date: 2016.11.30
* Author: koshi ito
*/

#include "Graphics.h"
#include <wrl.h>
#include <memory>
#include <SimpleMath.h>

/**
* 名前空間
*/
using namespace DirectX;
using namespace Microsoft::WRL;
/*
* リンクするライブラリ
*/
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

/**
* static宣言
*/
Graphics* Graphics::_instance = nullptr;

class Graphics::Impl
{
public:
	BOOL								_fullScreen;		// フルスクリーンで起動する場合はTRUE
	D3D_DRIVER_TYPE                     _driverType;		// ドライバータイプ
	D3D_FEATURE_LEVEL                   _featureLevel;		// 機能レベル
	ComPtr<ID3D11Device>                _pd3dDevice;		// DirectX11のデバイス
	ComPtr<ID3D11DeviceContext>         _pImmediateContext;	// コンテキスト
	ComPtr<IDXGISwapChain>              _pSwapChain;		// スワップチェイン
	ComPtr<ID3D11RenderTargetView>      _pRenderTargetView;	// レンダーターゲットビュー
	ComPtr<ID3D11Texture2D>             _pDepthStencil;		// 深度ステンシル
	ComPtr<ID3D11DepthStencilView>      _pDepthStencilView;
	// スプライトバッチ
	std::unique_ptr<SpriteBatch>		_pSpriteBatch;
	std::unique_ptr<SpriteFont>			_pSpriteFont;
	std::unique_ptr<CommonStates>		_pState;

	/**
	* コンストラクタ
	*/
	Impl()
	{
		_fullScreen = FALSE;
		_driverType = D3D_DRIVER_TYPE_NULL;
		_featureLevel = D3D_FEATURE_LEVEL_11_0;
	}


	/**
	* デバイスの初期化
	* @param hWnd --- ウィンドウハンドル
	*/
	HRESULT Direct3DInitialize(HWND hWnd)
	{
		HRESULT hr = S_OK;

		// クライアント領域の取得
		RECT rc;
		GetClientRect(hWnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;	// デバックレイヤーをサポートするフラグ
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		// スワップチェーンの設定
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;	// <--リフレッシュレートはDXGIの自動設定に任せる
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = !_fullScreen;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // <--フルスクリーンへの切り替えの時に表示モードを変更する

		/**
		* デバイスとスワップチェインの作成
		*/
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			_driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
			return hr;

		ComPtr<IDXGIDevice1> pDXGIDevice;
		ComPtr<IDXGIAdapter> pDXGIAdapter;
		ComPtr<IDXGIFactory> pDXGIFactory;

		// インターフェイス取得
		hr = _pd3dDevice.As(&pDXGIDevice);
		if (FAILED(hr)) {
			return hr;
		}

		// アダプター取得
		hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
		if (FAILED(hr)) {
			return hr;
		}
		// ファクトリー取得
		hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (LPVOID*)&pDXGIFactory);
		if (FAILED(hr)) {
			return hr;
		}
		// 画面モードの切り替え機能の設定
		hr = pDXGIFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES);
		if (FAILED(hr)) {
			return hr;
		}

		/**
		* バックバッファの作成
		*/
		hr = Direct3DBufferInitialize();
		if (FAILED(hr))
			return hr;

		/**
		* ビューポートの作成
		*/
		CD3D11_VIEWPORT vp(0.0f, 0.0f, (FLOAT)width, (FLOAT)height);
		_pImmediateContext->RSSetViewports(1, &vp);

		return S_OK;
	}

	/**
	* バックバッファと深度バッファの設定
	*/
	HRESULT Direct3DBufferInitialize()
	{
		HRESULT hr = S_OK;
		/**
		* バックバッファの設定
		*/
		// スワップチェーンからバックバッファを取得
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
			return hr;

		// バックバッファに指定されているテクスチャ情報を取得
		D3D11_TEXTURE2D_DESC descBackBuffer;
		pBackBuffer->GetDesc(&descBackBuffer);

		// レンダーターゲットビューを作成
		hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, _pRenderTargetView.GetAddressOf());
		pBackBuffer->Release();
		if (FAILED(hr))
			return hr;

		/**
		* 深度バッファの設定
		*/

		// 深度ステンシルテクスチャの作成
		CD3D11_TEXTURE2D_DESC descDepth(descBackBuffer);	// <--バックバッファに指定されているテクスチャ情報を元に設定する
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = _pd3dDevice->CreateTexture2D(&descDepth, nullptr, &_pDepthStencil);
		if (FAILED(hr))
			return hr;

		// 深度ステンシルビューの作成
		CD3D11_DEPTH_STENCIL_VIEW_DESC descDSV(D3D11_DSV_DIMENSION_TEXTURE2D, descDepth.Format);
		hr = _pd3dDevice->CreateDepthStencilView(_pDepthStencil.Get(), &descDSV, &_pDepthStencilView);
		if (FAILED(hr))
			return hr;

		/**
		* レンダーターゲットの設定
		*/
		_pImmediateContext->OMSetRenderTargets(1, _pRenderTargetView.GetAddressOf(), _pDepthStencilView.Get());

		return S_OK;
	}

	/**
	* スプライトバッチの初期化
	*/
	void SpriteInitialize()
	{
		// コモンステートを作成
		_pState.reset(new CommonStates(_pd3dDevice.Get()));
		// スプライトバッチ
		_pSpriteBatch.reset(new SpriteBatch(_pImmediateContext.Get()));
		// スプライトフォント
		_pSpriteFont.reset(new SpriteFont(_pd3dDevice.Get(), L"myfile.spritefont"));
	}
};


/**
* コンストラクタ
*/
Graphics::Graphics() : _pImpl(new Impl())
{

}

/**
* デストラクタ
*/
Graphics::~Graphics()
{
	_instance = nullptr;
}

/**
* インスタンスの取得
*/
Graphics* Graphics::getInstance()
{
	if (!_instance)
		_instance = new Graphics();
	return _instance;
}

/**
* 初期化処理
*/
HRESULT Graphics::Initialize(HWND hwnd)
{
	HRESULT result = _pImpl->Direct3DInitialize(hwnd);
	if (result == E_FAIL) return result;
	_pImpl->SpriteInitialize();
	return result;
}

/**
* 後始末処理
*/
void Graphics::Finalize()
{
	if (_pImpl->_pImmediateContext) _pImpl->_pImmediateContext->ClearState();
}

/**
* デバイスの取得
*/
ID3D11Device* Graphics::getDevice()
{
	return _pImpl->_pd3dDevice.Get();
}

/**
* コンテキストの取得
*/
ID3D11DeviceContext* Graphics::getDeviceContext()
{
	return _pImpl->_pImmediateContext.Get();
}

/**
* スワップチェインの取得
*/
IDXGISwapChain* Graphics::getSwapChain()
{
	return _pImpl->_pSwapChain.Get();
}

/**
* レンダ―ターゲットビューの取得
*/
ID3D11RenderTargetView* Graphics::getRenderTargetView()
{
	return _pImpl->_pRenderTargetView.Get();
}

/**
* 深度ステンシルの取得
*/
ID3D11Texture2D* Graphics::getDepthStencil()
{
	return _pImpl->_pDepthStencil.Get();
}

/**
* 深度ステンシルビューの取得
*/
ID3D11DepthStencilView* Graphics::getDepthStencilView()
{
	return _pImpl->_pDepthStencilView.Get();
}

/**
* スプライトバッチの取得
*/
SpriteBatch* Graphics::getSpriteBatch()
{
	return _pImpl->_pSpriteBatch.get();
}

/**
* スプライトフォントの取得
*/
SpriteFont* Graphics::getSpriteFont()
{
	return _pImpl->_pSpriteFont.get();
}

/**
* コモンステートの取得
*/
CommonStates* Graphics::getCommonStates()
{
	return _pImpl->_pState.get();
}