#include "PlayerController.h"
#include "CapsuleCollider3D.h"
#include "Input.h"
#include "Time.h"
#include "Math.h"
#include "ModelData.h"
#include "BitFlag.h"
#include "SceneTransitionQuad.h"
#include "ItemField.h"
#include "PlayerState.h"
#include "UiInventory.h"
#include "ItemData.h"
#include "PlayerScoopAttack.h"
#include "ModelDisplay.h"
#include "SoundLoader.h"
#include "SoundData.h"
#include "SoundEffect.h"

PlayerController::PlayerController(
	const Vector3& position, const Quaternion& rotation, const UiInventory* uiInventory
) :
	Actor3D(
		"PlayerController",
		ModelData::List[static_cast<int>(ModelData::Name::Player)].ModelName,
		ModelData::List[static_cast<int>(ModelData::Name::Player)].BaseFrameIndex
	),
	m_uiInventory(uiInventory),
	m_playerScoopAttack(nullptr),
	m_inputMove(Vector2()),
	m_velocity(Vector3()),
	m_canMove(true),
	m_isAttack(false),
	m_isDeath(false),
	m_isInteract(false),
	m_seEat(0)
{
	// �����ʒu��ݒ�
	m_transform.position = position;
	m_transform.rotation = rotation;
	m_transform.scale = Scale;

	// �R���C�_�[��ݒ�
	m_collider = new CapsuleCollider3D(ColliderBasePos, Colliderpos2Offset, ColliderRadius);

	// �A�j���[�V�����o�^
	for (int animeNum = 0; animeNum < static_cast<int>(Anime::Length); animeNum++)
	{
		m_model->AnimeRegister(AnimeName[animeNum], AnimeData[animeNum]);
	}

	// �X�R�b�v�̃��f����ݒ�
	m_scoopObject = new ModelDisplay(
		"Scoop", ModelData::List[static_cast<int>(ModelData::Name::Scoop)].ModelName,
		Vector3(), Quaternion::CreateIdentity(), ScoopScale
	);
	AddChild(m_scoopObject);
}

void PlayerController::Load()
{
	Actor3D::Load();

	// SE�ǂݍ���
	m_seEat = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ItemEat)].SoundName, false);

	// SE�ݒ�
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seEat, SoundData::List[static_cast<int>(SoundData::Name::ItemEat)].volume);
}

void PlayerController::Release()
{
	Actor3D::Release();

	// SE�폜
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ItemEat)].SoundName);
}

void PlayerController::Update()
{
	Actor3D::Update();

	if (m_canMove == true && m_isDeath == false && m_isInteract == false)
	{
		// �L�[�̓��͂�������
		m_inputMove = Vector2();

		// �L�[�̓��͂��擾
		m_inputMove.x += Input::GetInstance()->IsKeyPress(KEY_INPUT_D) ? 1 : 0;
		m_inputMove.x += Input::GetInstance()->IsKeyPress(KEY_INPUT_A) ? -1 : 0;
		m_inputMove.y += Input::GetInstance()->IsKeyPress(KEY_INPUT_W) ? 1 : 0;
		m_inputMove.y += Input::GetInstance()->IsKeyPress(KEY_INPUT_S) ? -1 : 0;
		
		// ���E��]
		OnLook(m_inputMove);

		// �O��ړ�
		OnMove(m_inputMove);

		// �}�E�X�����N���b�N���ꂽ��C���x���g���̃A�C�e�����g�p
		if (
			Input::GetInstance()->IsMouseDown(KEY_INPUT_LEFT) &&
			m_uiInventory->IsActive() == true
			)
		{
			UseInventorySlotItem();
		}
	}

	// �A�j���[�V�������I�������ꍇ
	if (m_model->IsFinishAnime())
	{
		// �ړ��\�ɂ���
		m_canMove = true;
	}

	if (m_uiInventory->IsActive() == true)
	{
		// ��Ɏ����Ă���A�C�e���̃A�C�e���ԍ����擾
		int haveItemNum = static_cast<int>(ItemField::GetInstance()->CheckInventory(
			ItemField::FieldName::Inventory, static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x)));

		// �X�R�b�v�������Ă�����\��
		if (haveItemNum == static_cast<int>(ItemData::OtherItem::Scoop))
		{
			m_scoopObject->SetActive(true);
		}
		else
		{
			m_scoopObject->SetActive(false);
		}
	}

	// �X�R�b�v�̈ړ��E��]
	m_scoopObject->SetPosition(MV1GetFramePosition(GetModelHandle(), RightHandFrameIndex));
	m_scoopObject->SetRotation(m_transform.rotation);

	m_isInteract = false;
}

