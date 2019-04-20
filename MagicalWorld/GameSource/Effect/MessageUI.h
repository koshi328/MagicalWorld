/**
* File: MessageUI.h
* 文字表示
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include <SimpleMath.h>
struct MessageUI
{
	// 生成位置
	DirectX::SimpleMath::Vector3 position_;
	// 表示時間
	int count_;
	// 表示文字
	wchar_t str_[32];
	// 色
	DirectX::SimpleMath::Color color_;
	// 初期化
	void Initialize(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		const float& screenWidth, const float& screenHight,
		const DirectX::SimpleMath::Vector3& pos, const int& count, const wchar_t* buf,
		DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1));
	// 実行
	bool Run();
};

