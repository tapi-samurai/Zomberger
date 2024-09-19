#include "LoadingScreen.h"
#include "TextureData.h"
#include "Time.h"

LoadingScreen::LoadingScreen()
{
	// �摜�̐ݒ�
	m_sprite.Register(TextureData::List[static_cast<int>(TextureData::Name::LoadingIcon)].TextureName);

	m_sprite.Load();
}

void LoadingScreen::Release()
{
	m_sprite.Release();
}

void LoadingScreen::Draw()
{
	m_sprite.Draw(IconBasePos);
}