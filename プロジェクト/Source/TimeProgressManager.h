#pragma once
#include "Math.h"

class TimeProgressManager
{
public:
	static constexpr int InitialDay = 0;
	static constexpr int InitialTimezone = 0;
	static constexpr float InitialSecond = 0.0f;
	static constexpr int MaxDay = 99;
	static constexpr int MaxTimezone = 4;
	static constexpr float MaxSecond = 60.0f;

private:
	int m_elapsedDay;		// �o�ߓ���
	int m_elapsedTimezone;	// ���ԑ�
	float m_elapsedSecond;	// �o�ߕb

public:
	// �V���O���g��
	static TimeProgressManager* GetInstance()
	{
		static TimeProgressManager instance;
		return &instance;
	}

	TimeProgressManager() :
		m_elapsedDay(InitialDay),
		m_elapsedTimezone(InitialTimezone),
		m_elapsedSecond(InitialSecond) {}

	// �o�ߓ������擾
	const int GetElapsedDay() const { return m_elapsedDay; }

	// ���ԑт��擾
	const int GetElapsedTimezone() const { return m_elapsedTimezone; }

	// �o�ߕb���擾
	const float GetElapsedSecond() const { return m_elapsedSecond; }

	// �o�ߓ������w�蕪�i�߂�
	void AddElapsedDay(int addDay = 1) { m_elapsedDay = Math::Clamp(m_elapsedDay + addDay, InitialDay, MaxDay); }

	// ���ԑт��w�蕪�i�߂�
	void AddElapsedTimezone(int addTimezone = 1) { m_elapsedTimezone = Math::Clamp(m_elapsedTimezone + addTimezone, InitialTimezone, MaxTimezone); }

	// �o�ߕb���w�蕪�i�߂�
	void AddElapsedSecond(float addSecond) { m_elapsedSecond = Math::Clamp(m_elapsedSecond + addSecond, InitialSecond, MaxSecond); }

	// �o�ߓ�����������
	void DayInitialization() { m_elapsedDay = InitialDay; }

	// ���ԑт�������
	void TimezoneInitialization() { m_elapsedTimezone = InitialTimezone; }

	// �o�ߕb��������
	void SecondInitialization() { m_elapsedSecond = InitialSecond; }
};