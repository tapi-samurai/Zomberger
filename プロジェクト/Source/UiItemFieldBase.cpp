#include "UiItemFieldBase.h"
#include "SpriteLoader.h"
#include "DxLib.h"
#include "ItemData.h"
#include "Math.h"
#include "ItemField.h"
#include "TextureData.h"

UiItemFieldBase::UiItemFieldBase(int slotHeightNum, int slotWidthNum) :
	m_basePositionTopLeft(Vector2()),
	m_slotHeightNum(slotHeightNum),
	m_slotWidthNum(slotWidthNum),
	m_slotTextureHandle(0),
	m_selectTextureHandle(0),
	m_isActive(false)
{
	// 表示するアイテムのフィールドアドレスを持つ二次元配列を定義
	m_itemAddressArray = std::vector<std::vector<int*>>(m_slotHeightNum, std::vector<int*>(m_slotWidthNum, nullptr));

	// アイテムの画像ハンドルを持つ配列を定義
	m_itemTextureHandleArray = std::vector<int>(static_cast<int>(ItemData::OtherItem::AllLength), 0);
}

void UiItemFieldBase::Load()
{
	Node::Load();

	// 画像のロード
	m_slotTextureHandle =
		SpriteLoader::GetInstance()->Load(TextureData::List[static_cast<int>(TextureData::Name::ItemSlot)].TextureName
		);
	m_selectTextureHandle =
		SpriteLoader::GetInstance()->Load(TextureData::List[static_cast<int>(TextureData::Name::SelectSlot)].TextureName
		);

	// 全てのアイテム画像をロード
	for (int itemNum = 0; itemNum < static_cast<int>(ItemData::OtherItem::AllLength); itemNum++)
	{
		m_itemTextureHandleArray[itemNum] = SpriteLoader::GetInstance()->Load(ItemData::List[itemNum].TextureName);
	}
}

void UiItemFieldBase::Release()
{
	Node::Release();

	// 画像の解放
	SpriteLoader::GetInstance()->Delete(TextureData::List[static_cast<int>(TextureData::Name::ItemSlot)].TextureName);
	SpriteLoader::GetInstance()->Delete(TextureData::List[static_cast<int>(TextureData::Name::SelectSlot)].TextureName);

	// 全てのアイテム画像を削除
	for (int itemNum = 0; itemNum < static_cast<int>(ItemData::OtherItem::AllLength); itemNum++)
	{
		SpriteLoader::GetInstance()->Delete(ItemData::List[itemNum].TextureName);
	}
}

void UiItemFieldBase::Draw()
{
	Node::Draw();

	if (m_isActive == false) return;

	// 枠の表示
	for (int heightNum = 0; heightNum < m_slotHeightNum; heightNum++)
	{
		for (int widthNum = 0; widthNum < m_slotWidthNum; widthNum++)
		{
			// 表示しない枠であればスキップ
			if (m_itemAddressArray[heightNum][widthNum] == nullptr) continue;

			// 表示する座標を計算
			Vector2 dispPos = m_basePositionTopLeft + Vector2(widthNum * SlotWidthLength, heightNum * SlotHeightLength);

			// 枠を表示
			DrawGraph(static_cast<int>(dispPos.x), static_cast<int>(dispPos.y), m_slotTextureHandle, true);

			// 中身があれば表示
			int* itemNum = m_itemAddressArray[heightNum][widthNum];
			if (itemNum != nullptr)
			{
				DrawGraph(static_cast<int>(dispPos.x), static_cast<int>(dispPos.y), m_itemTextureHandleArray[*itemNum], true);
			}
		}
	}

	// 現在選択しているスロットに画像を表示する
	// カーソルがスロット外であれば表示しない
	if (ItemField::GetInstance()->GetSelectSlotNum() == ItemField::OutSlot) return;

	// 表示する座標を計算
	Vector2 selectSlotNum = ItemField::GetInstance()->GetSelectSlotNum();
	Vector2 arrowDispPos = m_basePositionTopLeft +
		Vector2(selectSlotNum.x * SlotWidthLength, selectSlotNum.y * SlotWidthLength);

	// 画像を表示
	DrawGraph(static_cast<int>(arrowDispPos.x), static_cast<int>(arrowDispPos.y), m_selectTextureHandle, true);
}

