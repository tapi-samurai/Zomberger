#pragma once
#include "UiMessageBase.h"
#include "Vector2.h"
#include <string>

class UiTextMessage : public UiMessageBase
{
private:
	static constexpr Vector2 BasePosition = Vector2(240, 459);
	static constexpr int TextWidth = 320;
	static constexpr int TextHeight = 15;
	static constexpr int TextMargin = 3;

	const char* StageText1 = "‚±‚Ìæ‚Å ";
	const char* StageText2 = " ‚ªè‚É“ü‚è‚»‚¤‚¾...";
	const char* ReturnCampText = "‚±‚±‚©‚ç‹A‚ê‚»‚¤‚¾...";
	const char* BedText = "Q‚éB(‹ó• “x‚ª‚P‚³‚ª‚é)";

	int m_nextStageNum;
	int m_fontHandle;
	bool m_dispStageText;
	bool m_dispBedText;

	std::string MakeStageText();
	std::string MakeBedText();

protected:
	void Load() override;
	void Release() override;
	void Draw() override;

public:
	UiTextMessage() :
		m_nextStageNum(0),
		m_fontHandle(0),
		m_dispStageText(false),
		m_dispBedText(false) {}

	void OnDispStageText(int nextStageNum);
	void OnDispBedText();
};