#include "SceneTransitionQuad.h"
#include "QuadCollider3D.h"
#include "BitFlag.h"
#include "StageData.h"
#include "ModelDisplay.h"
#include "ModelData.h"
#include "Math.h"
#include "Time.h"
#include "DxLib.h"
#include "UiInteractable.h"
#include "DxLib.h"
#include "UiTextMessage.h"

SceneTransitionQuad::SceneTransitionQuad(
	UiInteractable* const uiInteractable, UiTextMessage* const uiTextMessage, const int*& nextStagePointer,
	int stageType, const Vector3& position, const Quaternion& rotation, float height, float width
) :
	InteractiveObjectBase(new QuadCollider3D(Vector3(), height, width), "SceneTransitionQuad"),
	m_uiInteractable(uiInteractable),
	m_uiTextMessage(uiTextMessage),
	m_nextStageNum(stageType),
	m_nextStagePointer(nextStagePointer),
	m_elapsedTime(0)
{
	// 座標と回転を設定
	m_transform.position = position;
	m_transform.rotation = rotation;

	// 遷移先のステージ番号を保持
	m_nextStageNum = stageType;

	// コライダーを設定
	m_collider = new QuadCollider3D(Vector3(), height, width);

	// 矢印を生成
	m_modelDisplay = new ModelDisplay(
		"Arrow", ModelData::List[static_cast<int>(ModelData::Name::Arrow)].ModelName,
		position, rotation * Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(ArrowAngleY)), ArrowScale
	);

	AddChild(m_modelDisplay);

	// 遷移先のステージから矢印の色を指定
	m_color = StageData::List[m_nextStageNum].Color;
}

void SceneTransitionQuad::Update()
{
	InteractiveObjectBase::Update();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	// 矢印を前後に移動
	m_modelDisplay->SetPosition(
		m_transform.position + m_transform.Forward() * std::sin(m_elapsedTime * ArrowSwingRate) +
		m_transform.Forward() * ArrowPositionOffsetForward
	);
}

void SceneTransitionQuad::Draw()
{
	InteractiveObjectBase::Draw();

	// 矢印の色を変更
	MV1SetDifColorScale(m_modelDisplay->GetModelHandle(), m_color);

	// プレイヤーが近くにいる時の処理
	if (m_isNear)
	{
		// UIの表示
		m_uiInteractable->OnDisp(ConvWorldPosToScreenPos(m_playerPos + UiPositionOffset));

		// 遷移先のステージで手に入りやすいアイテムを表示するUIの描画
		m_uiTextMessage->OnDispStageText(m_nextStageNum);
	}
}

void SceneTransitionQuad::OnInteract()
{
	// ステージ遷移用のポインタに次ステージの値を持つアドレスを代入
	m_nextStagePointer = &m_nextStageNum;
}