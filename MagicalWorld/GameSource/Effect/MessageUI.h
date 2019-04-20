/**
* File: MessageUI.h
* �����\��
*
* Date: 2017.1.10
* Author: koshi ito
*/
#pragma once

#include <SimpleMath.h>
struct MessageUI
{
	// �����ʒu
	DirectX::SimpleMath::Vector3 position_;
	// �\������
	int count_;
	// �\������
	wchar_t str_[32];
	// �F
	DirectX::SimpleMath::Color color_;
	// ������
	void Initialize(
		const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj,
		const float& screenWidth, const float& screenHight,
		const DirectX::SimpleMath::Vector3& pos, const int& count, const wchar_t* buf,
		DirectX::SimpleMath::Color color = DirectX::SimpleMath::Color(1, 1, 1, 1));
	// ���s
	bool Run();
};

