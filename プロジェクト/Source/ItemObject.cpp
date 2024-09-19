#include "ItemObject.h"
#include "ItemData.h"
#include "ModelData.h"
#include "SphereCollider3D.h"
#include "ItemField.h"
#include "ModelDisplay.h"
#include "Time.h"
#include "PlayerController.h"
#include "UiInteractItemGage.h"
#include "Math.h"

ItemObject::ItemObject(
	PlayerController* playerController, UiInteractItemGage* const uiInteractItemGage,
	const Vector3& position, const Quaternion& rotation, const char* itemName, int itemNum
) :
	InteractiveObjectBase(new SphereCollider3D(Vector3(), ColliderRadius), "Item"),
	m_playerController(playerController),
	m_uiInteractItemGage(uiInteractItemGage),
	m_itemNum(itemNum),
	m_elapsedTime(0),
	m_interactTime(0)
{
	// 座標と回転を設定
	m_transform.position = position + Vector3(0, ColliderRadius, 0);
	m_transform.rotation = rotation;

	// コライダーを設定
	m_collider = new SphereCollider3D(Vector3(), ColliderRadius);

	// モデル表示用のクラスを設定
	m_modelDisplay = new ModelDisplay(
		"-", ModelData::List[static_cast<int>(ModelData::Name::Exclamation)].ModelName,
		m_transform.position, m_transform.rotation, Vector3(ExclamationScale, ExclamationScale, ExclamationScale)
		);

	AddChild(m_modelDisplay);
}

void ItemObject::Update()
{
	// プレイヤーが近くにいなければインタラクトの進行状況を減らす
	if (m_isNear == false && m_interactTime >= 0)
	{
		float time = Time::GetInstance()->GetLocalDeltaTime();
		m_interactTime -= time > m_interactTime ? m_interactTime : time;
	}

	InteractiveObjectBase::Update();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	// 値が大きくなりすぎるのを防ぐ
	if (m_elapsedTime > Math::Fpi * 2)
	{
		m_elapsedTime -= Math::Fpi * 2;
	}

	// ビックリマークを上下させる
	m_modelDisplay->SetPosition(
		m_transform.position + ExclamationBaseOffset + Vector3(0, std::sin(m_elapsedTime * ExclamationSwingRate), 0)
	);
}

void ItemObject::Draw()
{
	InteractiveObjectBase::Draw();

	// インタラクトの進行状況を表示
	if (m_isNear)
	{
		// ゲージの表示座標を計算
		VECTOR screenPos = ConvWorldPosToScreenPos(m_transform.position + ExclamationBaseOffset + InteractUiOffset);

		// ゲージを表示
		m_uiInteractItemGage->OnDisp(screenPos, m_interactTime / GetItemSeconds);
	}
}

void ItemObject::OnInteract()
{
	// プレイヤーにインタラクトを通知
	m_playerController->OnInteract();

	// UIにインタラクトを通知
	m_uiInteractItemGage->IsInteract();

	m_interactTime += Time::GetInstance()->GetLocalDeltaTime();

	if (m_interactTime >= GetItemSeconds)
	{
		// インベントリに追加
		ItemField::GetInstance()->AddItem(ItemField::FieldName::Inventory, m_itemNum);

		// 自身を削除
		Destroy();
	}
}