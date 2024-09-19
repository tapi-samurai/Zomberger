#pragma once
#include "UiItemFieldBase.h"
#include "Button.h"

class Sprite;

class UiTrader : public UiItemFieldBase
{
private:
	static constexpr Vector2 BasePosition = Vector2(64, 204);
	static constexpr Vector2 ButtonPosition = Vector2(640, 428);
	static constexpr int ButtonHeight = 64;
	static constexpr int ButtonWidth = 128;

	static constexpr int SlotHeightNum = 4;
	static constexpr int SlotWidthNum = 7;
	static constexpr int ItemboxWidth = 5;
	static constexpr int HintItemBright = 50;

	Button m_tradeButton;
	Sprite* m_spriteButton;
	int m_selectItemNum;
	int m_seClick;

	void OnClick();

protected:
	void Load() override;
	void Release() override;
	void Update() override;
	void Draw() override;

public:
	UiTrader();
};