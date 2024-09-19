#pragma once

// 時間管理クラス
class Time
{
private:
	int m_prevTime;		// 前フレームの時刻
	float m_deltaTime;	// 前フレームからの経過時間

	Time();	// コンストラクタ

public:
	float timeScale;	// 経過時間の倍率

	// シングルトン
	static Time* GetInstance()
	{
		static Time instance;
		return &instance;
	}

	void Update();	// 更新
	float GetDeltaTime(){ return m_deltaTime; }	// 前フレームからの経過時間を取得
	float GetLocalDeltaTime() { return m_deltaTime * timeScale; }	// ローカルな経過時間を取得
};
