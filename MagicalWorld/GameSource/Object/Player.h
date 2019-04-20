/**
* File: Player.h
* プレイヤー制御
*
* Data:   2016.10.30
* Author: Koshi ito
*/
#pragma once
#include "Actor.h"
#include <memory>

class Sphere;
class Canvas;
class SkillController;

class Player : public Actor
{
private:
	std::unique_ptr<Sphere> collision_;
	std::unique_ptr<Canvas> canvas_;
	std::unique_ptr<SkillController> skillController_;
	int unrivaledTime_;
public:
	Player(DirectX::SimpleMath::Vector3 position);
	~Player();

	// 衝突判定へのアクセス
	Collision* getCollision();

	void Initialize();
	void Update();
	void Draw();

	void Move();

	bool LoadStatus();
	void Respawn();
	void SearchItem();
	void setSkill(int type);
};