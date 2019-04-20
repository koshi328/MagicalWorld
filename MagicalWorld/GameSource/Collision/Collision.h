/**
* File: Collision.h
*
* �}�`�̒�`�̏W���A����т����̏Փ˔���̏W��
*
* Date: 2016.11.18
* Author: koshi ito
*/
#pragma once
#include <SimpleMath.h>

// ��s�錾
class Sphere;
class Capsule;

// ���N���X
class Collision
{
public:
	// �R���X�g���N�^
	Collision() = default;
	// �f�X�g���N�^
	~Collision() = default;

	// �_�u���f�B�X�p�b�`
	virtual bool Check(Collision* col) = 0;
	virtual bool Check(Sphere* col) = 0;
	virtual bool Check(Capsule* col) = 0;
	// �ő唼�a���擾
	virtual float getMaxRange() = 0;
};

// �����蔻��N���X
class Sphere : public Collision
{
public:
	DirectX::SimpleMath::Vector3 Center;// ���S���W
	float Radius;						// ���a

	// �R���X�g���N�^
	Sphere()
	{
		Radius = 1.0f;
	}
	// �`�F�b�N�֐�
	bool Check(Collision* col);
	bool Check(Sphere* col);
	bool Check(Capsule* col);

	float getMaxRange() { return Radius; }
};

class Segment
{
public:
	DirectX::SimpleMath::Vector3 Start;// �n�_���W
	DirectX::SimpleMath::Vector3 End;  // �I�_���W
};

// �J�v�Z���i�ʂ��X�E�B�[�v�����`��j
class Capsule : public Collision
{
public:
	Segment Segment;//�c��
	float Radius;	// ���a
	//�@�R���X�g���N�^
	Capsule()
	{
		Segment.Start = DirectX::SimpleMath::Vector3(0, 0, 0);
		Segment.End = DirectX::SimpleMath::Vector3(0, 1, 0);
		Radius = 1.0f;
	}
	// �`�F�b�N�֐�
	bool Check(Collision* col);
	bool Check(Sphere* col);
	bool Check(Capsule* col);

	float getMaxRange()
	{
		float range = DirectX::SimpleMath::Vector3::Distance(Segment.Start, Segment.End);
		if (range / 2 > Radius) return range / 2;
		return Radius;
	}
};


// �@���t���O�p�`�i�����v��肪�\�ʁj
class Triangle
{
public:
	DirectX::SimpleMath::Vector3	P0;
	DirectX::SimpleMath::Vector3	P1;
	DirectX::SimpleMath::Vector3	P2;
	DirectX::SimpleMath::Vector3	Normal;	// �@���x�N�g��
};
bool CheckPoint2Triangle(const DirectX::SimpleMath::Vector3& _point, const Triangle& _triangle);
bool CheckSphere2Triangle(const Sphere& _sphere, const Triangle& _triangle, DirectX::SimpleMath::Vector3 *_inter);

// �����m�̏Փ˔���
bool CheckSphere2Sphere(const Sphere& _sphereA, const Sphere& _sphereB);
bool CheckSphere2Sphere(const Sphere& _sphereA, const Sphere& _sphereB,DirectX::SimpleMath::Vector3* _inter = nullptr);
// �J�v�Z�����m�̏Փ˔���
bool CheckCapsule2Capsule(const Capsule& _capsuleA, const Capsule& _capsuleB);
bool CheckCapsule2Capsule(const Capsule& _capsuleA, const Capsule& _capsuleB,DirectX::SimpleMath::Vector3* _inter = nullptr);
// �J�v�Z���Ƌ��̏Փ˔���
bool CheckSphere2Capsule(const Sphere& _sphere, const Capsule& _capsule);
bool CheckSphere2Capsule(const Sphere& _sphere, const Capsule& _capsule, DirectX::SimpleMath::Vector3* _inter = nullptr);

//float VectorLength(const DirectX::SimpleMath::Vector3& v);
//float Distance3D(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2);


float VectorLengthSQ(const DirectX::SimpleMath::Vector3& v);
float DistanceSQ3D(const DirectX::SimpleMath::Vector3& p1, const DirectX::SimpleMath::Vector3& p2);


float Clamp(float _x, float _min, float _max);
float GetSqDistanceSegment2Segment(const Segment& _segment0, const Segment& _segment1);
float GetSqDistancePoint2Segment(const DirectX::SimpleMath::Vector3& _point, const Segment& _segment);

void ClosestPtSegment2Segment(const Segment& _segment0, const Segment& _segment1, DirectX::SimpleMath::Vector3* _closest0, DirectX::SimpleMath::Vector3* _closest1);
void ClosestPtPoint2Segment(const DirectX::SimpleMath::Vector3& _point, const Segment& _segment, DirectX::SimpleMath::Vector3* _closest);

bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, DirectX::SimpleMath::Vector3 *_inter);