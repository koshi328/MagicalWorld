/**
* File: RangeMap.cpp
*
* �U���\���͈͂�\������N���X
*
* Date: 2017.2.1
* Author: koshi ito
*/
#include "RangeMap.h"
#include "..\GameSource\Collision\Collision2D.h"
#include "..\Utility\RenderTarget.h"
#include "..\Utility\BinaryLoader.h"
#include "..\Utility\MeshManager.h"
#include "..\GameBase\Graphics.h"
#include "..\Utility\ConstantBuffer.h"

using namespace std;
using namespace DirectX::SimpleMath;
using namespace DirectX;
using namespace Microsoft::WRL;
/**
* �O���[�o���֐�
*/
extern MeshManager* getMeshManager();
/**
* static�ϐ�
*/
RangeMap* RangeMap::instance_ = nullptr;

/**
* �R���X�^���g�o�b�t�@
*/
struct MatrixesConstant
{
	XMMATRIX WVP;
	XMMATRIX AboveWVP;

	using Type = ConstantBuffer<MatrixesConstant>;
};

/**
* �C���v�������g�N���X
*/
class RangeMap::Impl
{
public:
	unique_ptr<MeshManager> meshManager;				// �͈͕\�����f����`�悷�郁�b�V���}�l�[�W���[
	unique_ptr<RenderTarget> renderTarget[2];			// �����_�[�^�[�Q�b�g
	unique_ptr<MatrixesConstant::Type> matrixesConstant;// �R���X�^���g�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11VertexShader> rangeVS;	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> rangePS;	// �s�N�Z���V�F�[�_�[
	int renderNum;										// �����_�[�^�[�Q�b�g�؂�ւ��p
	// �R���X�g���N�^
	Impl()
	{
		ID3D11Device* device = Graphics::getInstance()->getDevice();
		// ���b�V���}�l�[�W���[�̐���
		meshManager = make_unique<MeshManager>();
		meshManager->Initialize(50, Graphics::getInstance()->getDeviceContext(), Graphics::getInstance()->getDevice());
		// �����_�[�^�[�Q�b�g�̐���
		renderTarget[0] = make_unique<RenderTarget>();
		renderTarget[0]->Create(device, 1280 * 2, 720 * 2, 
			DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
		renderTarget[1] = make_unique<RenderTarget>();
		renderTarget[1]->Create(device, 1280 * 2, 720 * 2,
			DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
		renderNum = 0;

		// �t�@�C���I�u�W�F�N�g
		BinaryLoader buffer;
		// �\���pshader
		buffer.Load(L"Resources/cModels/rangeVS.cso");
		device->CreateVertexShader(buffer.Data(), buffer.Size(), nullptr, rangeVS.GetAddressOf());
		buffer.Load(L"Resources/cModels/rangePS.cso");
		device->CreatePixelShader(buffer.Data(), buffer.Size(), nullptr, rangePS.GetAddressOf());
		// �R���X�^���g�o�b�t�@�̐���
		matrixesConstant.reset(new MatrixesConstant::Type(device));
	}
	// �f�X�g���N�^
	~Impl()
	{

	}
	/**
	* �T�[�N���͈͂�o�^����
	* @param circle -- Collision2D��Circle�̃|�C���^
	*/
	void Add(Circle* circle)
	{
		ID3D11DeviceContext* context = Graphics::getInstance()->getDeviceContext();
		CommonStates* state = Graphics::getInstance()->getCommonStates();
		Matrix world = Matrix::Identity;
		Matrix trans = Matrix::CreateTranslation(Vector3(circle->_position.x, 0.f, circle->_position.z));
		Matrix scale = Matrix::CreateScale(circle->_radius);
		world *= scale * trans;
		meshManager->Entry(5, world, [context, state]{
			context->OMSetBlendState(state->AlphaBlend(), Colors::Brown, 0xFFFFFFFF);
		});
	}
	/**
	* ��`�͈͂�o�^����
	* @param square -- Collision2D��Square�̃|�C���^
	*/
	void Add(Square* square)
	{
		ID3D11DeviceContext* context = Graphics::getInstance()->getDeviceContext();
		CommonStates* state = Graphics::getInstance()->getCommonStates();
		Matrix world = Matrix::Identity;
		Matrix trans = Matrix::CreateTranslation(Vector3(square->_position.x, 0.f, square->_position.z));
		Matrix scale = Matrix::CreateScale(Vector3(square->_x / 2, 1.f, square->_z / 2));
		Matrix rotY = Matrix::CreateRotationY(square->_angle);
		world *= scale * rotY * trans;

		meshManager->Entry(7, world, [context, state] {
			context->OMSetBlendState(state->AlphaBlend(), Colors::Brown, 0xFFFFFFFF);
		});
	}
	/**
	* ��͈͂�o�^����
	* @param fanShape -- Collision2D��FanShape�̃|�C���^
	*/
	void Add(FanShape* fanShape)
	{
		//Matrix world = Matrix::Identity;
		//Matrix trans = Matrix::CreateTranslation(Vector3(fanShape->_position.x, 0.f, fanShape->_position.z));
		//Matrix scale = Matrix::CreateScale(fanShape->_radius);
		//Matrix rotY = Matrix::CreateRotationY();
		//meshManager->Entry(5, world);
	}
	/**
	* �͈͕\���̃e�N�X�`���𐶐�����
	* @param context -- �f�o�C�X�R���e�L�X�g
	* @param state   -- CommonState
	*/
	void CreateTexture(ID3D11DeviceContext* context, CommonStates* state)
	{
		Graphics* graphics = Graphics::getInstance();
		// �������ރ����_�[��؂�ւ���
		auto& main = renderTarget[renderNum];
		renderNum = 1 - renderNum;
		auto& sub = renderTarget[renderNum];
		// ��������猩��J������ݒ�
		Matrix aboveView = Matrix::CreateLookAt(
			Vector3(0, 900, 0),
			Vector3(0, 0, 0),
			Vector3(1, 0, 0));
		Matrix aboveProj = Matrix::CreatePerspectiveFieldOfView(
			XMConvertToRadians(45), 1280.f / 720.f, 10, 1000);
		Matrix world = Matrix::Identity;
		world *= aboveView * aboveProj;
		// �R���X�^���g�o�b�t�@��ݒ�
		matrixesConstant->AboveWVP = world.Transpose();
		static float time = 0.f;
		time += 0.2f;
		// ���f����`��
		main->BeginAndClear(context, Color(1, 1, 1, 1));
		meshManager->Render(state, aboveView, aboveProj);
		main->End(context);
		meshManager->Reset();
		// �����x��ς���
		sub->BeginAndClear(context, Color(1, 1, 1, 1));
		graphics->getSpriteBatch()->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred,
			graphics->getCommonStates()->NonPremultiplied(),
			graphics->getCommonStates()->PointClamp());
		RECT rect = rect = { 0,0,1280 * 2,720 * 2 };
		graphics->getSpriteBatch()->Draw(main->GetResourceView(), rect, Color(1, 1, 1, sinf(time) + 1.0f));
		graphics->getSpriteBatch()->End();
		sub->End(context);
	}
	/**
	* DirectX::Model��CustomState��ݒ肷��֐�
	* @param context -- �f�o�C�X�R���e�L�X�g
	* @param state   -- CommonState
	*/
	std::function<void(void)> CustomState(ID3D11DeviceContext* context, CommonStates* state)
	{
		return [this, context, state]() {
			// map��unmap�̏���
			matrixesConstant->SetData(context);
			// �o�[�e�b�N�X�V�F�[�_�̐ݒ�
			context->VSSetShader(rangeVS.Get(), nullptr, 0);
			context->PSSetShader(rangePS.Get(), nullptr, 0);
			// �o�[�e�b�N�X�V�F�[�_�ɓn���R���X�^���g�o�b�t�@
			ID3D11Buffer *buffer[] = {
				matrixesConstant->GetBuffer(),
			};

			context->VSSetConstantBuffers(0, 1, buffer);
			context->PSSetConstantBuffers(0, 1, buffer);
			// �e�N�X�`���̐ݒ�
			ID3D11ShaderResourceView *textures[] = { renderTarget[renderNum]->GetResourceView() };
			context->PSSetShaderResources(1, 1, textures);

			ID3D11SamplerState *samplers[] = { state->LinearClamp() };
			context->PSSetSamplers(1, 1, samplers);
		};
	}
};
/**
* �R���X�g���N�^
*/
RangeMap::RangeMap() : impl_(new Impl())
{
}

/**
* �f�X�g���N�^
*/
RangeMap::~RangeMap()
{
	instance_ = nullptr;
}
/**
* �~�͈͔͈͂�ǉ�����֐�
* @param circle -- Collision2D��Circle�|�C���^
*/
void RangeMap::Add(Circle* circle)
{
	impl_->Add(circle);
}
/**
* ��`�͈͂�ǉ�����֐�
* @param square -- Collision2D��Square�|�C���^
*/
void RangeMap::Add(Square* square)
{
	impl_->Add(square);
}
/**
* ��͈͂�ǉ�����֐�
* @param fanShape -- Collision2D��FanShape�|�C���^
*/
void RangeMap::Add(FanShape* fanShape)
{
	impl_->Add(fanShape);
}
/**
* �͈͕\���̃e�N�X�`���𐶐�����֐�
* @param context -- �f�o�C�X�R���e�L�X�g
* @param state   -- CommonState
*/
void RangeMap::CreateTexture(ID3D11DeviceContext* context, CommonStates* state)
{
	impl_->CreateTexture(context, state);
}
/**
* DirectX::Model��CustomState��ݒ肷��֐�
* @param context -- �f�o�C�X�R���e�L�X�g
* @param state   -- CommonState
*/
std::function<void(void)> RangeMap::CustomState(ID3D11DeviceContext* context, CommonStates* state)
{
	return impl_->CustomState(context, state);
}
/**
* �R���X�^���g�o�b�t�@��ݒ肷��֐��i���_�J������WorldViewProjection�s��j
* @param wvp -- ��ʂ��ʂ��Ă���J������ViewProjection
*/
void RangeMap::setWorldViewProjection(const DirectX::SimpleMath::Matrix& wvp)
{
	impl_->matrixesConstant->WVP = wvp;
}
/**
* �R���X�^���g�o�b�t�@��ݒ肷��֐��i�X�e�[�W�������낷���_�̃J�����j
* @param wvp -- ��ʂ��ォ�猩�Ă���WorldViewProjection�s��
*/
void RangeMap::setAboveWorldViewProjection(const DirectX::SimpleMath::Matrix& wvp)
{
	impl_->matrixesConstant->AboveWVP = wvp;
}
/**
* �͈͕\���̃e�N�X�`�����擾����֐�
*/
ID3D11ShaderResourceView* RangeMap::getResourceView()
{
	return impl_->renderTarget[0]->GetResourceView();
}