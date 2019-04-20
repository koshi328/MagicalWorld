/**
* File: RangeMap.h
*
* 攻撃予測範囲を表示するクラス
*
* Date: 2017.2.1
* Author: koshi ito
*/
#pragma once
#include <memory>
#include <d3d11.h>
#include <functional>
#include <CommonStates.h>
#include <SimpleMath.h>

/**
* 前方宣言
*/
class Circle;
class Square;
class FanShape;

class RangeMap
{
public:
	// シングルトン
	static RangeMap* getInstance()
	{
		if (!instance_)
			instance_ = new RangeMap();
		return instance_;
	}
	// デストラクタ
	~RangeMap();
	// 表示範囲を追加する
	void Add(Circle* circle);
	void Add(Square* square);
	void Add(FanShape* fanShape);
	// 表示範囲を描いたテクスチャを生成する
	void CreateTexture(ID3D11DeviceContext* context, DirectX::CommonStates* state);
	// DirectX::ModelのCustomStateを設定する関数
	std::function<void(void)> CustomState(ID3D11DeviceContext* context, DirectX::CommonStates* state);
	// コンスタントバッファを設定する
	void setWorldViewProjection(const DirectX::SimpleMath::Matrix& wvp);
	void setAboveWorldViewProjection(const DirectX::SimpleMath::Matrix& wvp);
	// 範囲テクスチャを取得する
	ID3D11ShaderResourceView* getResourceView();
private:
	// インプリメントクラス
	class Impl;
	std::unique_ptr<Impl> impl_;
	// コンストラクタ
	RangeMap();
	static RangeMap* instance_; // インスタンス
};

