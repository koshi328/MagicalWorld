/**
* File: Terrain.h
*
* 地形との衝突判定
*
* Date: 2016.11.30
* Author: koshi ito
*/
#pragma once
#include <map>
#include <memory>
#include "TerrainData.h"
#include "Collision.h"
#include <SimpleMath.h>

class Terrain
{
public:
	// コンストラクタ
	Terrain();
	// デストラクタ
	~Terrain();
	// 地形データの読み込み
	void LoadTerrainData(UINT num, wchar_t* mdlfile,
		DirectX::SimpleMath::Vector3 position = DirectX::SimpleMath::Vector3(0, 0, 0),
		DirectX::SimpleMath::Vector3 rotation = DirectX::SimpleMath::Vector3(0, 0, 0),
		DirectX::SimpleMath::Vector3 scale = DirectX::SimpleMath::Vector3(1, 1, 1));
	// 更新
	void Update();
	// モデルの姿勢の計算
	void Calc();
	// 描画
	void Draw();

	// 衝突判定
	bool IntersectSphere(const Sphere& sphere, DirectX::SimpleMath::Vector3* reject);
	void ComputeTriangle(const DirectX::SimpleMath::Vector3& _p0, const DirectX::SimpleMath::Vector3& _p1, const DirectX::SimpleMath::Vector3& _p2, Triangle* _triangle);
	bool IntersectSegment(const Segment& segment, DirectX::SimpleMath::Vector3* inter);
private:
	static std::map<std::wstring, std::unique_ptr<TerrainData>> s_terrainDataArray;	// 地形データのプール
private:
	const TerrainData* terrainData_; // 地形データ
	// ワールド→モデル行列
	DirectX::SimpleMath::Matrix m_WorldLocal;

	UINT modelNum_;
	DirectX::SimpleMath::Vector3 position_;
	DirectX::SimpleMath::Vector3 rotation_;
	DirectX::SimpleMath::Vector3 scale_;

	DirectX::SimpleMath::Matrix CreateWorldMatrix();
};