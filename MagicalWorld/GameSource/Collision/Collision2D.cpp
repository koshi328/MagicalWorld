/**
* File: Collision.h
*
* 2D�}�`�̒�`�̏W���A����т����̏Փ˔���̏W��
*
* Date: 2016.11.30
* Author: koshi ito
*/
#include "Collision2D.h"

/**
* ���O���
*/
using namespace DirectX::SimpleMath;

/**
* �~�Ɖ~�̏Փ˔���
* @param circleA --- �}�`�P
* @param circleB --- �}�`�Q
* @return �Փ˂������ۂ�
*/
bool Circle2Circle(Circle& circleA, Circle& circleB)
{
	// �����x�N�g�������߂�
	Vector3 lengthVec = circleA._position - circleB._position;
	// ���������߂�
	float distance = lengthVec.x * lengthVec.x + lengthVec.z * lengthVec.z;
	if (distance <= (circleA._radius + circleB._radius) * (circleA._radius + circleB._radius))
	{
		return true;
	}
	return false;
}

/**
* �~�Ɛ�̏Փ˔���
* @param circle   --- �~
* @param fanShape --- ��
* @return �Փ˂������ۂ�
*/
bool Circle2FanFhape(Circle& circle, FanShape& fanShape)
{
	// �����x�N�g�������߂�
	Vector3 lengthVec = circle._position - fanShape._position;
	// ���������߂�
	float distance = lengthVec.x * lengthVec.x + lengthVec.z * lengthVec.z;
	// �~�`�͈͓��ł��邱�Ƃ�ۏ؂���
	if (distance > (circle._radius + fanShape._radius) * (circle._radius + fanShape._radius))
	{
		return false;
	}
	// ���Ίp�x�����߂�
	lengthVec.Normalize();
	fanShape._forward.Normalize();
	float range = lengthVec.Dot(fanShape._forward);
	range /= lengthVec.Length() * fanShape._forward.Length();
	range = acosf(range);
	// �͈͓������肷��
	if (range * 180.f / 3.14f <= fanShape._rangeAngle)
	{
		return true;
	}
	return false;
}

/**
* �~�Ƌ�`�̏Փ˔���
* @param circle --- �~
* @param square --- ��`
* @return �Փ˂������ۂ�
*/
bool Circle2Square(Circle& circle, Square& square)
{
	// �����x�N�g�������߂�
	Vector3 lengthVec = circle._position - square._position;
	// ���W�ʒu����̍����p�����߂�
	float angle = atan2(lengthVec.x, lengthVec.z);
	// ���������߂�
	float distance = sqrtf(lengthVec.x * lengthVec.x + lengthVec.z * lengthVec.z);
	// Square�̉�]�ʂ����W�֌W�ɏ�悹����
	angle -= square._angle;
	// Square�ɑ΂���Circle�̑��Έʒu�����߂�
	Vector3 point;
	point.x = sinf(angle) * distance + square._position.x;
	point.z = cosf(angle) * distance + square._position.z;

	if (square._position.x + square._x / 2 > point.x - circle._radius &&
		square._position.x - square._x / 2 < point.x + circle._radius &&
		square._position.z + square._z / 2 > point.z - circle._radius &&
		square._position.z - square._z / 2 < point.z + circle._radius)
	{
		return true;
	}
	return false;
}