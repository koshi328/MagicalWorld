/**
* File: RenderTarget.cpp
*
* レンダーターゲット
*
* Date: 2017.1.15
* Author: koshi ito
*/
#include "RenderTarget.h"
#include <d3d11.h>
#include <wrl.h>
#include <SimpleMath.h>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;

class RenderTarget::Impl
{
	// シャドウマップ用のレンダーターゲット
	ComPtr<ID3D11RenderTargetView> targetView;
	ComPtr<ID3D11ShaderResourceView> resourceView;
	ComPtr<ID3D11DepthStencilView> depthView;
	D3D11_VIEWPORT viewport;

	// 元のレンダーターゲット
	ID3D11RenderTargetView *oldTarget[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11DepthStencilView *oldDepth;
	UINT oldViewportNum;
	D3D11_VIEWPORT oldViewport[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];


	float width;	// 幅
	float height;	// 高さ

public:
	/**
	* コンストラクタ
	*/
	Impl()
	{
		for (auto &i : oldTarget) i = nullptr;
		oldDepth = nullptr;

		width = height = 2048;
	}

	/**
	* デストラクタ
	*/
	~Impl()
	{
		Release();
	}

	/**
	* 解放
	*/
	void Release()
	{
		for (auto &i : oldTarget)
		{
			if (i)
			{
				i->Release();
				i = nullptr;
			}
		}

		if (oldDepth)
		{
			oldDepth->Release();
			oldDepth = nullptr;
		}
	}

	/**
	* オブジェクトの作成
	*/
	void Create(ID3D11Device * device,UINT width, UINT height, DXGI_FORMAT targetFormat, DXGI_FORMAT depthFormat)
	{
		this->width = (float)width;
		this->height = (float)height;

		// レンダーターゲットテクスチャの作成
		ComPtr<ID3D11Texture2D> textuer;
		D3D11_TEXTURE2D_DESC  targetDesc = {};
		targetDesc.Width = width;
		targetDesc.Height = height;
		targetDesc.MipLevels = 1;
		targetDesc.ArraySize = 1;
		targetDesc.Format = targetFormat;
		targetDesc.SampleDesc.Count = 1;
		targetDesc.SampleDesc.Quality = 0;
		targetDesc.Usage = D3D11_USAGE_DEFAULT;
		targetDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		targetDesc.CPUAccessFlags = 0;
		targetDesc.MiscFlags = 0;
		device->CreateTexture2D(&targetDesc, nullptr, textuer.GetAddressOf());

		// レンダーターゲットの作成
		D3D11_RENDER_TARGET_VIEW_DESC rtDesc = {};
		rtDesc.Format = targetFormat;
		rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtDesc.Buffer.ElementOffset = 0;
		rtDesc.Buffer.ElementWidth = 0;
		rtDesc.Buffer.FirstElement = 0;
		rtDesc.Buffer.NumElements = 0;
		rtDesc.Texture2D.MipSlice = 0;
		device->CreateRenderTargetView(textuer.Get(), &rtDesc, targetView.ReleaseAndGetAddressOf());

		// シェーダリソースビューの作成
		D3D11_SHADER_RESOURCE_VIEW_DESC srDesc = {};
		srDesc.Format = targetFormat;
		srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srDesc.Texture2D.MostDetailedMip = 0;
		srDesc.Texture2D.MipLevels = 1;
		device->CreateShaderResourceView(textuer.Get(), &srDesc, resourceView.ReleaseAndGetAddressOf());


		if (depthFormat == DXGI_FORMAT_D16_UNORM ||
			depthFormat == DXGI_FORMAT_D24_UNORM_S8_UINT ||
			depthFormat == DXGI_FORMAT_D32_FLOAT ||
			depthFormat == DXGI_FORMAT_D32_FLOAT_S8X24_UINT)
		{
			// 深度ステンシルバッファ用テクスチャを作成
			ComPtr<ID3D11Texture2D> depth;
			D3D11_TEXTURE2D_DESC depthDesc = {};
			depthDesc.Width = width;
			depthDesc.Height = height;
			depthDesc.MipLevels = 1;
			depthDesc.ArraySize = 1;
			depthDesc.Format = depthFormat;
			depthDesc.SampleDesc.Count = 1;
			depthDesc.SampleDesc.Quality = 0;
			depthDesc.Usage = D3D11_USAGE_DEFAULT;
			depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthDesc.CPUAccessFlags = 0;
			depthDesc.MiscFlags = 0;
			device->CreateTexture2D(&depthDesc, nullptr, depth.GetAddressOf());


			// 深度ステンシルバッファのビューを作成
			D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc = {};
			dsDesc.Format = depthDesc.Format = depthFormat;
			dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			dsDesc.Flags = 0;
			dsDesc.Texture2D.MipSlice = 0;
			device->CreateDepthStencilView(depth.Get(), &dsDesc, depthView.ReleaseAndGetAddressOf());
		}
		else
		{
			depthView.Reset();
		}


		// ビューポートの設定
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
	}

	/**
	* レンダーターゲットを切り替える
	*/
	void Begin(ID3D11DeviceContext *context)
	{
		// 元のレンダーターゲットを取得
		context->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, oldTarget, &oldDepth);
		oldViewportNum = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
		context->RSGetViewports(&oldViewportNum, oldViewport);

		ID3D11ShaderResourceView *nullTextures[8] = { nullptr };
		context->PSSetShaderResources(0, 8, nullTextures);

		// シャドウマップ用のレンダーターゲットを設定する
		ID3D11RenderTargetView *targets[] = { targetView.Get() };
		context->OMSetRenderTargets(1, targets, depthView.Get());
		context->RSSetViewports(1, &viewport);
	}

