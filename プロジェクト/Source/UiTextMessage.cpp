#include "UiTextMessage.h"
#include "DxLib.h"
#include "StageData.h"
#include "ItemData.h"
#include "FontLoader.h"
#include "FontData.h"

void UiTextMessage::Load()
{
	UiMessageBase::Load();

	m_fontHandle = FontLoader::GetInstance()->Load(FontData::List[static_cast<int>(FontData::Name::CinecaptionText)]);
}

void UiTextMessage::Release()
{
	UiMessageBase::Release();

	FontLoader::GetInstance()->Delete(FontData::List[static_cast<int>(FontData::Name::CinecaptionText)]);
}

void UiTextMessage::Draw()
{
	if (m_isDisp == false) return;

	std::string text;

	if (m_dispStageText) text = MakeStageText();
	if (m_dispBedText) text = MakeBedText();

	// 字幕の背景を表示
	DrawBox(
		static_cast<int>(BasePosition.x - TextMargin), static_cast<int>(BasePosition.y - TextMargin),
		static_cast<int>(BasePosition.x + TextWidth + TextMargin), static_cast<int>(BasePosition.y + TextHeight + TextMargin),
		GetColor(0, 0, 0), true
	);

	// テキストを表示
	DrawFormatStringToHandle(
		static_cast<int>(BasePosition.x), static_cast<int>(BasePosition.y), GetColor(255, 255, 255),
		m_fontHandle, text.c_str()
	);

	m_isDisp = false;
	m_dispStageText = false;
	m_dispBedText = false;
}

std::string UiTextMessage::MakeStageText()
{
	std::string text;
	if (m_nextStageNum == static_cast<int>(StageData::Name::Camp))
	{
		text = ReturnCampText;
	}
	else
	{
		// 文字列をstringに変換して結合
		std::string text1 = StageText1;
		std::string itemName = ItemData::List[static_cast<int>(StageData::List[m_nextStageNum].FrequentItemName)].ItemName;
		std::string text2 = StageText2;

		text = text1 + itemName + text2;
	}

	return text;
}

std::string UiTextMessage::MakeBedText()
{
	return BedText;
}

void UiTextMessage::OnDispStageText(int nextStageNum)
{
	m_nextStageNum = nextStageNum;
	m_dispStageText = true;

	UiMessageBase::OnDisplay();
}

void UiTextMessage::OnDispBedText()
{
	m_dispBedText = true;

	UiMessageBase::OnDisplay();
}