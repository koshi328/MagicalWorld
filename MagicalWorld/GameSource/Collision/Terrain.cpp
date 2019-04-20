/**
* File: Terrain.cpp
*
* �n�`�Ƃ̏Փ˔���
*
* Date: 2016.11.30
* Author: koshi ito
*/
#include "Terrain.h"
#include "..\..\Utility\MeshManager.h"
#include "..\..\GameBase\Graphics.h"
#include "..\..\RangeMap\RangeMap.h"
#include "..\Camera\CameraManager.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;


extern MeshManager* getMeshManager();
// static�̎��̉��錾
map<wstring, unique_ptr<TerrainData>> Terrain::s_terrainDataArray;

/**
* �R���X�g���N�^
*/
Terrain::Terrain()
{

}

/**
* �f�X�g���N�^
*/
Terrain::~Terrain()
{

}

// -----------------------------------------------------------
// �n�`�f�[�^�̓ǂݍ���
// cmofile : �`�惂�f��(.cmo)
// mdlfile : ���胂�f��(.MDL)
// -----------------------------------------------------------
/**
* �n�`�f�[�^�̓ǂݍ���
* @param cmofile --- �`�惂�f���ԍ�
* @param mdlfile --- ���胂�f���A�h���X
*/
void Terrain::LoadTerrainData(UINT num, wchar_t* mdlfile,
	DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 rotation, DirectX::SimpleMath::Vector3 scale)
{
	if (mdlfile)
	{
		if (s_terrainDataArray.count(mdlfile) == 0)
		{
			// ���f���̓ǂݍ���
			s_terrainDataArray[mdlfile] = TerrainData::CreateFromMDL(mdlfile);
		}
		// �n�`�f�[�^���Z�b�g
		terrainData_ = s_terrainDataArray[mdlfile].get();
	}

	modelNum_ = num;
	position_ = position;
	rotation_ = rotation;
	scale_ = scale;
	this->Calc();
}

/**
* �X�V
*/
void Terrain::Update()
{
	ID3D11DeviceContext* context = Graphics::getInstance()->getDeviceContext();
	DirectX::CommonStates* state = Graphics::getInstance()->getCommonStates();

	Matrix view = CameraManager::getInstance()->getCamera()->GetViewmat();
	Matrix proj = CameraManager::getInstance()->getCamera()->GetProjmat();
	Matrix wvp = CreateWorldMatrix() * view * proj;
	RangeMap::getInstance()->setWorldViewProjection(wvp.Transpose());
	RangeMap::getInstance()->CreateTexture(context, state);
	getMeshManager()->Entry(modelNum_, CreateWorldMatrix(), RangeMap::getInstance()->CustomState(context, state));
}

/**
* �p���̌v�Z
*/
void Terrain::Calc()
{
	// �t�s����v�Z
	const Matrix& localworld = CreateWorldMatrix();
	m_WorldLocal = localworld.Invert();
}

/**
* �`��
*/
void Terrain::Draw()
{

}

/**
* ���_�f�[�^����O�p�`�𐶐�
*/
void Terrain::ComputeTriangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, Triangle* _triangle)
{
	_triangle->P0 = _p0;
	_triangle->P1 = _p1;
	_triangle->P2 = _p2;

	Vector3 PO_P1 = _triangle->P1 - _triangle->P0;
	Vector3 PO_P2 = _triangle->P2 - _triangle->P0;

	_triangle->Normal = PO_P1.Cross(PO_P2);
	_triangle->Normal.Normalize();
}

/**
* ���ƒn�`�̏Փ˔���
* @param sphere --- ���苅
* @param reject --- �Փ˓_�i�o�́j
* @return �Փ˂������ۂ�
*/
bool Terrain::IntersectSphere(const Sphere& sphere, Vector3* reject)
{
	if (terrainData_ == nullptr)return false;

	int nTri = terrainData_->m_Indices.size() / 3;

	bool hit = false;
	float distance = 1.0e5;
	Vector3 l_inter;
	Vector3 l_normal;
	Vector3 l_down;

	float scale = 1.0f;
	//float scale = GetScale();
	Sphere localsphere = sphere;

	if (scale <= 1.0e-10)return false;

	localsphere.Center = Vector3::Transform(sphere.Center, m_WorldLocal);
	localsphere.Radius = sphere.Radius / scale;

	for (int i = 0; i < nTri; i++)
	{
		int index0 = terrainData_->m_Indices[i * 3];
		int index1 = terrainData_->m_Indices[i * 3 + 1];
		int index2 = terrainData_->m_Indices[i * 3 + 2];

		Vector3 pos0 = terrainData_->m_Vertices[index0].Pos;
		Vector3 pos1 = terrainData_->m_Vertices[index1].Pos;
		Vector3 pos2 = terrainData_->m_Vertices[index2].Pos;

		Triangle tri;

		ComputeTriangle(pos0, pos1, pos2, &tri);

		float temp_distance;
		Vector3 temp_inter;

		if (CheckSphere2Triangle(localsphere, tri, &temp_inter))
		{
			hit = true;
			temp_distance = Vector3::Distance(localsphere.Center, temp_inter);
			if (temp_distance < distance)
			{
				l_inter = temp_inter;
				l_normal = tri.Normal;
				distance = temp_distance;
			}
		}
	}

	if (hit)
	{
		distance *= scale;

		const Matrix& localworld = CreateWorldMatrix();

		if (reject != nullptr)
		{
			*reject = Vector3::TransformNormal(l_normal, localworld);
			reject->Normalize();
			*reject = (*reject) * (sphere.Radius - distance);
		}
	}
	return hit;
}

