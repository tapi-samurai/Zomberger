#include "Fade.h"
#include "Time.h"
#include "DxLib.h"

Fade::Fade() :
	m_phase(Phase::None),
	m_elapsedTime(0),
	m_duration(0),
	m_beginBright(0),
	m_endBright(0)
{
}

void Fade::Update(int screen)
{
	// フェード中じゃなければ何もしない
	if (m_phase == Phase::None) return;

	// 経過時間と所要時間から0.0～1.0の割合を算出
	m_elapsedTime += Time::GetInstance()->GetDeltaTime();
	float t = m_elapsedTime / m_duration;
	if (t >= 1.0f)
	{
		t = 1.0f;
		m_phase = Phase::None;
	}

	// ↑で出した割合を使って線形補間
	int bright = static_cast<int>(m_beginBright * (1.0f - t) + m_endBright * t);

	// 引数で受け取ったスクリーンに対して明度を設定
	GraphFilter(screen, DX_GRAPH_FILTER_HSB, 0, 0, 0, bright);
}

void Fade::StartFadeIn(float duration, Color color)
{
	m_phase = Phase::Fade;
	m_elapsedTime = 0;
	m_duration = duration;
	m_beginBright = (color == Color::Black) ? Black : White;
	m_endBright = Normal;
}

void Fade::StartFadeOut(float duration, Color color)
{
	m_phase = Phase::Fade;
	m_elapsedTime = 0;
	m_duration = duration;
	m_beginBright = Normal;
	m_endBright = (color == Color::Black) ? Black : White;
}