	/**
	* レンダーターゲットの切り替えとクリア
	*/
	void BeginAndClear(ID3D11DeviceContext *context,Color color)
	{
		Begin(context);

		// クリア
		if ( targetView )
		{
			context->ClearRenderTargetView(targetView.Get(), color);
		}
		if (depthView)
		{
			context->ClearDepthStencilView(depthView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
	}

	/**
	* 終了
	*/
	void End(ID3D11DeviceContext *context)
	{
		// 元のレンダーターゲットを設定
		context->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, oldTarget, oldDepth);
		context->RSSetViewports(oldViewportNum, oldViewport);

		Release();
	}
	/**
	* テクスチャの取得
	*/
	ID3D11ShaderResourceView* GetResourceView()
	{
		return resourceView.Get();
	}
};
/**
* コンストラクタ
*/
RenderTarget::RenderTarget() : pImpl_(new Impl())
{
}
/**
* デストラクタ
*/
RenderTarget::~RenderTarget()
{
}

/**
* オブジェクトの生成
* @param device -- デバイス
* @param width  -- 幅
* @param height -- 高さ
* @param targetFormat -- フォーマット
* @param depthFormat  -- フォーマット
*/
void RenderTarget::Create(ID3D11Device * device, UINT width, UINT height, DXGI_FORMAT targetFormat, DXGI_FORMAT depthFormat)
{
	pImpl_->Create(device, width, height, targetFormat, depthFormat);
}
/**
* レンダーターゲットの切り替え
* @param context -- デバイスコンテキスト
*/
void RenderTarget::Begin(ID3D11DeviceContext *context)
{
	pImpl_->Begin(context);
}
/**
* レンダーターゲットの切り替えとクリア
* @param context -- デバイスコンテキスト
* @param color   -- 色
*/
void RenderTarget::BeginAndClear(ID3D11DeviceContext *context, Color color)
{
	pImpl_->BeginAndClear(context, color);
}
/**
* 終了
* @param context -- デバイスコンテキスト
*/
void RenderTarget::End(ID3D11DeviceContext *context)
{
	pImpl_->End(context);
}
/**
* テクスチャの取得
*/
ID3D11ShaderResourceView*  RenderTarget::GetResourceView()
{
	return pImpl_->GetResourceView();
}
