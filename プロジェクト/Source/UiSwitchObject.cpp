#include "UiSwitchObject.h"
#include "SphereCollider3D.h"
#include "UiItemFieldBase.h"
#include "Input.h"
#include "UiInventory.h"
#include "UiInteractable.h"
#include "NoActionObject.h"
#include "Time.h"
#include "Math.h"
#include "ModelDisplay.h"
#include "ModelData.h"

UiSwitchObject::UiSwitchObject(
	UiItemFieldBase* const uiItemFieldBase, UiInventory* const uiInventory, UiInteractable* const uiInteractable,
	const Vector3& position, const Vector3& scale, float colliderRadius
) :
	InteractiveObjectBase(new SphereCollider3D(Vector3(), colliderRadius)),
	m_uiItemFieldBase(uiItemFieldBase),
	m_uiInventory(uiInventory),
	m_uiInteractable(uiInteractable),
	m_elapsedTime(0)
{
	// 座標、回転、拡縮を設定
	m_transform.position = position + Vector3(0, colliderRadius, 0);
	m_transform.scale = scale;

	// 「！」オブジェクト
	m_exclamation = new ModelDisplay(
		"Exclamation",
		ModelData::List[static_cast<int>(ModelData::Name::Exclamation)].ModelName,
		m_transform.position + ExclamationPositionOffset, Quaternion::CreateIdentity(), Vector3(ExclamationScale)
	);
	AddChild(m_exclamation);
}

void UiSwitchObject::Update()
{
	// プレイヤーが離れたら非表示
	if (m_uiItemFieldBase->IsActive() && m_isNear == false)
	{
		// 表示切替
		m_uiItemFieldBase->SetActive(false);
		m_uiInventory->SetActive(true);
	}

	InteractiveObjectBase::Update();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	// 値が大きくなりすぎるのを防ぐ
	if (m_elapsedTime > Math::Fpi * 2)
	{
		m_elapsedTime -= Math::Fpi * 2;
	}

	// 「！」オブジェクトを上下に揺らす
	m_exclamation->SetPosition(
		m_transform.position + ExclamationPositionOffset + Vector3(0, std::sin(m_elapsedTime * ExclamationSwingRate), 0)
	);
}

void UiSwitchObject::Draw()
{
	Actor3D::Draw();

	// 非表示かつプレイヤーが近くにいたらUIを表示
	if (m_uiItemFieldBase->IsActive() == false && m_isNear)
	{
		m_uiInteractable->OnDisp(ConvWorldPosToScreenPos(m_playerPos + UiPositionOffset));
	}
}

void UiSwitchObject::OnInteract()
{
	// 毎フレーム処理が通るのを防ぐ
	if (Input::GetInstance()->IsKeyDown(KEY_INPUT_F) == false) return;

	// 表示切替
	m_uiItemFieldBase->SetActive(!m_uiItemFieldBase->IsActive());
	m_uiInventory->SetActive(!m_uiItemFieldBase->IsActive());
}