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
	// ���W�A��]�A�g�k��ݒ�
	m_transform.position = position + Vector3(0, ModelData::List[static_cast<int>(ModelData::Name::Bed)].OffsetY, 0) * Scale.y;
	m_transform.rotation = rotation;
	m_transform.scale = Scale;

	// �����蔻��
	AddChild(new NoActionObject(
		position * Vector3(1, 0, 1) - Vector3(0, ColliderRadius / 2, 0),
		m_transform.rotation, m_transform.scale, "Object", nullptr, CollisionRadius)
	);

	// �u�I�v�I�u�W�F�N�g
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

	// SE�ǂݍ���
	m_seInteract = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::Bed)].SoundName);

	// SE�ݒ�
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seInteract, SoundData::List[static_cast<int>(SoundData::Name::Bed)].volume);
}

void SleepBed::Release()
{
	InteractiveObjectBase::Release();

	// SE�폜
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::Bed)].SoundName);
}

void SleepBed::Update()
{
	InteractiveObjectBase::Update();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	// �l���傫���Ȃ肷����̂�h��
	if (m_elapsedTime > Math::Fpi * 2)
	{
		m_elapsedTime -= Math::Fpi * 2;
	}

	// �u�I�v�I�u�W�F�N�g���㉺�ɗh�炷
	m_exclamation->SetPosition(
		m_transform.position + ExclamationPositionOffset + Vector3(0, std::sin(m_elapsedTime * ExclamationSwingRate), 0)
	);
}

void SleepBed::Draw()
{
	Actor3D::Draw();

	// �v���C���[���߂��ɂ��鎞�̏���
	if (m_isNear)
	{
		// UI�̕\��
		m_uiInteractable->OnDisp(ConvWorldPosToScreenPos(m_playerPos + UiPositionOffset));

		// �x�b�h�̐����e�L�X�g��\��
		m_uiTextMessage->OnDispBedText();
	}
}

void SleepBed::OnInteract()
{
	// �����蔻����폜����
	m_collider->isEnabled = false;

	// �o�ߓ�����i�߂�
	TimeProgressManager::GetInstance()->AddElapsedDay();

	// �X�e�[�W�J�ڗp�̃|�C���^�ɃL�����v�V�[���̃X�e�[�W�ԍ��A�h���X����
	m_nextStagePointer = &NextStageNum;

	// SE
	PlaySoundMem(m_seInteract, DX_PLAYTYPE_BACK);
}