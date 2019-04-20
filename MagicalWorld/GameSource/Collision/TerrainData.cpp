/**
* File: TerrainData.cpp
*
* 地形判定用のデータ群と読み込み用関数
*
* Date: 2016.11.24
* Author: koshi ito
*/
#include "TerrainData.h"
#include <fstream>
#include <algorithm>

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

/**
* 地形データの読み込み
* @param meshData --- ノードの名前
* @return 生成された地形データのポインタ
*/
std::unique_ptr<TerrainData> TerrainData::CreateFromMDL(const char* meshData)
{
	// 最新の読み込み位置を示すポインタ
	const void* head = meshData;

	std::unique_ptr<TerrainData> landshape(new TerrainData());
	// ノード数を読み取る
	const UINT* p_nNode = static_cast<const UINT*>(head);
	// 読み込み位置を進める
	head = p_nNode + 1;
	// ノード数取得
	UINT nNode = *p_nNode;
	// ノード情報を読み取る
	const NodeInfo* nodeInfo_array = static_cast<const NodeInfo*>(head);
	head = nodeInfo_array + nNode;

	int mesh = 0;
	// メッシュの頂点データとインデックスデータを読み込み
	landshape->m_Indices.clear();
	// ノード数分×各ノードの頂点数の二次元配列
	vector<vector<VERTEX_LANDSHAPE>> vertex_array;
	vector<vector<USHORT>> index_array;
	for (UINT i = 0; i < nNode; i++)
	{
		// ノード情報一個分を取得
		const NodeInfo* nodeInfo = &nodeInfo_array[i];

		vector<VERTEX_LANDSHAPE> va;
		vector<USHORT> ia;

		vertex_array.push_back(va);
		index_array.push_back(ia);
		// メッシュなら
		if (nodeInfo->kind == 1)
		{
			// 頂点データ数を読み取る
			const UINT* p_nVertex = static_cast<const UINT*>(head);
			head = p_nVertex + 1;
			// 頂点データ数
			UINT nVertex = *p_nVertex;
			vertex_array[i].reserve(nVertex);
			size_t vbBytes = sizeof(VERTEX_FILE)* nVertex;

			const VERTEX_FILE* node_vertex_array = static_cast<const VERTEX_FILE*>(head);
			head = node_vertex_array + nVertex;

			for (UINT j = 0; j < nVertex; j++)
			{
				const VERTEX_FILE* vertex = &node_vertex_array[j];

				VERTEX_LANDSHAPE vertex_landshape;

				vertex_landshape.Pos = vertex->Pos;
				vertex_landshape.Normal = vertex->Normal;

				vertex_array[i].push_back(vertex_landshape);
			}

			const UINT* p_nIndices = static_cast<const UINT*>(head);
			head = p_nIndices + 1;

			UINT nIndices = *p_nIndices;
			index_array[i].reserve(nIndices);

			size_t ibBytes = sizeof(USHORT)* nIndices;
			const USHORT* node_index_array = static_cast<const USHORT*>(head);
			head = node_index_array + nIndices;

			for (UINT j = 0; j < nIndices; j++)
			{
				const USHORT* index = &node_index_array[j];
				index_array[i].push_back(*index);
			}
		}
	}
	UpdataNodeMatrices(0, nodeInfo_array, nullptr, vertex_array);

	landshape->m_Vertices.clear();
	landshape->m_Indices.clear();
	int vertex_count = 0;
	for (UINT i = 0; i < nNode; i++)
	{
		vector<USHORT>::iterator it;
		for (it = index_array[i].begin(); it != index_array[i].end(); it++)
		{
			*it += vertex_count;
		}

		vertex_count += vertex_array[i].size();

		landshape->m_Vertices.insert(landshape->m_Vertices.end(), vertex_array[i].begin(), vertex_array[i].end());
		landshape->m_Indices.insert(landshape->m_Indices.end(), index_array[i].begin(), index_array[i].end());
	}
	return landshape;
}

/**
* 地形データの読み込み
* @param meshData --- 読み込むMDLのアドレス
* @return 生成された地形データのポインタ
*/
std::unique_ptr<TerrainData> TerrainData::CreateFromMDL(const wchar_t* szFileName)
{
	ifstream ifs;
	std::unique_ptr<char[]> data;

	ifs.open(szFileName, ios::in | ios::binary);

	assert(ifs && "CreateFromMDL Error: Failed to open MDL.");

	ifs.seekg(0, fstream::end);
	streamoff eofPos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, fstream::beg);
	streamoff begPos = ifs.tellg();
	streamoff size = eofPos - begPos;

	data.reset(new char[(UINT)size]);

	ifs.read(data.get(), size);

	ifs.close();

	OutputDebugStringW(szFileName);
	OutputDebugStringW(L"\n");

	auto model = CreateFromMDL(data.get());
	model->name = szFileName;

	return model;
}

/**
* 各ノードの頂点を親行列で変換
*/
void TerrainData::UpdataNodeMatrices(int index, const NodeInfo* nodeInfo_array, Matrix* pParentMatrix, vector<vector<VERTEX_LANDSHAPE>>& vertexArray)
{
	const NodeInfo* nodeInfo = &nodeInfo_array[index];

	Matrix modelm;

	Matrix rotm = Matrix::CreateFromQuaternion(nodeInfo->rotation);

	Matrix transm = Matrix::CreateTranslation(nodeInfo->translation);

	modelm *= rotm;
	modelm *= transm;

	if (pParentMatrix != nullptr)
	{
		modelm = modelm * (*pParentMatrix);
	}

	if (nodeInfo->kind == 1)
	{
		vector<VERTEX_LANDSHAPE>::iterator it;

		for (it = vertexArray[index].begin(); it != vertexArray[index].end(); it++)
		{
			it->Pos = Vector3::Transform(it->Pos, modelm);
			it->Normal = Vector3::TransformNormal(it->Normal, modelm);

			char str[256];

			sprintf_s(str, "(%f.3,%f.3,%f.3)\n", it->Pos.x, it->Pos.y, it->Pos.z);

			OutputDebugStringA(str);
		}
	}

	if (nodeInfo->sibling > 0)
	{
		UpdataNodeMatrices(nodeInfo->sibling, nodeInfo_array, pParentMatrix, vertexArray);
	}

	if (nodeInfo->child > 0)
	{
		UpdataNodeMatrices(nodeInfo->child, nodeInfo_array, &modelm, vertexArray);
	}
}