#include "BgmPlayer.h"
#include "DxLib.h"
#include "SoundLoader.h"

void BgmPlayer::Register(const char* bgmName)
{
	// Œ»Ý“o˜^‚µ‚Ä‚¢‚é‹È‚Æ“¯‚¶‚È‚çƒXƒLƒbƒv
	if (m_bgmName == bgmName) return;

	// “o˜^‚µ‚Ä‚¢‚éBGM‚ðíœ
	if (m_bgmName != nullptr)
	{
		Delete(m_bgmName);
	}

	m_bgmHandle = SoundLoader::GetInstance()->Load(bgmName);
	m_bgmName = bgmName;
}

void BgmPlayer::Delete(const char* bgmName)
{
	// Œ»Ý“o˜^‚µ‚Ä‚¢‚é‹È‚Æˆá‚¤‚È‚çƒXƒLƒbƒv
	if (m_bgmName != bgmName) return;

	// BGMíœ
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