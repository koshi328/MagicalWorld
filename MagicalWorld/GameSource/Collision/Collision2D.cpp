/**
* File: Collision.h
*
* 2D図形の定義の集合、およびそれらの衝突判定の集合
*
* Date: 2016.11.30
* Author: koshi ito
*/
#include "Collision2D.h"

/**
* 名前空間
*/
using namespace DirectX::SimpleMath;

/**
* 円と円の衝突判定
* @param circleA --- 図形１
* @param circleB --- 図形２
* @return 衝突したか否か
*/
bool Circle2Circle(Circle& circleA, Circle& circleB)
{
	// 差分ベクトルを求める
	Vector3 lengthVec = circleA._position - circleB._position;
	// 距離を求める
	float distance = lengthVec.x * lengthVec.x + lengthVec.z * lengthVec.z;
	if (distance <= (circleA._radius + circleB._radius) * (circleA._radius + circleB._radius))
	{
		return true;
	}
	return false;
}

/**
* 円と扇の衝突判定
* @param circle   --- 円
* @param fanShape --- 扇
* @return 衝突したか否か
*/
bool Circle2FanFhape(Circle& circle, FanShape& fanShape)
{
	// 差分ベクトルを求める
	Vector3 lengthVec = circle._position - fanShape._position;
	// 距離を求める
	float distance = lengthVec.x * lengthVec.x + lengthVec.z * lengthVec.z;
	// 円形範囲内であることを保証する
	if (distance > (circle._radius + fanShape._radius) * (circle._radius + fanShape._radius))
	{
		return false;
	}
	// 相対角度を求める
	lengthVec.Normalize();
	fanShape._forward.Normalize();
	float range = lengthVec.Dot(fanShape._forward);
	range /= lengthVec.Length() * fanShape._forward.Length();
	range = acosf(range);
	// 範囲内か判定する
	if (range * 180.f / 3.14f <= fanShape._rangeAngle)
	{
		return true;
	}
	return false;
}

/**
* 円と矩形の衝突判定
* @param circle --- 円
* @param square --- 矩形
* @return 衝突したか否か
*/
bool Circle2Square(Circle& circle, Square& square)
{
	// 差分ベクトルを求める
	Vector3 lengthVec = circle._position - square._position;
	// 座標位置からの差分角を求める
	float angle = atan2(lengthVec.x, lengthVec.z);
	// 距離を求める
	float distance = sqrtf(lengthVec.x * lengthVec.x + lengthVec.z * lengthVec.z);
	// Squareの回転量を座標関係に上乗せする
	angle -= square._angle;
	// Squareに対するCircleの相対位置を求める
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