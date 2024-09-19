#pragma once
#include "UiItemFieldBase.h"

class UiItembox : public UiItemFieldBase
{
private:
	static constexpr Vector2 BasePosition = Vector2(240, 328);

	static constexpr int HeightNum = 4;
	static constexpr int WidthNum = 5;

	static constexpr Vector2 SelectItemOffset = Vector2(0, -20);

	int m_selectItemNum;
	int m_seClick;

	virtual void ReleaseItem() override;

protected:
	void Load() override;
	void Release() override;
	void Update() override;
	void Draw() override;

public:
	UiItembox();
};