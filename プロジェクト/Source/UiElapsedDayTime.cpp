#include "UiElapsedDayTime.h"
#include "DxLib.h"
#include "TimeProgressManager.h"
#include "TextureData.h"
#include "SpriteLoader.h"
#include "FontData.h"

void UiElapsedDayTime::Load()
{
	Node::Load();

	// 画像をロード
	m_calendarHandle = SpriteLoader::GetInstance()->Load(TextureData::List[static_cast<int>(TextureData::Name::Calendar)].TextureName);

	// フォントデータの読み込み
	// エッジを設定したいのでLoaderクラスを介さず読み込み
	m_fontHandle = LoadFontDataToHandle(FontData::List[static_cast<int>(FontData::Name::CinecaptionNum)]);
}

void UiElapsedDayTime::Release()
{
	Node::Release();

	// 画像を削除
	SpriteLoader::GetInstance()->Delete(TextureData::List[static_cast<int>(TextureData::Name::Calendar)].TextureName);

	// フォントを削除
	DeleteFontToHandle(m_fontHandle);
}

void UiElapsedDayTime::Draw()
{
	Node::Draw();

	// 経過日数に合わせてカレンダーを表示
	int elapsedDay = TimeProgressManager::GetInstance()->GetElapsedDay();
	int srcX = Width * (elapsedDay % 7);
	DrawRectGraph(DestX, DestY, srcX, 0, Width, Width, m_calendarHandle, true);

	// 経過日数を表示
	int width = GetDrawFormatStringWidthToHandle(m_fontHandle, "%d", elapsedDay + 1);
	DrawFormatStringToHandle(BaseX - (width / 2), NumY, GetColor(60, 60, 60), m_fontHandle, "-%d", elapsedDay + 1);
}