#pragma once
#include "InteractiveObjectBase.h"
#include "Vector3.h"

class UiItemFieldBase;
class UiInventory;
class UiInteractable;
class ModelDisplay;

class UiSwitchObject : public InteractiveObjectBase
{
private:
	static constexpr Vector3 ExclamationPositionOffset = Vector3(0, 10, 0);			// �I�u�W�F�N�g�̏�ɕ\������u�I�v�̍��W�I�t�Z�b�g
	static constexpr Vector3 ExclamationScale = Vector3(0.015f, 0.015f, 0.015f);	// �u�I�v�̊g�k
	static constexpr float ExclamationSwingRate = 3.0f;								// �u�I�v�̗h��鑬�x

	static constexpr Vector3 UiPositionOffset = Vector3(0, 15, 0);

	UiItemFieldBase* const m_uiItemFieldBase;
	UiInventory* const m_uiInventory;
	UiInteractable* const m_uiInteractable;
	ModelDisplay* m_exclamation;

	float m_elapsedTime;

protected:
	void Update() override;
	void Draw() override;

	void OnInteract() override;

public:
	UiSwitchObject(
		UiItemFieldBase* const uiItemFieldBase, UiInventory* const uiInventory, UiInteractable* const uiInteractable,
		const Vector3& position, const Vector3& scale, float colliderRadius
	);
};