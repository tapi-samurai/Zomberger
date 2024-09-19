#pragma once

class BgmPlayer
{
private:
	const char* m_bgmName;
	int m_bgmHandle;

public:
	static BgmPlayer* GetInstance()
	{
		static BgmPlayer instance;
		return &instance;
	}

	BgmPlayer() :
		m_bgmName(nullptr),
		m_bgmHandle(0) {};

	void Register(const char* bgmName);
	void Delete(const char* bgmName);
	void Play() const;
	void Stop() const;

	int GetBgmHandle() { return m_bgmHandle; }
};