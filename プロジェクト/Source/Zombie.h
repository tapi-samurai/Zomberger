#pragma once
#include "Actor3D.h"

class PlayerController;

class Zombie : public Actor3D
{
private:
	// �A�j���[�V�����ݒ�
	enum class Anime
	{
		Clim,
		WalkForward,
		Attack,
		Die,

		Length,
	};

	const char* AnimeName[static_cast<int>(Anime::Length)] =
	{
		"Clim",
		"WalkForward",
		"Attack",
		"Die",
	};

	const Animation3D AnimeData[static_cast<int>(Anime::Length)] =
	{
		Animation3D("Resource/model/zombie/zombie_clim.mv1", false),
		Animation3D("Resource/model/zombie/zombie_walk.mv1"),
		Animation3D("Resource/model/zombie/zombie_attack.mv1", false),
		Animation3D("Resource/model/zombie/zombie_die.mv1", false),
	};

	// ��ԊǗ�
	enum class State
	{
		Spawn,
		SpawnWait, // �X�|�[�����̃A�j���[�V������
		Attack,
		OnChase,
		Die,
	};

	// Transform
	static constexpr float SpawnOffsetY = -10;						// �X�|�[�����̍������W�I�t�Z�b�g
	static constexpr float SpawnOffsetBefore = -7;					// �X�|�[�����̑O����W�I�t�Z�b�g
	static constexpr Vector3 Scale = Vector3(0.15f, 0.15f, 0.15f);
	static constexpr Vector3 Colliderpos2Offset = Vector3(0, 2, 0);
	static constexpr float ColliderRadius = 3.0f;

	// �R���g���[��
	static constexpr float MoveSpeed = 2.0f;
	static constexpr float DegAnglePerSecond = 45.0f;	// ��b�Ԃɉ�]����p�x
	static constexpr float AttackCooldownTime = 3.0f;	// �v���C���[�Ƀ_���[�W��^������A��~���鎞��

	// �X�e�[�^�X
	static constexpr int DamageValue = 1;
	static constexpr int MaxHealth = 3;

	// �{�C�XSE
	static constexpr int MaxVoiceInterval = 10;
	static constexpr int MinVoiceInterval = 5;

	PlayerController* m_playerController;
	State m_state;
	Vector3 m_velocity;

	int m_health;
	int m_seSpawn;
	int m_seVoice;
	int m_voiceNum;
	float m_elapsedTime;
	float m_remainingCooldownTime;
	float m_nextVoiceInterval;

	void LookAtPlayer();
	void MoveForward();
	void Die();

protected:
	void Load() override;
	void Release() override;
	void Update() override;

	void OnCollision3D(const Actor3D* other) override;

public:
	Zombie(PlayerController* playerController, const Vector3& position);
};