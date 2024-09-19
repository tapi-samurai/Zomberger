#pragma once
#include "Node.h"
#include "Vector2.h"
#include "DxLib.h"

class UiHealth : public Node
{
private:
	static constexpr Vector2 BasePosition = Vector2(240, 483);
	static constexpr int GridHeight = 16;
	static constexpr int GridWidth = 64;

	const unsigned int m_barColor;
	const unsigned int m_barLessColor;
	const unsigned int m_gridColor;

protected:
	void Draw() override;

public:
	UiHealth() :
		m_barColor(GetColor(124, 239, 110)),
		m_barLessColor(GetColor(128, 0, 0)),
		m_gridColor(GetColor(255, 255, 255)) {};
};