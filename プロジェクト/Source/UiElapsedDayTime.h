#pragma once
#include "Node.h"

class UiElapsedDayTime : public Node
{
private:
	static constexpr int Width = 192;				// 画像を表示する横幅
	static constexpr int BaseX = 120;				// 基点X座標
	static constexpr int BaseY = 110;				// 基点Y座標
	static constexpr int DestX = BaseX - Width / 2;	// カレンダーを表示するX座標
	static constexpr int DestY = BaseY - Width / 2;	// カレンダーを表示するY座標
	static constexpr int NumY = 62;					// 数字を表示するY座標

	int m_calendarHandle;
	int m_fontHandle;

protected:
	void Load() override;
	void Release() override;
	void Draw() override;

public:
	UiElapsedDayTime() :
		m_calendarHandle(0),
		m_fontHandle(0) {};
};