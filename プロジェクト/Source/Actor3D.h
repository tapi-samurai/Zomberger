#pragma once
#include "Node.h"
#include "Transform3D.h"
#include "Model.h"
#include <string>

class Collider3D;
class BitFlag;

// Actorクラスの3Dバージョン

class Actor3D : public Node
{
protected:
	std::string m_name;			// オブジェクト名
	Transform3D m_transform;	// 姿勢情報
	Model* m_model;				// 3D画像表示
	Collider3D* m_collider;		// 当たり判定
	BitFlag* m_bitFlag;			// 共通で使用するビットフラグ

	bool m_isActive;

	virtual void Load() override;		// リソースの読み込み
	virtual void Release() override;	// リソースの解放
	virtual void Draw() override;		// 描画

public:
	// コンストラクタ
	Actor3D(const char* name, const char* modelName = nullptr, int baseFrameIndex = 0, bool isActive = true);

	// 子ノードを含む更新
	virtual void TreeUpdate() override;

	// 姿勢の取得
	const Transform3D& GetTransform() const
	{
		return m_transform;
	}

	// 座標の取得
	const Vector3& GetPosition() const
	{
		return m_transform.position;
	}

	// 回転の取得
	const Quaternion& GetRotation() const
	{
		return m_transform.rotation;
	}

	// 名前の取得
	const std::string& GetName() const
	{
		return m_name;
	}

	// 当たり判定の取得
	const Collider3D* GetCollider3D() const
	{
		return m_collider;
	}

	// 衝突イベント
	virtual void OnCollision3D(const Actor3D* other) {}

	// モデルハンドルの取得
	const int GetModelHandle() const 
	{
		return m_model->GetModelHandle();
	}

	// ビットフラグの取得
	BitFlag* GetBitFlag() const 
	{
		return m_bitFlag;
	}

	// アクティブ切り替え
	void SetActive(bool state) { m_isActive = state; }

	bool IsActive() { return m_isActive; }
};