#pragma once
#include "InteractiveObjectBase.h"

class ModelDisplay;
class UiInteractable;
class UiTextMessage;

class SceneTransitionQuad : public InteractiveObjectBase
{
private:
	static constexpr float ArrowPositionOffsetForward = -5.0f;			// 矢印の前方向座標オフセット
	static constexpr Vector3 ArrowScale = Vector3(0.02f, 0.02f, 0.02f);
	static constexpr float ArrowSwingRate = 1.0f;						// 矢印の前後移動の速度倍率
	static constexpr float ArrowAngleY = 180.0f;

	static constexpr Vector3 UiPositionOffset = Vector3(0, 15, 0);	// プレイヤーの頭上に表示するUIの座標オフセット

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