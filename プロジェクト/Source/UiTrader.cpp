#include "UiTrader.h"
#include "Button.h"
#include "DxLib.h"
#include "ItemData.h"
#include "ItemField.h"
#include "Sprite.h"
#include "Input.h"
#include "SoundLoader.h"
#include "SoundEffect.h"
#include "SoundData.h"
#include "TextureData.h"

UiTrader::UiTrader() :
	UiItemFieldBase(SlotHeightNum, SlotWidthNum),
	m_tradeButton(Button(Vector2(ButtonWidth, ButtonHeight), MOUSE_INPUT_LEFT, std::bind(&UiTrader::OnClick, this))),
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
		int itemAddressArrayHeightNum = itemboxNum / ItemboxWidth;
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

	// 交換枠
	// 右下
	m_itemAddressArray[m_itemAddressArray.size() - 1][m_itemAddressArray[0].size() - 1] =
		ItemField::GetInstance()->GetItemFieldAddress(ItemField::FieldName::Trader, 0);

	// 基点座標を設定
	m_basePositionTopLeft = BasePosition;

	// ボタン画像を登録
	m_spriteButton = new Sprite();
	m_spriteButton->Register(TextureData::List[static_cast<int>(TextureData::Name::TradeButton)].TextureName);
}

void UiTrader::Load()
{
	UiItemFieldBase::Load();

	m_spriteButton->Load();

	// SE読み込み
	m_seClick = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].SoundName);

	// SE設定
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seClick, SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].volume);
}

void UiTrader::Release()
{
	UiItemFieldBase::Release();

	m_spriteButton->Release();

	// SE削除
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ItemClick)].SoundName);
}

void UiTrader::Update()
{
	Node::Update();

	if (IsActive() == false) return;

	// ボタンの更新処理
	m_tradeButton.Update(ButtonPosition);
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

void UiTrader::Draw()
{
	if (IsActive() == false) return;

	// ボタンの描画処理
	m_tradeButton.Draw(ButtonPosition);
	m_spriteButton->Draw(ButtonPosition);

	UiItemFieldBase::Draw();

	// 描画する画像の明るさを暗くする
	SetDrawBright(HintItemBright, HintItemBright, HintItemBright);

	// 交換に必要なアイテムをボックスに暗く表示
	if (
		*m_itemAddressArray[m_itemAddressArray.size() - 1][m_itemAddressArray[m_itemAddressArray.size() - 1].size() - 1] ==
		static_cast<int>(ItemData::RecipeItem::Null)
		)
	{
		int dispPosX =
			static_cast<int>(m_basePositionTopLeft.x + (m_itemAddressArray[m_itemAddressArray.size() - 1].size() - 1) * SlotWidthLength);
		int dispPosY =
			static_cast<int>(m_basePositionTopLeft.y + (m_itemAddressArray.size() - 1) * SlotHeightLength);

		DrawGraph(dispPosX, dispPosY, m_itemTextureHandleArray[static_cast<int>(ItemData::OtherItem::Hamburger)], true);
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

void UiTrader::OnClick()
{
	// 交換に必要なアイテムが入っていなければスキップ
	if (
		*m_itemAddressArray[m_itemAddressArray.size() - 1][m_itemAddressArray[m_itemAddressArray.size() - 1].size() - 1] !=
		static_cast<int>(ItemData::OtherItem::Hamburger)
		)
	{
		return;
	}

	// アイテムが入っている場合はスコップに置き換える
	ItemField::GetInstance()->ReduceItem(ItemField::FieldName::Trader, 0);
	ItemField::GetInstance()->AddItem(ItemField::FieldName::Trader, static_cast<int>(ItemData::OtherItem::Scoop));
}