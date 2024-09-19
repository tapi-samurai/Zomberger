#pragma once
#include "InteractiveObjectBase.h"

class ModelDisplay;
class UiInteractable;
class UiTextMessage;

class SceneTransitionQuad : public InteractiveObjectBase
{
private:
	static constexpr float ArrowPositionOffsetForward = -5.0f;			// ���̑O�������W�I�t�Z�b�g
	static constexpr Vector3 ArrowScale = Vector3(0.02f, 0.02f, 0.02f);
	static constexpr float ArrowSwingRate = 1.0f;						// ���̑O��ړ��̑��x�{��
	static constexpr float ArrowAngleY = 180.0f;

	static constexpr Vector3 UiPositionOffset = Vector3(0, 15, 0);	// �v���C���[�̓���ɕ\������UI�̍��W�I�t�Z�b�g

	ModelDisplay* m_modelDisplay;
	UiInteractable* const m_uiInteractable;
	UiTextMessage* const m_uiTextMessage;
	COLOR_F m_color;

	int m_nextStageNum;
	const int*& m_nextStagePointer;
	float m_elapsedTime;

protected:
	void Update() override;
	void Draw() override;

	void OnInteract() override;

public:
	SceneTransitionQuad(
		UiInteractable* const uiInteractable, UiTextMessage* const uiTextMessage, const int*& nextStagePointer,
		int stageType, const Vector3& position, const Quaternion& rotation, float height, float width
	);
};