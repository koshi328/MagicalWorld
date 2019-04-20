/**
* File: Collision.h
*
* 図形の定義の集合、およびそれらの衝突判定の集合
*
* Date: 2016.11.18
* Author: koshi ito
*/
#pragma once
#include <SimpleMath.h>

// 先行宣言
class Sphere;
class Capsule;

// 基底クラス
class Collision
{
public:
	// コンストラクタ
	Collision() = default;
	// デストラクタ
	~Collision() = default;

	// ダブルディスパッチ
	virtual bool Check(Collision* col) = 0;
	virtual bool Check(Sphere* col) = 0;
	virtual bool Check(Capsule* col) = 0;
	// 最大半径を取得
	virtual float getMaxRange() = 0;
};

// 当たり判定クラス
class Sphere : public Collision
{
public:
	DirectX::SimpleMath::Vector3 Center;// 中心座標
	float Radius;						// 半径

	// コンストラクタ
	Sphere()
	{
		Radius = 1.0f;
	}
	// チェック関数
	bool Check(Collision* col);
	bool Check(Sphere* col);
	bool Check(Capsule* col);

	float getMaxRange() { return Radius; }
};

class Segment
{
public:
	DirectX::SimpleMath::Vector3 Start;// 始点座標
	DirectX::SimpleMath::Vector3 End;  // 終点座標
};

// カプセル（玉をスウィープした形状）
class Capsule : public Collision
{
public:
	Segment Segment;//芯線
	float Radius;	// 半径
	//　コンストラクタ
	Capsule()
	{
		Segment.Start = DirectX::SimpleMath::Vector3(0, 0, 0);
		Segment.End = DirectX::SimpleMath::Vector3(0, 1, 0);
		Radius = 1.0f;
	}
	// チェック関数
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


// 法線付き三角形（反時計回りが表面）
class Triangle
{
public:
	DirectX::SimpleMath::Vector3	P0;
	DirectX::SimpleMath::Vector3	P1;
	DirectX::SimpleMath::Vector3	P2;
	DirectX::SimpleMath::Vector3	Normal;	// 法線ベクトル
};
bool CheckPoint2Triangle(const DirectX::SimpleMath::Vector3& _point, const Triangle& _triangle);
bool CheckSphere2Triangle(const Sphere& _sphere, const Triangle& _triangle, DirectX::SimpleMath::Vector3 *_inter);

// 球同士の衝突判定
bool CheckSphere2Sphere(const Sphere& _sphereA, const Sphere& _sphereB);
bool CheckSphere2Sphere(const Sphere& _sphereA, const Sphere& _sphereB,DirectX::SimpleMath::Vector3* _inter = nullptr);
// カプセル同士の衝突判定
bool CheckCapsule2Capsule(const Capsule& _capsuleA, const Capsule& _capsuleB);
bool CheckCapsule2Capsule(const Capsule& _capsuleA, const Capsule& _capsuleB,DirectX::SimpleMath::Vector3* _inter = nullptr);
// カプセルと球の衝突判定
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