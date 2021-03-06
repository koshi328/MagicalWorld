/**
* File: TerrainData.cpp
*
* 地形判定用のデータ群と読み込み用関数
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
	// 頂点情報構造体（必要なもののみ）
	struct VERTEX_LANDSHAPE
	{
		DirectX::SimpleMath::Vector3 Pos;		// 座標
		DirectX::SimpleMath::Vector3 Normal;	// 法線ベクトル
	};

	// 頂点情報構造体（読み込みデータ）
	struct VERTEX_FILE
	{
		DirectX::SimpleMath::Vector3 Pos;		// 座標
		DirectX::SimpleMath::Vector3 Normal;	// 法線ベクトル
		DirectX::SimpleMath::Vector2 UV;		// テクセル
	};

	// ノード情報
	struct NodeInfo
	{
		// マテリアル情報構造体
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
		// 種類
		int kind;
		// 名前
		char name[32];
		// 兄弟のノード番号
		int sibling;
		// 子供のノード番号
		int child;
		// 基本姿勢
		DirectX::SimpleMath::Vector3 translation;
		DirectX::SimpleMath::Quaternion rotation;
		// テクスチャ名(Diffuse)
		char textureNameA[32];
		// テクスチャ名(Specular)
		char textureNameB[32];
		// テクスチャ名(Bump)
		char textureNameC[32];
		// マテリアル情報
		Matrial matrial;
	};
	// 名前
	std::wstring name;
	// 頂点データ配列
	std::vector<VERTEX_LANDSHAPE> m_Vertices;
	// インデックスデータ配列
	std::vector<USHORT> m_Indices;
	// モデルデータ読み込み
	static std::unique_ptr<TerrainData> CreateFromMDL(const char* meshData);
	static std::unique_ptr<TerrainData> CreateFromMDL(const wchar_t* szFileName);
	// 座標変換
	static void UpdataNodeMatrices(int index, const NodeInfo* nodeInfo_array, 
		DirectX::SimpleMath::Matrix* pParentMatrix, std::vector<std::vector<VERTEX_LANDSHAPE>>& vertexArray);
};