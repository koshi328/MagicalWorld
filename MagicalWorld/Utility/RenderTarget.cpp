/**
* File: RenderTarget.cpp
*
* �����_�[�^�[�Q�b�g
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
	// �V���h�E�}�b�v�p�̃����_�[�^�[�Q�b�g
	ComPtr<ID3D11RenderTargetView> targetView;
	ComPtr<ID3D11ShaderResourceView> resourceView;
	ComPtr<ID3D11DepthStencilView> depthView;
	D3D11_VIEWPORT viewport;

	// ���̃����_�[�^�[�Q�b�g
	ID3D11RenderTargetView *oldTarget[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
	ID3D11DepthStencilView *oldDepth;
	UINT oldViewportNum;
	D3D11_VIEWPORT oldViewport[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];


	float width;	// ��
	float height;	// ����

public:
	/**
	* �R���X�g���N�^
	*/
	Impl()
	{
		for (auto &i : oldTarget) i = nullptr;
		oldDepth = nullptr;

		width = height = 2048;
	}

	/**
	* �f�X�g���N�^
	*/
	~Impl()
	{
		Release();
	}

	/**
	* ���
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
	* �I�u�W�F�N�g�̍쐬
	*/
	void Create(ID3D11Device * device,UINT width, UINT height, DXGI_FORMAT targetFormat, DXGI_FORMAT depthFormat)
	{
		this->width = (float)width;
		this->height = (float)height;

		// �����_�[�^�[�Q�b�g�e�N�X�`���̍쐬
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

		// �����_�[�^�[�Q�b�g�̍쐬
		D3D11_RENDER_TARGET_VIEW_DESC rtDesc = {};
		rtDesc.Format = targetFormat;
		rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtDesc.Buffer.ElementOffset = 0;
		rtDesc.Buffer.ElementWidth = 0;
		rtDesc.Buffer.FirstElement = 0;
		rtDesc.Buffer.NumElements = 0;
		rtDesc.Texture2D.MipSlice = 0;
		device->CreateRenderTargetView(textuer.Get(), &rtDesc, targetView.ReleaseAndGetAddressOf());

		// �V�F�[�_���\�[�X�r���[�̍쐬
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
			// �[�x�X�e���V���o�b�t�@�p�e�N�X�`�����쐬
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


			// �[�x�X�e���V���o�b�t�@�̃r���[���쐬
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


		// �r���[�|�[�g�̐ݒ�
		viewport.Width = (float)width;
		viewport.Height = (float)height;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
	}

	/**
	* �����_�[�^�[�Q�b�g��؂�ւ���
	*/
	void Begin(ID3D11DeviceContext *context)
	{
		// ���̃����_�[�^�[�Q�b�g���擾
		context->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, oldTarget, &oldDepth);
		oldViewportNum = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
		context->RSGetViewports(&oldViewportNum, oldViewport);

		ID3D11ShaderResourceView *nullTextures[8] = { nullptr };
		context->PSSetShaderResources(0, 8, nullTextures);

		// �V���h�E�}�b�v�p�̃����_�[�^�[�Q�b�g��ݒ肷��
		ID3D11RenderTargetView *targets[] = { targetView.Get() };
		context->OMSetRenderTargets(1, targets, depthView.Get());
		context->RSSetViewports(1, &viewport);
	}

	/**
	* �����_�[�^�[�Q�b�g�̐؂�ւ��ƃN���A
	*/
	void BeginAndClear(ID3D11DeviceContext *context,Color color)
	{
		Begin(context);

		// �N���A
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
	* �I��
	*/
	void End(ID3D11DeviceContext *context)
	{
		// ���̃����_�[�^�[�Q�b�g��ݒ�
		context->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, oldTarget, oldDepth);
		context->RSSetViewports(oldViewportNum, oldViewport);

		Release();
	}
	/**
	* �e�N�X�`���̎擾
	*/
	ID3D11ShaderResourceView* GetResourceView()
	{
		return resourceView.Get();
	}
};
/**
* �R���X�g���N�^
*/
RenderTarget::RenderTarget() : pImpl_(new Impl())
{
}
/**
* �f�X�g���N�^
*/
RenderTarget::~RenderTarget()
{
}

/**
* �I�u�W�F�N�g�̐���
* @param device -- �f�o�C�X
* @param width  -- ��
* @param height -- ����
* @param targetFormat -- �t�H�[�}�b�g
* @param depthFormat  -- �t�H�[�}�b�g
*/
void RenderTarget::Create(ID3D11Device * device, UINT width, UINT height, DXGI_FORMAT targetFormat, DXGI_FORMAT depthFormat)
{
	pImpl_->Create(device, width, height, targetFormat, depthFormat);
}
/**
* �����_�[�^�[�Q�b�g�̐؂�ւ�
* @param context -- �f�o�C�X�R���e�L�X�g
*/
void RenderTarget::Begin(ID3D11DeviceContext *context)
{
	pImpl_->Begin(context);
}
/**
* �����_�[�^�[�Q�b�g�̐؂�ւ��ƃN���A
* @param context -- �f�o�C�X�R���e�L�X�g
* @param color   -- �F
*/
void RenderTarget::BeginAndClear(ID3D11DeviceContext *context, Color color)
{
	pImpl_->BeginAndClear(context, color);
}
/**
* �I��
* @param context -- �f�o�C�X�R���e�L�X�g
*/
void RenderTarget::End(ID3D11DeviceContext *context)
{
	pImpl_->End(context);
}
/**
* �e�N�X�`���̎擾
*/
ID3D11ShaderResourceView*  RenderTarget::GetResourceView()
{
	return pImpl_->GetResourceView();
}
