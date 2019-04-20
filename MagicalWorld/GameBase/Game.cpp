/**
* File: Game.cpp
*
* �Q�[���̃��C������
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
// �T�E���h
#include "..\Sound\ADX2Le.h"
#include "..\Resources\Music\MagicalWorld_acf.h"
#include "..\Resources\Music\WorkUnit_0\CueSheet_0.h"

/**
* ���O���
*/
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

/**
* �O���[�o���֐�
*/
/**
* ���b�V���}�l�[�W���[���擾
*/
MeshManager* getMeshManager()
{
	static MeshManager* instance = new MeshManager;
	return instance;
}
/**
* �^�X�N�}�l�[�W���[���擾
*/
TaskManager* getTaskManager()
{
	static TaskManager* instance = new TaskManager;
	return instance;
}
/**
* �p�[�e�B�N���}�l�[�W���[���擾
*/
ParticleManager* getParticleManager()
{
	static ParticleManager* instance = new ParticleManager(Graphics::getInstance()->getDeviceContext());
	return instance;
}

/**
* �Q�[���N���X�̃C���v�������g
*/
class Game::Impl
{
public:
	Graphics* _pGraphics;						// �O���t�B�b�N
	unique_ptr<CameraManager> _pCameraManager;	// �J�����}�l�[�W���[
	unique_ptr<SceneManager> _pSceneManager;	// �V�[���}�l�[�W���[
public:
	/**
	* �R���X�g���N�^
	*/
	Impl()
	{
		_pSceneManager.reset(SceneManager::getInstance());
		_pCameraManager.reset(CameraManager::getInstance());
	}
	/**
	* �f�X�g���N�^
	*/
	~Impl()
	{

	}
	/**
	* ������
	*/
	void Initialize()
	{
		// �O���t�B�b�N�f�o�C�X�̐���
		_pGraphics = Graphics::getInstance();
		// �G�t�F�N�g�t�@�N�g���[�̐���
		unique_ptr<DGSLEffectFactory> factory(new DGSLEffectFactory(_pGraphics->getDevice()));
		factory->SetDirectory(L"Resources/cModels");
		// �I�[�f�B�I�ݒ�
		ADX2Le::Initialize("Resources/Music/MagicalWorld.acf");
		ADX2Le::LoadAcb("Resources/Music/WorkUnit_0/CueSheet_0.acb", "Resources/Music/WorkUnit_0/CueSheet_0.awb");

		// ���b�V���}�l�[�W���[�̐ݒ�
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

		// �p�[�e�B�N���}�l�[�W���[�̐ݒ�
		getParticleManager()->Initialize();
		getParticleManager()->LoadTexture(L"Resources/Media/bit");
		getParticleManager()->SetAutoBillboard();

		// �����V�[���̐���
		_pSceneManager->setNextScene(SceneManager::SCENE_ID::TITLE);
		_pSceneManager->ChangeScene(SceneManager::CHANGE_TYPE::BKACK_FADE);
	}
	/**
	* �X�V
	*/
	void Update()
	{
		// �`�惂�f���̃��Z�b�g
		getMeshManager()->Reset();
		// �I�[�f�B�I�X�V
		ADX2Le::Update();
		// �p�[�e�B�N���}�l�[�W���[�̍X�V
		getParticleManager()->Update();
		// �J�����̍X�V
		_pCameraManager->Update();
		// �V�[���̍X�V
		_pSceneManager->Update();
		// �I������
		if (Key::getInstance()->Touch(VK_ESCAPE))
		{
			Mouse::getInstance()->setMode(Mouse::MODE_ABSOLUTE);
			if (MessageBox(NULL, TEXT("�I�����܂����H"),
				TEXT("�m�F�E�B���h�E"), MB_YESNO) == IDYES)
			{
				PostQuitMessage(0);	// ���C�����[�v�𔲂���
			}
			else
			{
				Mouse::getInstance()->setMode(Mouse::MODE_RELATIVE);
			}
		}
	}
	/**
	* �`��
	*/
	void Render()
	{
		// �^�X�N�}�l�[�W���[�̕`��
		getTaskManager()->Run();
		// �V�[���̕`��
		_pSceneManager->Render();
		Camera* pCamera = _pCameraManager->getCamera();
		getMeshManager()->Render(_pGraphics->getCommonStates(), pCamera->GetViewmat(), pCamera->GetProjmat());
		// �p�[�e�B�N���̕`��
		getParticleManager()->Draw(pCamera->GetViewmat(), pCamera->GetProjmat());
	}
	/**
	* �I��
	*/
	void Finalize()
	{
		// ���
		ADX2Le::Finalize();
	}
};

/**
* �R���X�g���N�^
*/
Game::Game()
	:GameBase(this), _pImpl(new Impl())
{

}
/**
* �f�X�g���N�^
*/
Game::~Game()
{
	delete getMeshManager();
	delete getParticleManager();
	delete getTaskManager();
}
/**
* ������
*/
void Game::Initialize()
{
	GameBase::Initialize();
	_pImpl->Initialize();
}
/**
* �X�V
*/
void Game::Update()
{
	_pImpl->Update();
	GameBase::Update();
}
/**
* �`��
*/
void Game::Render()
{
	_pImpl->Render();
	GameBase::Render();
}
/**
* �I��
*/
void Game::Finalize()
{
	_pImpl->Finalize();
	GameBase::Finalize();
}