#include "UiElapsedDayTime.h"
#include "DxLib.h"
#include "TimeProgressManager.h"
#include "TextureData.h"
#include "SpriteLoader.h"
#include "FontData.h"

void UiElapsedDayTime::Load()
{
	Node::Load();

	// �摜�����[�h
	m_calendarHandle = SpriteLoader::GetInstance()->Load(TextureData::List[static_cast<int>(TextureData::Name::Calendar)].TextureName);

	// �t�H���g�f�[�^�̓ǂݍ���
	// �G�b�W��ݒ肵�����̂�Loader�N���X������ǂݍ���
	m_fontHandle = LoadFontDataToHandle(FontData::List[static_cast<int>(FontData::Name::CinecaptionNum)]);
}

void UiElapsedDayTime::Release()
{
	Node::Release();

	// �摜���폜
	SpriteLoader::GetInstance()->Delete(TextureData::List[static_cast<int>(TextureData::Name::Calendar)].TextureName);

	// �t�H���g���폜
	DeleteFontToHandle(m_fontHandle);
}

void UiElapsedDayTime::Draw()
{
	Node::Draw();

	// �o�ߓ����ɍ��킹�ăJ�����_�[��\��
	int elapsedDay = TimeProgressManager::GetInstance()->GetElapsedDay();
	int srcX = Width * (elapsedDay % 7);
	DrawRectGraph(DestX, DestY, srcX, 0, Width, Width, m_calendarHandle, true);

	// �o�ߓ�����\��
	int width = GetDrawFormatStringWidthToHandle(m_fontHandle, "%d", elapsedDay + 1);
	DrawFormatStringToHandle(BaseX - (width / 2), NumY, GetColor(60, 60, 60), m_fontHandle, "-%d", elapsedDay + 1);
}