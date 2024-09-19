#pragma once
#include "Math.h"

class PlayerState
{
private:
	int m_health;
	int m_satietyLevel;

public:
	static constexpr int MaxHealth = 5;			// �ő�̗�
	static constexpr int MaxSatietyLevel = 3;	// �ő�󕠓x

	// �V���O���g��
	static PlayerState* GetInstance()
	{
		static PlayerState instance;
		return &instance;
	}

	PlayerState() :
		m_health(MaxHealth),
		m_satietyLevel(MaxSatietyLevel) {}

	void SetHealth(int value) { m_health = Math::Clamp(m_health + value, 0, MaxHealth); }

	void SetSatietyLevel(int value) { m_satietyLevel = Math::Clamp(m_satietyLevel + value, 0, MaxSatietyLevel); }

	const int GetHealth() const { return m_health; }

	const int GetSatietyLevel() const { return m_satietyLevel; }
};