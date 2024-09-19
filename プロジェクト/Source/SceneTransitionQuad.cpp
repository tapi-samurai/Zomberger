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
	// ���W�Ɖ�]��ݒ�
	m_transform.position = position;
	m_transform.rotation = rotation;

	// �J�ڐ�̃X�e�[�W�ԍ���ێ�
	m_nextStageNum = stageType;

	// �R���C�_�[��ݒ�
	m_collider = new QuadCollider3D(Vector3(), height, width);

	// ���𐶐�
	m_modelDisplay = new ModelDisplay(
		"Arrow", ModelData::List[static_cast<int>(ModelData::Name::Arrow)].ModelName,
		position, rotation * Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(ArrowAngleY)), ArrowScale
	);

	AddChild(m_modelDisplay);

	// �J�ڐ�̃X�e�[�W������̐F���w��
	m_color = StageData::List[m_nextStageNum].Color;
}

void SceneTransitionQuad::Update()
{
	InteractiveObjectBase::Update();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	// ����O��Ɉړ�
	m_modelDisplay->SetPosition(
		m_transform.position + m_transform.Forward() * std::sin(m_elapsedTime * ArrowSwingRate) +
		m_transform.Forward() * ArrowPositionOffsetForward
	);
}

void SceneTransitionQuad::Draw()
{
	InteractiveObjectBase::Draw();

	// ���̐F��ύX
	MV1SetDifColorScale(m_modelDisplay->GetModelHandle(), m_color);

	// �v���C���[���߂��ɂ��鎞�̏���
	if (m_isNear)
	{
		// UI�̕\��
		m_uiInteractable->OnDisp(ConvWorldPosToScreenPos(m_playerPos + UiPositionOffset));

		// �J�ڐ�̃X�e�[�W�Ŏ�ɓ���₷���A�C�e����\������UI�̕`��
		m_uiTextMessage->OnDispStageText(m_nextStageNum);
	}
}

void SceneTransitionQuad::OnInteract()
{
	// �X�e�[�W�J�ڗp�̃|�C���^�Ɏ��X�e�[�W�̒l�����A�h���X����
	m_nextStagePointer = &m_nextStageNum;
}