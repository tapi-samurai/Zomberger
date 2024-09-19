#include "SleepBed.h"
#include "SphereCollider3D.h"
#include "ModelData.h"
#include "TimeProgressManager.h"
#include "UiInteractable.h"
#include "NoActionObject.h"
#include "ModelDisplay.h"
#include "Time.h"
#include "SoundLoader.h"
#include "SoundData.h"
#include "SoundEffect.h"
#include "UiTextMessage.h"

SleepBed::SleepBed(
	UiInteractable* const uiInteractable, UiTextMessage* const uiTextMessage,
	const Vector3& position, const Quaternion& rotation, const int*& nextStagePointer
) :
	InteractiveObjectBase(
		new SphereCollider3D(Vector3(), ColliderRadius), "Bed", ModelData::List[static_cast<int>(ModelData::Name::Bed)].ModelName
	),
	m_uiInteractable(uiInteractable),
	m_uiTextMessage(uiTextMessage),
	m_nextStagePointer(nextStagePointer),
	m_elapsedTime(0),
	m_seInteract(0)
{
	// 座標、回転、拡縮を設定
	m_transform.position = position + Vector3(0, ModelData::List[static_cast<int>(ModelData::Name::Bed)].OffsetY, 0) * Scale.y;
	m_transform.rotation = rotation;
	m_transform.scale = Scale;

	// 当たり判定
	AddChild(new NoActionObject(
		position * Vector3(1, 0, 1) - Vector3(0, ColliderRadius / 2, 0),
		m_transform.rotation, m_transform.scale, "Object", nullptr, CollisionRadius)
	);

	// 「！」オブジェクト
	m_exclamation = new ModelDisplay(
		"Exclamation",
		ModelData::List[static_cast<int>(ModelData::Name::Exclamation)].ModelName,
		m_transform.position + ExclamationPositionOffset, Quaternion::CreateIdentity(), Vector3(ExclamationScale)
	);
	AddChild(m_exclamation);
}

void SleepBed::Load()
{
	InteractiveObjectBase::Load();

	// SE読み込み
	m_seInteract = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::Bed)].SoundName);

	// SE設定
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seInteract, SoundData::List[static_cast<int>(SoundData::Name::Bed)].volume);
}

void SleepBed::Release()
{
	InteractiveObjectBase::Release();

	// SE削除
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::Bed)].SoundName);
}

void SleepBed::Update()
{
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

void SleepBed::Draw()
{
	Actor3D::Draw();

	// プレイヤーが近くにいる時の処理
	if (m_isNear)
	{
		// UIの表示
		m_uiInteractable->OnDisp(ConvWorldPosToScreenPos(m_playerPos + UiPositionOffset));

		// ベッドの説明テキストを表示
		m_uiTextMessage->OnDispBedText();
	}
}

void SleepBed::OnInteract()
{
	// 当たり判定を削除する
	m_collider->isEnabled = false;

	// 経過日数を進める
	TimeProgressManager::GetInstance()->AddElapsedDay();

	// ステージ遷移用のポインタにキャンプシーンのステージ番号アドレスを代入
	m_nextStagePointer = &NextStageNum;

	// SE
	PlaySoundMem(m_seInteract, DX_PLAYTYPE_BACK);
}