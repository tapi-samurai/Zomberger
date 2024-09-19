#pragma once
#include "Actor3D.h"

class PlayerController;

class PlayerScoopAttack : public Actor3D
{
private:
	static constexpr float AttackDelay = 0.6f;		// �{�^���������Ă��画�肪�L��������܂ł̒x��
	static constexpr float AttackDuration = 0.3f;	// ���肪����������܂ł̎�������
	static constexpr float AttackCoolTime = 1.0f;

	static constexpr float LengthAsPlayer = 5.0f;	// �v���C���[�܂ł̋���
	static constexpr float ColliderRadius = 3.0f;	// �����蔻��̔��a
	static constexpr int BreakProbability = 5;		// �X�R�b�v������m��(0% �` 100%)

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