#pragma once
#include "Actor3D.h"
#include "Vector2.h"
#include "Quaternion.h"
#include "Animation3D.h"

class UiInventory;
class PlayerScoopAttack;
class ModelDisplay;

class PlayerController : public Actor3D
{
private:
	// アニメーション設定
	enum class Anime
	{
		Idle,
		WalkForward,
		WalkBack,
		ScoopSwing,
		Search,
		Die,

		Length,
	};

	const char* AnimeName[static_cast<int>(Anime::Length)] =
	{
		"Idle",
		"Walk",
		"WalkBack",
		"ScoopSwing",
		"Search",
		"Die"
	};

	const Animation3D AnimeData[static_cast<int>(Anime::Length)] =
	{
		Animation3D("Resource/model/player/player_idle.mv1", true),
		Animation3D("Resource/model/player/player_walk_forward.mv1"),
		Animation3D("Resource/model/player/player_walk_back.mv1"),
		Animation3D("Resource/model/player/scoop_swing.mv1", false),
		Animation3D("Resource/model/player/player_search.mv1", false),
		Animation3D("Resource/model/player/player_die.mv1", false),
	};

	// Transform
	static constexpr Vector3 Scale = Vector3(0.15f, 0.15f, 0.15f);
	static constexpr float ColliderRadius = 2.0f;
	static constexpr Vector3 ColliderBasePos = Vector3(0, ColliderRadius, 0);
	static constexpr Vector3 Colliderpos2Offset = Vector3(0, 5, 0);

	// コントロール
	static constexpr float MoveForwardSpeed = 10.0f;	// 前進速度
	static constexpr float MoveBackwardSpeed = 5.0f;	// 後退速度
	static constexpr float DegAnglePerSecond = 90.0f;	// 地面と水平方向に一秒間に回転する角度

	// スコップ
	static constexpr int RightHandFrameIndex = 18;		// モデルの右手フレーム番号
	static constexpr int RightelbowFrameIndex = 15;		// モデルの肘フレーム番号
	static constexpr Vector3 ScoopScale = Vector3(0.1f, 0.1f, 0.1f);

	const UiInventory* m_uiInventory;
	PlayerScoopAttack* m_playerScoopAttack;
	ModelDisplay* m_scoopObject;
	Vector2 m_inputMove;
	Vector3 m_velocity;

	bool m_canMove;
	bool m_isAttack;
	bool m_isDeath;
	bool m_isInteract;
	int m_seEat;

	void OnMove(const Vector2& inputMove);
	void OnLook(const Vector2& inputMove);
	void UseInventorySlotItem();

protected:
	void Load() override;
	void Release() override;
	void Update() override;

	void OnCollision3D(const Actor3D* other) override;

public:
	PlayerController(
		const Vector3& position, const Quaternion& rotation, const UiInventory* uiInventory
	);

	void AddDamage(int damage);

	void OnInteract();

	void SetPlayerScoopAttack(PlayerScoopAttack* playerScoopAttack) { m_playerScoopAttack = playerScoopAttack; }
};