#include "BgmPlayer.h"
#include "DxLib.h"
#include "SoundLoader.h"

void BgmPlayer::Register(const char* bgmName)
{
	// ���ݓo�^���Ă���ȂƓ����Ȃ�X�L�b�v
	if (m_bgmName == bgmName) return;

	// �o�^���Ă���BGM���폜
	if (m_bgmName != nullptr)
	{
		Delete(m_bgmName);
	}

	m_bgmHandle = SoundLoader::GetInstance()->Load(bgmName);
	m_bgmName = bgmName;
}

void BgmPlayer::Delete(const char* bgmName)
{
	// ���ݓo�^���Ă���ȂƈႤ�Ȃ�X�L�b�v
	if (m_bgmName != bgmName) return;

	// BGM�폜
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