/**
* File: Collision.h
*
* 2D図形の定義の集合、およびそれらの衝突判定の集合
*
* Date: 2016.11.30
* Author: koshi ito
*/
#pragma once
#include <SimpleMath.h>

// 基底クラス
class Collision2D
{
public:
	// コンストラクタ
	Collision2D() = default;
	// デストラクタ
	~Collision2D() = default;
public:
	DirectX::SimpleMath::Vector3 _position; // 中心座標
};

/**
* 円形
*/
class Circle : public Collision2D
{
public:
	// コンストラクタ
	Circle() = default;
	// デストラクタ
	~Circle() = default;

public:
	float _radius; // 半径
};
/**
* 扇形
*/
class FanShape : public Collision2D
{
public:
	// コンストラクタ
	FanShape() = default;
	// デストラクタ
	~FanShape() = default;
public:
	float _radius;		// 半径
	float _rangeAngle;	// 範囲角
	DirectX::SimpleMath::Vector3 _forward; // 前方方向へのベクトル
};
/**
* 矩形
*/
class Square : public Collision2D
{
public:
	// コンストラクタ
	Square() = default;
	// デストラクタ
	~Square() = default;
public:
	float _x;		// X長さ
	float _z;		// Z長さ
	float _angle;	// 角度(度)
};

// 判定関数
bool Circle2Circle(Circle& circleA, Circle& circleB);
bool Circle2FanFhape(Circle& circle, FanShape& fanShape);
bool Circle2Square(Circle& circle, Square& square);