// セレクト枠を移動(移動量を指定)
void UiItemFieldBase::MoveSelectSlot(const Vector2& input)
{
	if (input == Vector2()) return;

	float moveY = Math::Clamp(input.y, -1.0f, 1.0f);
	int count = 0;

	// 縦移動
	while (count < std::abs(input.y))
	{
		// 移動先のスロット番号を計算
		Vector2 selectSlotNum = ItemField::GetInstance()->GetSelectSlotNum();
		Vector2 nextSelectSlotNum = Vector2(selectSlotNum.x, selectSlotNum.y + moveY);

		// 移動先が枠外であれば終了
		if (nextSelectSlotNum.y < 0 || nextSelectSlotNum.y >= m_slotHeightNum)
		{
			break;
		}

		// 移動先がnullであればそのマスの１マス先をチェックする
		if (
			m_itemAddressArray[static_cast<int>(nextSelectSlotNum.y)][static_cast<int>(nextSelectSlotNum.x)] == nullptr
			)
		{
			moveY += moveY;
			continue;
		}

		// 移動量をリセットしてマスを移動
		ItemField::GetInstance()->SetSelectSlotNum(nextSelectSlotNum);
		moveY = Math::Clamp(input.y, -1.0f, 1.0f);
		count++;
	}

	float moveX = Math::Clamp(input.x, -1.0f, 1.0f);
	count = 0;

	// 横移動
	while (count < std::abs(input.x))
	{
		// 移動先のスロット番号を計算
		Vector2 selectSlotNum = ItemField::GetInstance()->GetSelectSlotNum();
		Vector2 nextSelectSlotNum = Vector2(selectSlotNum.x + moveX, selectSlotNum.y);

		// 移動先が枠外であれば終了
		if (nextSelectSlotNum.x < 0 || nextSelectSlotNum.x >= m_slotWidthNum)
		{
			break;
		}

		// 移動先がnullであればそのマスの１マス先をチェックする
		int nextSelectSlotNumX = static_cast<int>(nextSelectSlotNum.x);
		int nextSelectSlotNumY = static_cast<int>(nextSelectSlotNum.y);

		if (m_itemAddressArray[static_cast<int>(nextSelectSlotNum.y)][static_cast<int>(nextSelectSlotNum.x)] == nullptr)
		{
			moveX += moveX;
			continue;
		}

		// 移動量をリセットしてマスを移動
		ItemField::GetInstance()->SetSelectSlotNum(nextSelectSlotNum);
		moveX = Math::Clamp(input.x, -1.0f, 1.0f);
		count++;
	}
}

// セレクト枠を移動(直接移動先を指定)
void UiItemFieldBase::MoveSelectSlot(const int selectHeightNum, const int selectWidthNum)
{
	// セレクト枠が枠外であれば終了
	if (
		selectHeightNum < 0 || selectHeightNum >= m_slotHeightNum ||
		selectWidthNum < 0 || selectWidthNum >= m_slotWidthNum
		)
	{
		ItemField::GetInstance()->SetSelectSlotNum(ItemField::OutSlot);
		return;
	}

	// セレクト枠の中身が定義されていなければ終了
	if (m_itemAddressArray[selectHeightNum][selectWidthNum] == nullptr)
	{
		ItemField::GetInstance()->SetSelectSlotNum(ItemField::OutSlot);
		return;
	}

	ItemField::GetInstance()->SetSelectSlotNum(Vector2(selectWidthNum, selectHeightNum));
}

// 表示切替
const void UiItemFieldBase::SetActive(bool value)
{
	m_isActive = value;

	// 表示切替時に必ずセレクト枠を(0, 0)に戻す
	MoveSelectSlot(0, 0);

	// 掴んでいるアイテムを離す処理
	ReleaseItem();
}