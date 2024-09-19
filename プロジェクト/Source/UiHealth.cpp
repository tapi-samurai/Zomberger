#include "UiHealth.h"
#include "PlayerState.h"

void UiHealth::Draw()
{
	Node::Draw();

	// 体力表示
	for (int widthNum = 0; widthNum < PlayerState::MaxHealth; widthNum++)
	{
		// 座標を設定
		int x1 = static_cast<int>(BasePosition.x) + GridWidth * widthNum;
		int y1 = static_cast<int>(BasePosition.y);
		int x2 = x1 + GridWidth;
		int y2 = y1 + GridHeight;
		unsigned int color = widthNum + 1 <= PlayerState::GetInstance()->GetHealth() ? m_barColor : m_barLessColor;

		// 体力を表示
		DrawBox(x1, y1, x2, y2, color, true);

		// 外枠の表示
		DrawBox(x1, y1, x2, y2, m_gridColor, false);
	}
}