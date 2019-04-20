/**
* File: Graphics.cpp
*
* �`��֘A
*
* Date: 2016.11.30
* Author: koshi ito
*/

#include "Graphics.h"
#include <wrl.h>
#include <memory>
#include <SimpleMath.h>

/**
* ���O���
*/
using namespace DirectX;
using namespace Microsoft::WRL;
/*
* �����N���郉�C�u����
*/
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

/**
* static�錾
*/
Graphics* Graphics::_instance = nullptr;

class Graphics::Impl
{
public:
	BOOL								_fullScreen;		// �t���X�N���[���ŋN������ꍇ��TRUE
	D3D_DRIVER_TYPE                     _driverType;		// �h���C�o�[�^�C�v
	D3D_FEATURE_LEVEL                   _featureLevel;		// �@�\���x��
	ComPtr<ID3D11Device>                _pd3dDevice;		// DirectX11�̃f�o�C�X
	ComPtr<ID3D11DeviceContext>         _pImmediateContext;	// �R���e�L�X�g
	ComPtr<IDXGISwapChain>              _pSwapChain;		// �X���b�v�`�F�C��
	ComPtr<ID3D11RenderTargetView>      _pRenderTargetView;	// �����_�[�^�[�Q�b�g�r���[
	ComPtr<ID3D11Texture2D>             _pDepthStencil;		// �[�x�X�e���V��
	ComPtr<ID3D11DepthStencilView>      _pDepthStencilView;
	// �X�v���C�g�o�b�`
	std::unique_ptr<SpriteBatch>		_pSpriteBatch;
	std::unique_ptr<SpriteFont>			_pSpriteFont;
	std::unique_ptr<CommonStates>		_pState;

	/**
	* �R���X�g���N�^
	*/
	Impl()
	{
		_fullScreen = FALSE;
		_driverType = D3D_DRIVER_TYPE_NULL;
		_featureLevel = D3D_FEATURE_LEVEL_11_0;
	}


	/**
	* �f�o�C�X�̏�����
	* @param hWnd --- �E�B���h�E�n���h��
	*/
	HRESULT Direct3DInitialize(HWND hWnd)
	{
		HRESULT hr = S_OK;

		// �N���C�A���g�̈�̎擾
		RECT rc;
		GetClientRect(hWnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;	// �f�o�b�N���C���[���T�|�[�g����t���O
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

		// �X���b�v�`�F�[���̐ݒ�
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;	// <--���t���b�V�����[�g��DXGI�̎����ݒ�ɔC����
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = !_fullScreen;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // <--�t���X�N���[���ւ̐؂�ւ��̎��ɕ\�����[�h��ύX����

		/**
		* �f�o�C�X�ƃX���b�v�`�F�C���̍쐬
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

		// �C���^�[�t�F�C�X�擾
		hr = _pd3dDevice.As(&pDXGIDevice);
		if (FAILED(hr)) {
			return hr;
		}

		// �A�_�v�^�[�擾
		hr = pDXGIDevice->GetAdapter(&pDXGIAdapter);
		if (FAILED(hr)) {
			return hr;
		}
		// �t�@�N�g���[�擾
		hr = pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (LPVOID*)&pDXGIFactory);
		if (FAILED(hr)) {
			return hr;
		}
		// ��ʃ��[�h�̐؂�ւ��@�\�̐ݒ�
		hr = pDXGIFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_WINDOW_CHANGES);
		if (FAILED(hr)) {
			return hr;
		}

		/**
		* �o�b�N�o�b�t�@�̍쐬
		*/
		hr = Direct3DBufferInitialize();
		if (FAILED(hr))
			return hr;

		/**
		* �r���[�|�[�g�̍쐬
		*/
		CD3D11_VIEWPORT vp(0.0f, 0.0f, (FLOAT)width, (FLOAT)height);
		_pImmediateContext->RSSetViewports(1, &vp);

		return S_OK;
	}

	/**
	* �o�b�N�o�b�t�@�Ɛ[�x�o�b�t�@�̐ݒ�
	*/
	HRESULT Direct3DBufferInitialize()
	{
		HRESULT hr = S_OK;
		/**
		* �o�b�N�o�b�t�@�̐ݒ�
		*/
		// �X���b�v�`�F�[������o�b�N�o�b�t�@���擾
		ID3D11Texture2D* pBackBuffer = nullptr;
		hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
			return hr;

		// �o�b�N�o�b�t�@�Ɏw�肳��Ă���e�N�X�`�������擾
		D3D11_TEXTURE2D_DESC descBackBuffer;
		pBackBuffer->GetDesc(&descBackBuffer);

		// �����_�[�^�[�Q�b�g�r���[���쐬
		hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, _pRenderTargetView.GetAddressOf());
		pBackBuffer->Release();
		if (FAILED(hr))
			return hr;

		/**
		* �[�x�o�b�t�@�̐ݒ�
		*/

		// �[�x�X�e���V���e�N�X�`���̍쐬
		CD3D11_TEXTURE2D_DESC descDepth(descBackBuffer);	// <--�o�b�N�o�b�t�@�Ɏw�肳��Ă���e�N�X�`���������ɐݒ肷��
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

		// �[�x�X�e���V���r���[�̍쐬
		CD3D11_DEPTH_STENCIL_VIEW_DESC descDSV(D3D11_DSV_DIMENSION_TEXTURE2D, descDepth.Format);
		hr = _pd3dDevice->CreateDepthStencilView(_pDepthStencil.Get(), &descDSV, &_pDepthStencilView);
		if (FAILED(hr))
			return hr;

		/**
		* �����_�[�^�[�Q�b�g�̐ݒ�
		*/
		_pImmediateContext->OMSetRenderTargets(1, _pRenderTargetView.GetAddressOf(), _pDepthStencilView.Get());

		return S_OK;
	}

