#include "ItemObject.h"
#include "ItemData.h"
#include "ModelData.h"
#include "SphereCollider3D.h"
#include "ItemField.h"
#include "ModelDisplay.h"
#include "Time.h"
#include "PlayerController.h"
#include "UiInteractItemGage.h"
#include "Math.h"

ItemObject::ItemObject(
	PlayerController* playerController, UiInteractItemGage* const uiInteractItemGage,
	const Vector3& position, const Quaternion& rotation, const char* itemName, int itemNum
) :
	InteractiveObjectBase(new SphereCollider3D(Vector3(), ColliderRadius), "Item"),
	m_playerController(playerController),
	m_uiInteractItemGage(uiInteractItemGage),
	m_itemNum(itemNum),
	m_elapsedTime(0),
	m_interactTime(0)
{
	// ���W�Ɖ�]��ݒ�
	m_transform.position = position + Vector3(0, ColliderRadius, 0);
	m_transform.rotation = rotation;

	// �R���C�_�[��ݒ�
	m_collider = new SphereCollider3D(Vector3(), ColliderRadius);

	// ���f���\���p�̃N���X��ݒ�
	m_modelDisplay = new ModelDisplay(
		"-", ModelData::List[static_cast<int>(ModelData::Name::Exclamation)].ModelName,
		m_transform.position, m_transform.rotation, Vector3(ExclamationScale, ExclamationScale, ExclamationScale)
		);

	AddChild(m_modelDisplay);
}

void ItemObject::Update()
{
	// �v���C���[���߂��ɂ��Ȃ���΃C���^���N�g�̐i�s�󋵂����炷
	if (m_isNear == false && m_interactTime >= 0)
	{
		float time = Time::GetInstance()->GetLocalDeltaTime();
		m_interactTime -= time > m_interactTime ? m_interactTime : time;
	}

	InteractiveObjectBase::Update();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	// �l���傫���Ȃ肷����̂�h��
	if (m_elapsedTime > Math::Fpi * 2)
	{
		m_elapsedTime -= Math::Fpi * 2;
	}

	// �r�b�N���}�[�N���㉺������
	m_modelDisplay->SetPosition(
		m_transform.position + ExclamationBaseOffset + Vector3(0, std::sin(m_elapsedTime * ExclamationSwingRate), 0)
	);
}

void ItemObject::Draw()
{
	InteractiveObjectBase::Draw();

	// �C���^���N�g�̐i�s�󋵂�\��
	if (m_isNear)
	{
		// �Q�[�W�̕\�����W���v�Z
		VECTOR screenPos = ConvWorldPosToScreenPos(m_transform.position + ExclamationBaseOffset + InteractUiOffset);

		// �Q�[�W��\��
		m_uiInteractItemGage->OnDisp(screenPos, m_interactTime / GetItemSeconds);
	}
}

void ItemObject::OnInteract()
{
	// �v���C���[�ɃC���^���N�g��ʒm
	m_playerController->OnInteract();

	// UI�ɃC���^���N�g��ʒm
	m_uiInteractItemGage->IsInteract();

	m_interactTime += Time::GetInstance()->GetLocalDeltaTime();

	if (m_interactTime >= GetItemSeconds)
	{
		// �C���x���g���ɒǉ�
		ItemField::GetInstance()->AddItem(ItemField::FieldName::Inventory, m_itemNum);

		// ���g���폜
		Destroy();
	}
}