#pragma once
#include "UiItemFieldBase.h"
#include "Button.h"

class Sprite;

class UiKitchen : public UiItemFieldBase
{
private:
	static constexpr Vector2 BasePosition = Vector2(64, 150);
	static constexpr Vector2 ButtonPosition = Vector2(640, 428);
	static constexpr int ButtonHeight = 64;
	static constexpr int ButtonWidth = 128;

	static constexpr int SlotHeightNum = 5;
	static constexpr int SlotWidthNum = 7;
	static constexpr int ItemboxWidth = 5;
	static constexpr int HintItemBright = 50;

	Button m_cookButton;
	Sprite* m_spriteButton;
	int m_selectItemNum;
	int m_seClick;
	int m_seCook;

	void OnClick();
	virtual void ReleaseItem() override;

protected:
	void Load() override;
	void Release() override;
	void Update() override;
	void Draw() override;

public:
	UiKitchen();
};