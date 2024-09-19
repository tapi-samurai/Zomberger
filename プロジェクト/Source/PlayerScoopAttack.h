#pragma once
#include "Actor3D.h"

class PlayerController;

class PlayerScoopAttack : public Actor3D
{
private:
	static constexpr float AttackDelay = 0.6f;		// ボタンを押してから判定が有効化するまでの遅延
	static constexpr float AttackDuration = 0.3f;	// 判定が無効化するまでの持続時間
	static constexpr float AttackCoolTime = 1.0f;

	static constexpr float LengthAsPlayer = 5.0f;	// プレイヤーまでの距離
	static constexpr float ColliderRadius = 3.0f;	// 当たり判定の半径
	static constexpr int BreakProbability = 5;		// スコップが壊れる確率(0% ～ 100%)

	const PlayerController* m_playerController;

	float m_coolTime;
	float m_elapsedTime;
	int m_inventoryNum;
	bool m_isAttack;
	int m_seSwing;
	int m_seHit;

protected:
	void Load() override;
	void Release() override;
	void Update() override;

	void OnCollision3D(const Actor3D* other) override;

public:
	PlayerScoopAttack(const PlayerController* playerController);

	bool const OnAttack(int inventoryNum);
};