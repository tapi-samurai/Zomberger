#include "UiSatietyLevel.h"
#include "PlayerState.h"

void UiSatietyLevel::Draw()
{
	Node::Draw();

	// �󕠓x�\��
	for (int widthNum = 0; widthNum < PlayerState::MaxSatietyLevel; widthNum++)
	{
		int x1 = static_cast<int>(BasePosition.x) + GridWidth * widthNum;
		int y1 = static_cast<int>(BasePosition.y);
		int x2 = x1 + GridWidth;
		int y2 = y1 + GridHeight;
		unsigned int color = widthNum + 1 <= PlayerState::GetInstance()->GetSatietyLevel() ? m_barColor : m_barLessColor;

		// �̗͂�\��
		DrawBox(x1, y1, x2, y2, color, true);

		// �O�g�̕\��
		DrawBox(x1, y1, x2, y2, m_gridColor, false);
	}
}