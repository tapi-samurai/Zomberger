#pragma once

class Fade
{
private:
	static constexpr int Black = -255;	// 明度：黒
	static constexpr int White = 255;	// 明度：白
	static constexpr int Normal = 0;	// 明度：通常

	enum class Phase
	{
		None,	// 何もしていない
		Fade,	// フェード中
	};

	Phase m_phase;
	float m_elapsedTime;	// 経過時間
	float m_duration;		// 所要時間
	int m_beginBright;		// フェード開始時の明度
	int m_endBright;		// フェード終了時の明度

	// コンストラクタ
	Fade();

public:
	enum class Color
	{
		Black,
		White,
	};

	// シングルトン
	static Fade* GetInstance()
	{
		static Fade instance;
		return &instance;
	}

	// 更新
	void Update(int screen);

	// フェードイン
	void StartFadeIn(float duration = 0.5f, Color color = Color::Black);

	// フェードアウト
	void StartFadeOut(float duration = 0.5f, Color color = Color::Black);

	// フェード中かどうか
	bool IsFade() { return m_phase == Phase::Fade; }
};