	/**
	* �X�v���C�g�o�b�`�̏�����
	*/
	void SpriteInitialize()
	{
		// �R�����X�e�[�g���쐬
		_pState.reset(new CommonStates(_pd3dDevice.Get()));
		// �X�v���C�g�o�b�`
		_pSpriteBatch.reset(new SpriteBatch(_pImmediateContext.Get()));
		// �X�v���C�g�t�H���g
		_pSpriteFont.reset(new SpriteFont(_pd3dDevice.Get(), L"myfile.spritefont"));
	}
};


/**
* �R���X�g���N�^
*/
Graphics::Graphics() : _pImpl(new Impl())
{

}

/**
* �f�X�g���N�^
*/
Graphics::~Graphics()
{
	_instance = nullptr;
}

/**
* �C���X�^���X�̎擾
*/
Graphics* Graphics::getInstance()
{
	if (!_instance)
		_instance = new Graphics();
	return _instance;
}

/**
* ����������
*/
HRESULT Graphics::Initialize(HWND hwnd)
{
	HRESULT result = _pImpl->Direct3DInitialize(hwnd);
	if (result == E_FAIL) return result;
	_pImpl->SpriteInitialize();
	return result;
}

/**
* ��n������
*/
void Graphics::Finalize()
{
	if (_pImpl->_pImmediateContext) _pImpl->_pImmediateContext->ClearState();
}

/**
* �f�o�C�X�̎擾
*/
ID3D11Device* Graphics::getDevice()
{
	return _pImpl->_pd3dDevice.Get();
}

/**
* �R���e�L�X�g�̎擾
*/
ID3D11DeviceContext* Graphics::getDeviceContext()
{
	return _pImpl->_pImmediateContext.Get();
}

/**
* �X���b�v�`�F�C���̎擾
*/
IDXGISwapChain* Graphics::getSwapChain()
{
	return _pImpl->_pSwapChain.Get();
}

/**
* �����_�\�^�[�Q�b�g�r���[�̎擾
*/
ID3D11RenderTargetView* Graphics::getRenderTargetView()
{
	return _pImpl->_pRenderTargetView.Get();
}

/**
* �[�x�X�e���V���̎擾
*/
ID3D11Texture2D* Graphics::getDepthStencil()
{
	return _pImpl->_pDepthStencil.Get();
}

/**
* �[�x�X�e���V���r���[�̎擾
*/
ID3D11DepthStencilView* Graphics::getDepthStencilView()
{
	return _pImpl->_pDepthStencilView.Get();
}

/**
* �X�v���C�g�o�b�`�̎擾
*/
SpriteBatch* Graphics::getSpriteBatch()
{
	return _pImpl->_pSpriteBatch.get();
}

/**
* �X�v���C�g�t�H���g�̎擾
*/
SpriteFont* Graphics::getSpriteFont()
{
	return _pImpl->_pSpriteFont.get();
}

/**
* �R�����X�e�[�g�̎擾
*/
CommonStates* Graphics::getCommonStates()
{
	return _pImpl->_pState.get();
}