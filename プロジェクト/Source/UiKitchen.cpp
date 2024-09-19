#include "UiKitchen.h"
#include "ItemData.h"
#include "ItemField.h"
#include "input.h"
#include "SpriteLoader.h"
#include "Button.h"
#include "Sprite.h"
#include "SoundLoader.h"
#include "SoundEffect.h"
#include "SoundData.h"
#include "TextureData.h"

UiKitchen::UiKitchen() :
	UiItemFieldBase(SlotHeightNum, SlotWidthNum),
	m_cookButton(Button(Vector2(ButtonWidth, ButtonHeight), MOUSE_INPUT_LEFT, std::bind(&UiKitchen::OnClick, this))),
	m_selectItemNum(static_cast<int>(ItemData::RecipeItem::Null)),
	m_seClick(0),
	m_seCook(0)
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
		int itemAddressArrayHeightNum = itemboxNum / ItemboxWidth + 1;
		int itemAddressArrayWidthNum = itemboxNum % ItemboxWidth;

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

	// ハンバーガースロット
	// 右一列
	for (
		int hamburgerSlotNum = 0;
		hamburgerSlotNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::HamburgerRecipe)];
		hamburgerSlotNum++
		)
	{
		m_itemAddressArray[hamburgerSlotNum][SlotWidthNum - 1] =
			ItemField::GetInstance()->GetItemFieldAddress(ItemField::FieldName::HamburgerRecipe, hamburgerSlotNum);
	}

	// 基点座標を設定
	m_basePositionTopLeft = BasePosition;

	// ボタン画像を登録
	m_spriteButton = new Sprite();
	m_spriteButton->Register(TextureData::List[static_cast<int>(TextureData::Name::CookButton)].TextureName);
}

void UiKitchen::Load()
{
	UiItemFieldBase::Load();

	m_spriteButton->Load();

	// SE読み込み
	m_seClick = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].SoundName);
	m_seCook = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::Cook)].SoundName);

	// SE設定
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seClick, SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].volume);
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seCook, SoundData::List[static_cast<int>(SoundData::Name::Cook)].volume);
}

void UiKitchen::Release()
{
	UiItemFieldBase::Release();

	m_spriteButton->Release();

	// SE削除
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].SoundName);
}

void UiKitchen::Update()
{
	Node::Update();

	if (IsActive() == false) return;

	m_cookButton.Update(ButtonPosition);
	m_spriteButton->Update();

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

void UiKitchen::Draw()
{
	if (IsActive() == false) return;

	m_cookButton.Draw(ButtonPosition);
	m_spriteButton->Draw(ButtonPosition);

	UiItemFieldBase::Draw();

	// 描画する画像の明るさを暗くする
	SetDrawBright(HintItemBright, HintItemBright, HintItemBright);

	// ハンバーガーに必要なアイテムをボックスに暗く表示
	for (
		int hamburgerSlotNum = 0;
		hamburgerSlotNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::HamburgerRecipe)];
		hamburgerSlotNum++
		)
	{
		// 既に中身がセットされていたら描画しない
		if (*m_itemAddressArray[hamburgerSlotNum][SlotWidthNum - 1] != static_cast<int>(ItemData::RecipeItem::Null)) continue;

		int dispPosX = static_cast<int>(m_basePositionTopLeft.x + (SlotWidthNum - 1) * SlotWidthLength);
		int dispPosY = static_cast<int>(m_basePositionTopLeft.y + hamburgerSlotNum * SlotHeightLength);

		DrawGraph(dispPosX, dispPosY, m_itemTextureHandleArray[hamburgerSlotNum + 1], true);
	}

	// 描画する画像の明るさを標準に戻す
	SetDrawBright(255, 255, 255);

	// 現在掴んでいるアイテムをマウスカーソルの位置に表示
	if (m_selectItemNum != static_cast<int>(ItemData::RecipeItem::Null))
	{
		int dispPosX = static_cast<int>(Input::GetInstance()->GetMousePoint().x - SlotWidthLength / 2.0f);
		int dispPosY = static_cast<int>(Input::GetInstance()->GetMousePoint().y - SlotHeightLength / 2.0f);

		DrawGraph(dispPosX, dispPosY, m_itemTextureHandleArray[m_selectItemNum], true);
	}
}

void UiKitchen::OnClick()
{
	// ハンバーガーのレシピが揃っているかチェック
	for (
		int hamburgerNum = 0;
		hamburgerNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::HamburgerRecipe)];
		hamburgerNum++
		)
	{
		// レシピが揃っていなければスキップ
		if (*m_itemAddressArray[hamburgerNum][SlotWidthNum - 1] != hamburgerNum + 1) return;
	}

	// すべて揃っていたらインベントリにハンバーガーを追加
	int hamburger = static_cast<int>(ItemData::OtherItem::Hamburger);
	if (
		ItemField::GetInstance()->AddItem(
			ItemField::FieldName::Itembox, hamburger)
		)
	{
		// 追加出来たらレシピ欄のアイテムを全消去
		for (
			int widthNum = 0;
			widthNum < ItemField::ItemFieldSize[static_cast<int>(ItemField::FieldName::HamburgerRecipe)];
			widthNum++
			)
		{
			ItemField::GetInstance()->ReduceItem(ItemField::FieldName::HamburgerRecipe, widthNum);
		}
	}

	// SE
	PlaySoundMem(m_seCook, DX_PLAYTYPE_BACK);
}

void UiKitchen::ReleaseItem()
{
	if (m_selectItemNum == static_cast<int>(ItemData::RecipeItem::Null)) return;

	// アイテムを移せる場所を探す
	for (int height = 0; height < SlotHeightNum; height++)
	{
		for (int width = 0; width < SlotWidthNum; width++)
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