void PlayerController::OnCollision3D(const Actor3D* other)
{
	// �ǂɓ���������ړ��ʂ��l�������ꏊ�Ɉړ�������
	if (other->GetName() == "Wall")
	{
		// �ʂ̗��\���擾
		// �\�Ȃ�P�A���Ȃ�-�P
		Vector3 otherForward = other->GetTransform().Forward();
		Vector3 playerVec = (m_transform.position - other->GetPosition()).Normalize();
		float frontBack = Vector3::Dot(otherForward, playerVec) >= 0.0f ? 1.0f : -1.0f;

		Vector3 n = other->GetTransform().Forward();				// ���ʂ̖@��
		Vector3 x = other->GetPosition();							// ���ʂ̍��W
		Vector3 x0 = m_transform.position;							// �x�N�g���̎n�_
		Vector3 m = other->GetTransform().Forward() * (-frontBack);	// �x�N�g���̕���
		float h = Vector3::Dot(n, x);								// ���ʂ̖@���ƍ��W�̓���

		// ��_���W���v�Z
		Vector3 intersectPos = x0 + ((h - Vector3::Dot(n, x0)) / (Vector3::Dot(n, m))) * m;

		// �ړ��������v�Z
		float magnitude = (intersectPos - m_transform.position).Magnitude();
		float moveLength = m_collider->GetRadius() - magnitude;

		m_transform.position += other->GetTransform().Forward() * frontBack * moveLength;
	}

	if (other->GetName() == "Object")
	{
		// �݂��̔��a���擾
		float radius = m_collider->GetRadius();
		float otherRadius = other->GetCollider3D()->GetRadius();

		// �v���C���[�ƍ����������Ƃ݂Ȃ����I�u�W�F�N�g�܂ł̋������v�Z
		float length =
			(m_transform.position - Vector3(other->GetPosition().x, m_transform.position.y, other->GetPosition().z)).Magnitude();

		// �ړ��������v�Z
		float moveLength = radius + otherRadius - length;

		// �ړ�
		m_transform.position += (m_transform.position - other->GetPosition()).Normalize() * moveLength * Vector3(1, 0, 1);
	}
}

// �O��ړ�
void PlayerController::OnMove(const Vector2& inputMove)
{
	// �ړ�
	float speed = m_inputMove.y > 0 ? MoveForwardSpeed : MoveBackwardSpeed;
	m_velocity = -m_transform.Forward() * speed * m_inputMove.y * Time::GetInstance()->GetLocalDeltaTime();
	m_transform.position += m_velocity;

	// �A�j���[�V����
	if (m_inputMove.y > 0)
	{
		// �O�i
		m_model->Play(AnimeName[static_cast<int>(Anime::WalkForward)]);
	}
	else if (m_inputMove.y < 0)
	{
		// ���
		m_model->Play(AnimeName[static_cast<int>(Anime::WalkBack)]);
	}
	else
	{
		// �ҋ@
		m_model->Play(AnimeName[static_cast<int>(Anime::Idle)]);
	}
}

// ���E��]
void PlayerController::OnLook(const Vector2& inputMove)
{
	// ��]
	m_transform.rotation =
		m_transform.rotation * Quaternion::CreateRotationAxis(
			Vector3::Up(),
			Math::DegToRad(DegAnglePerSecond) * Time::GetInstance()->GetLocalDeltaTime() * m_inputMove.x
		);
}

void PlayerController::UseInventorySlotItem()
{
	// �A�C�e�����X�R�b�v������ȊO������
	if (
		static_cast<int>(ItemField::GetInstance()->CheckInventory(
			ItemField::FieldName::Inventory, static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x)))
		== static_cast<int>(ItemData::OtherItem::Scoop)
		)
	{
		// �X�R�b�v�̏ꍇ

		// �X�N���v�g��null�ł���΃X�L�b�v
		if (m_playerScoopAttack == nullptr) return;

		// �U��
		m_playerScoopAttack->OnAttack(static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x));

		// �ړ��s�ɂ���
		m_canMove = false;

		// �A�j���[�V����
		m_model->Play(AnimeName[static_cast<int>(Anime::ScoopSwing)]);
	}
	else
	{
		// �̗͂Ƌ󕠓x����
		PlayerState::GetInstance()->SetHealth(ItemData::List[ItemField::GetInstance()->CheckInventory(
			ItemField::FieldName::Inventory, static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x))].HealthRecoveryValue
		);
		PlayerState::GetInstance()->SetSatietyLevel(ItemData::List[ItemField::GetInstance()->CheckInventory(
			ItemField::FieldName::Inventory, static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x))].SatietyRecoveryValue
		);

		// �A�C�e�����폜
		ItemField::GetInstance()->ReduceItem(
			ItemField::FieldName::Inventory, static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x)
		);

		// SE
		PlaySoundMem(m_seEat, DX_PLAYTYPE_BACK);
	}
}

void PlayerController::AddDamage(int damage)
{
	PlayerState::GetInstance()->SetHealth(-damage);

	// ���S����
	if (PlayerState::GetInstance()->GetHealth() <= 0)
	{
		m_isDeath = true;

		m_collider->isEnabled = false;

		// �A�j���[�V����
		m_model->Play(AnimeName[static_cast<int>(Anime::Die)]);
	}
}

void PlayerController::OnInteract()
{
	// �C���^���N�g���̏���
	m_isInteract = true;

	m_model->Play(AnimeName[static_cast<int>(Anime::Search)]);
}