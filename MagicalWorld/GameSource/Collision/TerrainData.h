/**
* File: TerrainData.cpp
*
* �n�`����p�̃f�[�^�Q�Ɠǂݍ��ݗp�֐�
*
* Date: 2016.11.24
* Author: koshi ito
*/
#pragma once
#include <vector>
#include <memory>
#include <SimpleMath.h>

class TerrainData
{
public:
	// ���_���\���́i�K�v�Ȃ��̂̂݁j
	struct VERTEX_LANDSHAPE
	{
		DirectX::SimpleMath::Vector3 Pos;		// ���W
		DirectX::SimpleMath::Vector3 Normal;	// �@���x�N�g��
	};

	// ���_���\���́i�ǂݍ��݃f�[�^�j
	struct VERTEX_FILE
	{
		DirectX::SimpleMath::Vector3 Pos;		// ���W
		DirectX::SimpleMath::Vector3 Normal;	// �@���x�N�g��
		DirectX::SimpleMath::Vector2 UV;		// �e�N�Z��
	};

	// �m�[�h���
	struct NodeInfo
	{
		// �}�e���A�����\����
		struct Matrial
		{
			char name[32];
			DirectX::SimpleMath::Vector3 Ambient;
			DirectX::SimpleMath::Vector3 Diffuse;
			DirectX::SimpleMath::Vector3 Specular;
			DirectX::SimpleMath::Vector3 Emissive;
			float Opacity;
			float Shininess;
			float Reflectivity;
		};
		// ���
		int kind;
		// ���O
		char name[32];
		// �Z��̃m�[�h�ԍ�
		int sibling;
		// �q���̃m�[�h�ԍ�
		int child;
		// ��{�p��
		DirectX::SimpleMath::Vector3 translation;
		DirectX::SimpleMath::Quaternion rotation;
		// �e�N�X�`����(Diffuse)
		char textureNameA[32];
		// �e�N�X�`����(Specular)
		char textureNameB[32];
		// �e�N�X�`����(Bump)
		char textureNameC[32];
		// �}�e���A�����
		Matrial matrial;
	};
	// ���O
	std::wstring name;
	// ���_�f�[�^�z��
	std::vector<VERTEX_LANDSHAPE> m_Vertices;
	// �C���f�b�N�X�f�[�^�z��
	std::vector<USHORT> m_Indices;
	// ���f���f�[�^�ǂݍ���
	static std::unique_ptr<TerrainData> CreateFromMDL(const char* meshData);
	static std::unique_ptr<TerrainData> CreateFromMDL(const wchar_t* szFileName);
	// ���W�ϊ�
	static void UpdataNodeMatrices(int index, const NodeInfo* nodeInfo_array, 
		DirectX::SimpleMath::Matrix* pParentMatrix, std::vector<std::vector<VERTEX_LANDSHAPE>>& vertexArray);
};