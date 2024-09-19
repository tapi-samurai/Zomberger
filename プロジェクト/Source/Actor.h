﻿#pragma once
#include "Node.h"
#include "Transform.h"
//#include "Sprite.h"
#include <string>

class Sprite;
class Collider;

class Actor : public Node
{
protected:
	std::string m_name;		// オブジェクト名
	Transform m_transform;	// 姿勢情報
	Sprite* m_sprite;		// 2D画像表示
	Collider* m_collider;	// 当たり判定

	virtual void Load() override;		// リソースの読み込み
	virtual void Release() override;	// リソースの解放
	virtual void Draw() override;		// 描画

public:
	// コンストラクタ
	Actor(const char* name, const char* textureName = nullptr, const Vector3& position = Vector3());

	// 子ノードを含む更新
	virtual void TreeUpdate() override;

	// 姿勢の取得
	const Transform& GetTransform() const
	{
		return m_transform;
	}

	// 座標の取得
	const Vector2& GetPosition() const
	{
		return m_transform.position;
	}

	// 名前の取得
	const std::string& GetName() const
	{
		return m_name;
	}

	// 当たり判定の取得
	const Collider* GetCollider() const
	{
		return m_collider;
	}

	// 衝突イベント
	virtual void OnCollision(const Actor* other) {}
};