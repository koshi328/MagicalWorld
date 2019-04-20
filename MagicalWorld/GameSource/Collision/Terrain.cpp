/**
* File: Terrain.cpp
*
* 地形との衝突判定
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
// staticの実体化宣言
map<wstring, unique_ptr<TerrainData>> Terrain::s_terrainDataArray;

/**
* コンストラクタ
*/
Terrain::Terrain()
{

}

/**
* デストラクタ
*/
Terrain::~Terrain()
{

}

// -----------------------------------------------------------
// 地形データの読み込み
// cmofile : 描画モデル(.cmo)
// mdlfile : 判定モデル(.MDL)
// -----------------------------------------------------------
/**
* 地形データの読み込み
* @param cmofile --- 描画モデル番号
* @param mdlfile --- 判定モデルアドレス
*/
void Terrain::LoadTerrainData(UINT num, wchar_t* mdlfile,
	DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Vector3 rotation, DirectX::SimpleMath::Vector3 scale)
{
	if (mdlfile)
	{
		if (s_terrainDataArray.count(mdlfile) == 0)
		{
			// モデルの読み込み
			s_terrainDataArray[mdlfile] = TerrainData::CreateFromMDL(mdlfile);
		}
		// 地形データをセット
		terrainData_ = s_terrainDataArray[mdlfile].get();
	}

	modelNum_ = num;
	position_ = position;
	rotation_ = rotation;
	scale_ = scale;
	this->Calc();
}

/**
* 更新
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
* 姿勢の計算
*/
void Terrain::Calc()
{
	// 逆行列を計算
	const Matrix& localworld = CreateWorldMatrix();
	m_WorldLocal = localworld.Invert();
}

/**
* 描画
*/
void Terrain::Draw()
{

}

/**
* 頂点データから三角形を生成
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
* 球と地形の衝突判定
* @param sphere --- 判定球
* @param reject --- 衝突点（出力）
* @return 衝突したか否か
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
* 地形と線分の交差判定
* @param segment --- 線分
* @param inter   --- 交点（出力）
* @return 交差したか否か
*/
bool Terrain::IntersectSegment(const Segment& segment, Vector3* inter)
{
	if (terrainData_ == nullptr) return false;

	// 三角形の数
	int nTri = terrainData_->m_Indices.size() / 3;
	// ヒットフラグを初期化
	bool hit = false;
	// 大きい数字で初期化
	float distance = 1.0e5;
	// 角度判定用に地面とみなす角度の限界値<度>
	const float limit_angle = XMConvertToRadians(30.0f);
	Vector3 l_inter;

	// コピー
	Segment localSegment = segment;
	// 線分をワールド座標からモデル座標系に引き込む
	localSegment.Start = Vector3::Transform(localSegment.Start, m_WorldLocal);
	localSegment.End = Vector3::Transform(localSegment.End, m_WorldLocal);
	// 線分の方向ベクトルを取得
	Vector3 segmentNormal = localSegment.End - localSegment.End;
	segmentNormal.Normalize();

	for (int i = 0; i < nTri; i++)
	{
		// 三角形の各頂点のインデックスを取得
		int index0 = terrainData_->m_Indices[i * 3];
		int index1 = terrainData_->m_Indices[i * 3 + 1];
		int index2 = terrainData_->m_Indices[i * 3 + 2];
		// 各頂点のローカル座標を取得
		Vector3 pos0 = terrainData_->m_Vertices[index0].Pos;
		Vector3 pos1 = terrainData_->m_Vertices[index1].Pos;
		Vector3 pos2 = terrainData_->m_Vertices[index2].Pos;

		Triangle tri;

		// 3点から三角形を構築（TODO:先に計算しておく）
		ComputeTriangle(pos0, pos1, pos2, &tri);

		float temp_distance;
		Vector3 temp_inter;

		// 上方向ベクトルと法線の内積
		float dot = -segmentNormal.Dot(tri.Normal);
		// 上方向との角度差を計算
		float angle = acosf(dot);
		// 上方向との角度が限界角より大きければ、地面とみなさず、スキップ
		if (angle > limit_angle) continue;

		// 線分と三角形（ポリゴン）の交差判定
		if (CheckSegment2Triangle(localSegment, tri, &temp_inter))
		{
			hit = true;
			// 衝突点までの距離を算出
			temp_distance = Vector3::Distance(localSegment.Start, temp_inter);
			// 距離が一番近いものを記録
			if (temp_distance < distance)
			{
				l_inter = temp_inter;
				distance = temp_distance;
			}
		}
	}

	if (hit && inter != nullptr)
	{
		// 交点をモデル座標系からワールド座標系に変換
		const Matrix& localworld = CreateWorldMatrix();
		*inter = Vector3::Transform(l_inter, localworld);
	}

	return hit;
}

/**
* 姿勢の計算
* @return 移動、回転、拡縮を適応したWorldMatrix
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