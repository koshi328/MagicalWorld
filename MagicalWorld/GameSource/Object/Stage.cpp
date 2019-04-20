/**
* File: Stage.cpp
*
* �Q�[���̃X�e�[�W���Ǘ�����N���X
*
* Date: 2017.1.20
* Author: koshi ito
*/
#include "Stage.h"
#include <sstream>
#include <fstream>
#include "ObjectManager.h"
#include "EnemyCreator.h"
#include <locale.h>

using namespace std;
using namespace DirectX::SimpleMath;
/**
* static�ϐ�
*/
StageManager* StageManager::instance_ = nullptr;
/**
* �R���X�g���N�^
*/
StageManager::StageManager()
{
	Initialize();
}
/**
* ������
*/
void StageManager::Initialize()
{
	maxTime_ = 0.f;
	time_ = 0.f;
	killEnemyNum_ = 0;
	stageLevel_ = 1;
	creatorPos_.clear();
}
/**
* �X�V
*/
void StageManager::Update()
{
	// ���Ԃ̌v�Z
	time_ -= 1 / 60.f;
	// ���̃��x���ɕK�v�ȓ��������v�Z
	int nextMoreNum = (stageLevel_ * (stageLevel_ + 1) / 2) * 10;
	if (nextMoreNum <= killEnemyNum_)
	{
		int level = stageLevel_;
		wchar_t buf[64];
		swprintf_s(buf, 64, L"Resources/Data/Enemy/Creator/LEVEL%d.txt", level);
		for each(Vector3 pos in creatorPos_)
		{
			char c[64];
			size_t size;
			wcstombs_s(&size, c, 64, buf, 64);
			string str = c;
			EnemyCreator* creator = EnemyCreatorSetting(str);
			if(creator)
				creator->Position(pos);
		}
		stageLevel_++;
	}
}
/**
* �X�e�[�W���̓ǂݍ���
* @param filename -- �ǂݍ��ރX�e�[�W�t�@�C���̃A�h���X
*/
bool StageManager::LoadStageInfo(const wchar_t* filename)
{
	string str;
	ifstream ifs(filename);
	if (ifs.fail()) return false;
	while (getline(ifs, str))
	{
		string tmp;
		istringstream stream(str);
		getline(stream, tmp, ',');
		if (tmp == "FIELD")
		{
			TerrainSetting(str);
		}
		else if (tmp == "ENEMYCREATOR")
		{
			getline(stream, tmp, ',');
			EnemyCreator* creator = EnemyCreatorSetting(tmp);
			Vector3 pos;
			getline(stream, tmp, ',');
			pos.x = atoi(tmp.c_str()) * 1.f;
			getline(stream, tmp, ',');
			pos.y = atoi(tmp.c_str()) * 1.f;
			getline(stream, tmp, ',');
			pos.z = atoi(tmp.c_str()) * 1.f;
			creator->Position(pos);
		}
		else if (tmp == "IVENT")
		{
			getline(stream, tmp, ',');
			IventSetting(tmp);
		}
	}
	// �X�e�[�W�ɂ���N���G�C�^�[�̃|�W�V���������ׂċL������
	for each (GameObject* object in ObjectManager::getInstance()->getActorList(L"EnemyCreator"))
	{
		creatorPos_.push_back(object->Position());
	}
	ifs.close();
	return true;
}
/**
* �n�`�t�@�C���̓ǂݍ���
* @param str -- �n�`�ݒ�̕�����
*/
bool StageManager::TerrainSetting(const std::string& str)
{
	Terrain* terrain = new Terrain;
	int modelID = 0;
	Vector3 pos(0, 0, 0);
	Vector3 rot(0, 0, 0);
	Vector3 scale(1, 1, 1);
	wchar_t mdl[256];
	string tmp;
	istringstream stream(str);
	// �w�蕶����ǂݔ�΂�
	getline(stream, tmp, ',');
	// ���f��ID
	getline(stream, tmp, ',');
	modelID = atoi(tmp.c_str());
	// MDL
	getline(stream, tmp, ',');
	size_t ret;
	mbstowcs_s(&ret, mdl, 256, tmp.c_str(), _TRUNCATE);
	// ���W
	getline(stream, tmp, ',');
	pos.x = atoi(tmp.c_str()) * 1.f;
	getline(stream, tmp, ',');
	pos.y = atoi(tmp.c_str()) * 1.f;
	getline(stream, tmp, ',');
	pos.z = atoi(tmp.c_str()) * 1.f;
	// ��]
	getline(stream, tmp, ',');
	rot.x = atoi(tmp.c_str()) * 1.f;
	getline(stream, tmp, ',');
	rot.y = atoi(tmp.c_str()) * 1.f;
	getline(stream, tmp, ',');
	rot.z = atoi(tmp.c_str()) * 1.f;
	// �X�P�[��
	getline(stream, tmp, ',');
	scale.x = atoi(tmp.c_str()) * 1.f;
	getline(stream, tmp, ',');
	scale.y = atoi(tmp.c_str()) * 1.f;
	getline(stream, tmp, ',');
	scale.z = atoi(tmp.c_str()) * 1.f;

	terrain->LoadTerrainData(modelID, mdl, pos, rot, scale);
	ObjectManager::getInstance()->Add(terrain);
	return true;
}
/**
* EnemyCreator�̐���
* @param str -- EnemyCreator�̐ݒ蕶����
*/
EnemyCreator* StageManager::EnemyCreatorSetting(const std::string& str)
{
	EnemyCreator* creator = nullptr;
	string line;
	ifstream ifs(str);
	if (ifs.fail()) return creator;
	creator = new EnemyCreator;
	while (getline(ifs, line))
	{
		string tmp;
		string data;
		istringstream stream(line);
		getline(stream, tmp, ',');
		getline(stream, data);
		if (tmp == "MAX")
			creator->setMaxCreateNum(atoi(data.c_str()));
		else if (tmp == "TIME")
			creator->setFrequency(atoi(data.c_str()));
		else if (tmp == "ENEMY")
		{
			Enemy* enemy = new Enemy;
			enemy->setCreator(creator);
			wchar_t buf[256];
			size_t ret;
			mbstowcs_s(&ret, buf, 256, data.c_str(), _TRUNCATE);
			enemy->LoadStatus(buf);
			creator->setEnemyPrototype(enemy);
		}
	}
	ifs.close();
	ObjectManager::getInstance()->Add(L"EnemyCreator", creator);
	return creator;
}
/**
* �C�x���g�ݒ�
* @param str -- �C�x���g�̐ݒ蕶����
*/
bool StageManager::IventSetting(const std::string& str)
{
	string tmp;
	istringstream stream(str);
	while (getline(stream, tmp, ','))
	{

	}
	return true;
}