/**
* File: GameObject.cpp
*
* ゲームに登場するオブジェクトの基底クラス
*
* Date: 2016.10.15
* Author: koshi ito
*/
#include "GameObject.h"
/**
* 名前空間
*/
using namespace DirectX::SimpleMath;

/**
* コンストラクタ
*/
GameObject::GameObject()
:isDestroy(false), scale_(Vector3(1, 1, 1))
{
}
/**
* デストラクタ
*/
GameObject::~GameObject()
{
}
