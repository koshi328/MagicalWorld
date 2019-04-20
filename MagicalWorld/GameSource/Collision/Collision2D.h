/**
* File: Collision.h
*
* 2D�}�`�̒�`�̏W���A����т����̏Փ˔���̏W��
*
* Date: 2016.11.30
* Author: koshi ito
*/
#pragma once
#include <SimpleMath.h>

// ���N���X
class Collision2D
{
public:
	// �R���X�g���N�^
	Collision2D() = default;
	// �f�X�g���N�^
	~Collision2D() = default;
public:
	DirectX::SimpleMath::Vector3 _position; // ���S���W
};

/**
* �~�`
*/
class Circle : public Collision2D
{
public:
	// �R���X�g���N�^
	Circle() = default;
	// �f�X�g���N�^
	~Circle() = default;

public:
	float _radius; // ���a
};
/**
* ��`
*/
class FanShape : public Collision2D
{
public:
	// �R���X�g���N�^
	FanShape() = default;
	// �f�X�g���N�^
	~FanShape() = default;
public:
	float _radius;		// ���a
	float _rangeAngle;	// �͈͊p
	DirectX::SimpleMath::Vector3 _forward; // �O�������ւ̃x�N�g��
};
/**
* ��`
*/
class Square : public Collision2D
{
public:
	// �R���X�g���N�^
	Square() = default;
	// �f�X�g���N�^
	~Square() = default;
public:
	float _x;		// X����
	float _z;		// Z����
	float _angle;	// �p�x(�x)
};

// ����֐�
bool Circle2Circle(Circle& circleA, Circle& circleB);
bool Circle2FanFhape(Circle& circle, FanShape& fanShape);
bool Circle2Square(Circle& circle, Square& square);
