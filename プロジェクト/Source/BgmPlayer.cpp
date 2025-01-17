#include "BgmPlayer.h"
#include "DxLib.h"
#include "SoundLoader.h"

void BgmPlayer::Register(const char* bgmName)
{
	// 現在登録している曲と同じならスキップ
	if (m_bgmName == bgmName) return;

	// 登録しているBGMを削除
	if (m_bgmName != nullptr)
	{
		Delete(m_bgmName);
	}

	m_bgmHandle = SoundLoader::GetInstance()->Load(bgmName);
	m_bgmName = bgmName;
}

void BgmPlayer::Delete(const char* bgmName)
{
	// 現在登録している曲と違うならスキップ
	if (m_bgmName != bgmName) return;

	// BGM削除
	SoundLoader::GetInstance()->Delete(m_bgmName);

	m_bgmName = nullptr;
	m_bgmHandle = 0;
}

void BgmPlayer::Play() const
{
	if (m_bgmHandle == 0) return;

	PlaySoundMem(m_bgmHandle, DX_PLAYTYPE_LOOP);
}

void BgmPlayer::Stop() const
{
	if (m_bgmHandle == 0) return;

	StopSoundMem(m_bgmHandle);
}