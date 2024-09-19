#pragma once
#include "InteractiveObjectBase.h"
#include "Vector3.h"
#include "StageData.h"

class UiInteractable;
class ModelDisplay;
class UiTextMessage;

class SleepBed : public InteractiveObjectBase
{
private:
	static constexpr int NextStageNum = static_cast<int>(StageData::Name::Camp);
	static constexpr Vector3 Scale = Vector3(0.12f, 0.12f, 0.12f);
	static constexpr float ColliderRadius = 12.0f;									// インタラクト判定用コライダーの半径
	static constexpr float CollisionRadius = 10.0f;									// 衝突判定用コライダーの半径
	static constexpr Vector3 UiPositionOffset = Vector3(0, 15, 0);					// プレイヤーの頭上に表示するUIのプレイヤーに対するオフセット
	static constexpr Vector3 ExclamationPositionOffset = Vector3(0, 13, 0);			// オブジェクトの上に表示する「！」の座標オフセット
	static constexpr Vector3 ExclamationScale = Vector3(0.015f, 0.015f, 0.015f);	// 「！」の拡縮
	static constexpr float ExclamationSwingRate = 3.0f;								// 「！」の揺れる速度

	UiInteractable* const m_uiInteractable;
	UiTextMessage* const m_uiTextMessage;
	ModelDisplay* m_exclamation;

	const int*& m_nextStagePointer;
	float m_elapsedTime;
	int m_seInteract;

protected:
	void Load() override;
	void Release() override;
	void Update() override;
	void Draw() override;

	void OnInteract() override;

public:
	SleepBed(
		UiInteractable* const uiInteractable, UiTextMessage* const uiTextMessage,
		const Vector3& position, const Quaternion& rotation, const int*& nextStagePointer
	);
};