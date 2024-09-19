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
	int m_elapsedDay;		// 経過日数
	int m_elapsedTimezone;	// 時間帯
	float m_elapsedSecond;	// 経過秒

public:
	// シングルトン
	static TimeProgressManager* GetInstance()
	{
		static TimeProgressManager instance;
		return &instance;
	}

	TimeProgressManager() :
		m_elapsedDay(InitialDay),
		m_elapsedTimezone(InitialTimezone),
		m_elapsedSecond(InitialSecond) {}

	// 経過日数を取得
	const int GetElapsedDay() const { return m_elapsedDay; }

	// 時間帯を取得
	const int GetElapsedTimezone() const { return m_elapsedTimezone; }

	// 経過秒を取得
	const float GetElapsedSecond() const { return m_elapsedSecond; }

	// 経過日数を指定分進める
	void AddElapsedDay(int addDay = 1) { m_elapsedDay = Math::Clamp(m_elapsedDay + addDay, InitialDay, MaxDay); }

	// 時間帯を指定分進める
	void AddElapsedTimezone(int addTimezone = 1) { m_elapsedTimezone = Math::Clamp(m_elapsedTimezone + addTimezone, InitialTimezone, MaxTimezone); }

	// 経過秒を指定分進める
	void AddElapsedSecond(float addSecond) { m_elapsedSecond = Math::Clamp(m_elapsedSecond + addSecond, InitialSecond, MaxSecond); }

	// 経過日数を初期化
	void DayInitialization() { m_elapsedDay = InitialDay; }

	// 時間帯を初期化
	void TimezoneInitialization() { m_elapsedTimezone = InitialTimezone; }

	// 経過秒を初期化
	void SecondInitialization() { m_elapsedSecond = InitialSecond; }
};