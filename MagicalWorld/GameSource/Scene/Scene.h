/**
* File: Scene.h
*
* シーンの基底クラス
*
* Date: 2016.11.1
* Author: koshi ito
*/
#pragma once

class Scene
{
public:
	// コンストラクタ
	Scene() = default;
	// デストラクタ
	virtual ~Scene() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Render() = 0;
};