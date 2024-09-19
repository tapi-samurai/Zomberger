#include "Time.h"
#include "DxLib.h"

// コンストラクタ
Time::Time() :
	m_prevTime(GetNowCount()),
	m_deltaTime(0),
	timeScale(1)
{
}

void Time::Update()
{
	// 現在時刻（単位 ミリ秒）を取得 (正確には現在時刻ではない)
	int nowTime = GetNowCount();

	// 前フレームの時刻と現在時刻から経過時間を求める (単位は秒)
	m_deltaTime = (nowTime - m_prevTime) / 1000.0f;

	// 現在時刻を覚えておく
	m_prevTime = nowTime;
}
