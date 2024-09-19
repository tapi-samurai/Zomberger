#include "UiItembox.h"
#include "ItemField.h"
#include "ItemData.h"
#include "Input.h"
#include "Screen.h"
#include "SoundLoader.h"
#include "SoundData.h"
#include "SoundEffect.h"

UiItembox::UiItembox() :
	UiItemFieldBase(HeightNum, WidthNum),
	m_selectItemNum(static_cast<int>(ItemData::RecipeItem::Null)),
	m_seClick(0)
{
	// 配列にインベントリのアドレスを追加

	// アイテムボックスを登録
	// 縦横2×5
	for (
		int itemboxNum = 0;
		itemboxNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::Itembox)];
		itemboxNum++
		)
	{
		int itemAddressArrayHeightNum = itemboxNum / WidthNum;
		int itemAddressArrayWidthNum = itemboxNum % WidthNum;

		m_itemAddressArray[itemAddressArrayHeightNum][itemAddressArrayWidthNum] =
			ItemField::GetInstance()->GetItemFieldAddress(ItemField::FieldName::Itembox, itemboxNum);
	}

	// インベントリ
	// 最下段
	for (
		int inventoryNum = 0;
		inventoryNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::Inventory)];
		inventoryNum++
		)
	{
		m_itemAddressArray[m_itemAddressArray.size() - 1][inventoryNum] =
			ItemField::GetInstance()->GetItemFieldAddress(ItemField::FieldName::Inventory, inventoryNum);
	}

	// 基点座標を設定
	m_basePositionTopLeft = BasePosition;
}

void UiItembox::Load()
{
	UiItemFieldBase::Load();

	// SE読み込み
	m_seClick = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].SoundName);

	// SE設定
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seClick, SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].volume);
}

void UiItembox::Release()
{
	UiItemFieldBase::Release();

	// SE削除
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].SoundName);
}

void UiItembox::Update()
{
	Node::Update();

	if (IsActive() == false) return;
	
	// マウスの座標を基点に対するローカル座標に変換
	Vector2 mousePos = Input::GetInstance()->GetMousePoint();
	Vector2 localMousePos = Vector2(mousePos.x - BasePosition.x, mousePos.y - BasePosition.y);

	// マウスの座標をスロット番号に変換
	int selectHeightNum = static_cast<int>(localMousePos.y / SlotHeightLength + (localMousePos.y < 0 ? -1 : 0));
	int selectWidthNum = static_cast<int>(localMousePos.x / SlotWidthLength + (localMousePos.x < 0 ? -1 : 0));

	// セレクト枠を移動
	MoveSelectSlot(selectHeightNum, selectWidthNum);

	// アイテムの移動
	if (
		ItemField::GetInstance()->GetSelectSlotNum() != ItemField::OutSlot &&
		Input::GetInstance()->IsMouseDown(MOUSE_INPUT_LEFT)
		)
	{
		int selectHeight = static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().y);
		int selectWidth = static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x);

		// 現在掴んでいるアイテムとセレクト枠のアイテムを切り替える
		int selectItemBaseCopy = m_selectItemNum;
		m_selectItemNum = *m_itemAddressArray[selectHeight][selectWidth];
		*m_itemAddressArray[selectHeight][selectWidth] = selectItemBaseCopy;

		// SE
		PlaySoundMem(m_seClick, DX_PLAYTYPE_BACK);
	}
}

void UiItembox::Draw()
{
	if (IsActive() == false) return;

	UiItemFieldBase::Draw();

	// 現在掴んでいるアイテムをマウスカーソルの位置に表示
	if (m_selectItemNum != static_cast<int>(ItemData::RecipeItem::Null))
	{
		int dispPosX = static_cast<int>(Input::GetInstance()->GetMousePoint().x - SlotWidthLength / 2.0f);
		int dispPosY = static_cast<int>(Input::GetInstance()->GetMousePoint().y - SlotHeightLength / 2.0f);

		DrawGraph(dispPosX, dispPosY, m_itemTextureHandleArray[m_selectItemNum], true);
	}
}

void UiItembox::ReleaseItem()
{
	if (m_selectItemNum == static_cast<int>(ItemData::RecipeItem::Null)) return;

	// アイテムを移せる場所を探す
	for (int height = 0; height < HeightNum; height++)
	{
		for (int width = 0; width < WidthNum; width++)
		{
			if (
				m_itemAddressArray[height][width] == nullptr ||
				*m_itemAddressArray[height][width] != static_cast<int>(ItemData::RecipeItem::Null)
				)
			{
				continue;
			}

			// アイテムを移す
			*m_itemAddressArray[height][width] = m_selectItemNum;

			// 掴んでいるアイテムをNullにする
			m_selectItemNum = static_cast<int>(ItemData::RecipeItem::Null);
		}
	}
}