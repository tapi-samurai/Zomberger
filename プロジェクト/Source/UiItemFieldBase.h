#pragma once
#include "Node.h"
#include "Vector2.h"
#include <vector>

class UiItemFieldBase : public Node
{
private:
	int m_slotHeightNum;		// 縦のスロット数(表示しない空白を含む)
	int m_slotWidthNum;			// 縦のスロット数(表示しない空白を含む)
	int m_slotTextureHandle;	// スロットの画像ハンドル
	int m_selectTextureHandle;	// セレクト枠の画像ハンドル

	bool m_isActive;

protected:
	static constexpr int SlotHeightLength = 64;	// スロットの縦の幅
	static constexpr int SlotWidthLength = 64;	// スロットの横の幅

	Vector2 m_basePositionTopLeft;		// 表示の基点座標(左上)

	std::vector<std::vector<int*>> m_itemAddressArray;	// 表示するアイテムのフィールドアドレスを持つ二次元配列
	std::vector<int> m_itemTextureHandleArray;			// アイテムの表示に使用するテクスチャの配列

	void Load() override;
	void Release() override;
	void Draw() override;

	virtual void Update() override = 0;
	virtual void ReleaseItem() {}

public:
	UiItemFieldBase(int slotHeightNum, int slotWidthNum);

	void MoveSelectSlot(const Vector2& input);									// セレクト枠を移動(移動量を指定)
	void MoveSelectSlot(const int selectHeightNum, const int selectWidthNum);	// セレクト枠を移動(直接移動先を指定)

	const void SetActive(bool value);
	const bool IsActive() const { return m_isActive; }
};