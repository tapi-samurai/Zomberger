#pragma once
#include "Node.h"
#include "Vector2.h"
#include "DxLib.h"

class UiSatietyLevel : public Node
{
private:
	static constexpr Vector2 BasePosition = Vector2(240, 503);
	static constexpr int GridHeight = 16;
	static constexpr int GridWidth = 64;

	const unsigned int m_barColor;
	const unsigned int m_barLessColor;
	const unsigned int m_gridColor;

protected:
	void Draw() override;

public:
	UiSatietyLevel() :
		m_barColor(GetColor(255, 162, 110)),
		m_barLessColor(GetColor(105, 12, 0)),
		m_gridColor(GetColor(255, 255, 255)) {};
};