/**
* �n�`�Ɛ����̌�������
* @param segment --- ����
* @param inter   --- ��_�i�o�́j
* @return �����������ۂ�
*/
bool Terrain::IntersectSegment(const Segment& segment, Vector3* inter)
{
	if (terrainData_ == nullptr) return false;

	// �O�p�`�̐�
	int nTri = terrainData_->m_Indices.size() / 3;
	// �q�b�g�t���O��������
	bool hit = false;
	// �傫�������ŏ�����
	float distance = 1.0e5;
	// �p�x����p�ɒn�ʂƂ݂Ȃ��p�x�̌��E�l<�x>
	const float limit_angle = XMConvertToRadians(30.0f);
	Vector3 l_inter;

	// �R�s�[
	Segment localSegment = segment;
	// ���������[���h���W���烂�f�����W�n�Ɉ�������
	localSegment.Start = Vector3::Transform(localSegment.Start, m_WorldLocal);
	localSegment.End = Vector3::Transform(localSegment.End, m_WorldLocal);
	// �����̕����x�N�g�����擾
	Vector3 segmentNormal = localSegment.End - localSegment.End;
	segmentNormal.Normalize();

	for (int i = 0; i < nTri; i++)
	{
		// �O�p�`�̊e���_�̃C���f�b�N�X���擾
		int index0 = terrainData_->m_Indices[i * 3];
		int index1 = terrainData_->m_Indices[i * 3 + 1];
		int index2 = terrainData_->m_Indices[i * 3 + 2];
		// �e���_�̃��[�J�����W���擾
		Vector3 pos0 = terrainData_->m_Vertices[index0].Pos;
		Vector3 pos1 = terrainData_->m_Vertices[index1].Pos;
		Vector3 pos2 = terrainData_->m_Vertices[index2].Pos;

		Triangle tri;

		// 3�_����O�p�`���\�z�iTODO:��Ɍv�Z���Ă����j
		ComputeTriangle(pos0, pos1, pos2, &tri);

		float temp_distance;
		Vector3 temp_inter;

		// ������x�N�g���Ɩ@���̓���
		float dot = -segmentNormal.Dot(tri.Normal);
		// ������Ƃ̊p�x�����v�Z
		float angle = acosf(dot);
		// ������Ƃ̊p�x�����E�p���傫����΁A�n�ʂƂ݂Ȃ����A�X�L�b�v
		if (angle > limit_angle) continue;

		// �����ƎO�p�`�i�|���S���j�̌�������
		if (CheckSegment2Triangle(localSegment, tri, &temp_inter))
		{
			hit = true;
			// �Փ˓_�܂ł̋������Z�o
			temp_distance = Vector3::Distance(localSegment.Start, temp_inter);
			// ��������ԋ߂����̂��L�^
			if (temp_distance < distance)
			{
				l_inter = temp_inter;
				distance = temp_distance;
			}
		}
	}

	if (hit && inter != nullptr)
	{
		// ��_�����f�����W�n���烏�[���h���W�n�ɕϊ�
		const Matrix& localworld = CreateWorldMatrix();
		*inter = Vector3::Transform(l_inter, localworld);
	}

	return hit;
}

/**
* �p���̌v�Z
* @return �ړ��A��]�A�g�k��K������WorldMatrix
*/
DirectX::SimpleMath::Matrix Terrain::CreateWorldMatrix()
{
	Matrix world = Matrix::Identity;
	Matrix t = Matrix::CreateTranslation(position_);
	Matrix r = Matrix::CreateFromYawPitchRoll(rotation_.y, rotation_.x, rotation_.z);
	Matrix s = Matrix::CreateScale(scale_);
	world = r * s * t;
	return world;
}