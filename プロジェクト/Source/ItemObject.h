#pragma once
#include "InteractiveObjectBase.h"
#include "Vector3.h"

class ModelDisplay;
class PlayerController;
class UiInteractItemGage;

class ItemObject : public InteractiveObjectBase
{
private:
	// Transform
	static constexpr float ColliderRadius = 3.0f;

	// Control
	static constexpr float GetItemSeconds = 3.0f;	// アイテムを取得するために必要な時間

	// ビックリマーク
	static constexpr float ExclamationSwingRate = 2.0f;
	static constexpr float ExclamationScale = 0.02f;
	static constexpr Vector3 ExclamationBaseOffset = Vector3(0, 10, 0);
	static constexpr Vector3 InteractUiOffset = Vector3(0, 5, 0);

	ModelDisplay* m_modelDisplay;
	PlayerController* m_playerController;
	UiInteractItemGage* const m_uiInteractItemGage;

	int m_itemNum;
	float m_elapsedTime;
	float m_interactTime;

protected:
	void Update() override;
	void Draw() override;

	void OnInteract() override;

public:
	ItemObject(
		PlayerController* playerController, UiInteractItemGage* const uiInteractItemGage,
		const Vector3& position, const Quaternion& rotation, const char* itemName, int itemNum
	);
};