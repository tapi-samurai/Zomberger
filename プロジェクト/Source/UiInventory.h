#pragma once
#include "UiItemFieldBase.h"
#include "Sprite.h"
#include "Screen.h"

class UiInventory : public UiItemFieldBase
{
private:
	static constexpr Vector2 BasePosition = Vector2(240, 520);

	static constexpr int HeightNum = 1;
	static constexpr int WidthNum = 5;

protected:
	void Update() override;

public:
	UiInventory();
};