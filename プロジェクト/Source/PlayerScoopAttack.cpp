#include "PlayerScoopAttack.h"
#include "CapsuleCollider3D.h"
#include "PlayerController.h"
#include "ModelDisplay.h"
#include "Time.h"
#include "ItemField.h"
#include "SoundLoader.h"
#include "SoundData.h"
#include "SoundEffect.h"

PlayerScoopAttack::PlayerScoopAttack(const PlayerController* playerController) :
	Actor3D("Scoop"),
	m_playerController(playerController),
	m_coolTime(0),
	m_inventoryNum(0),
	m_elapsedTime(0),
	m_isAttack(false),
	m_seSwing(0),
	m_seHit(0)
{
	// ���W��ݒ�
	m_transform.position =
		m_playerController->GetPosition() + m_transform.Forward() * LengthAsPlayer * m_playerController->GetRotation();

	// �����蔻���ݒ�E������
	m_collider = new CapsuleCollider3D(Vector3(0, 6, 0), Vector3(0, 0, -8), ColliderRadius);
	m_collider->isEnabled = false;
}

void PlayerScoopAttack::Load()
{
	Actor3D::Load();

	// SE�ǂݍ���
	m_seSwing = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ScoopSwing)].SoundName);
	m_seHit = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ScoopHit)].SoundName);

	// SE�ݒ�
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seSwing, SoundData::List[static_cast<int>(SoundData::Name::ScoopSwing)].volume);
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seHit, SoundData::List[static_cast<int>(SoundData::Name::ScoopHit)].volume);
}

void PlayerScoopAttack::Release()
{
	Actor3D::Release();

	// SE�폜
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ScoopSwing)].SoundName);
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ScoopHit)].SoundName);
}

void PlayerScoopAttack::Update()
{
	Actor3D::Update();

	// ���W�Ɖ�]�𓯊�
	m_transform.position = m_playerController->GetPosition() + -m_playerController->GetTransform().Forward() * LengthAsPlayer;
	m_transform.rotation = m_playerController->GetRotation();

	// �N�[���^�C���������
	if (m_coolTime > 0)
	{
		// �N�[���^�C�������炷
		m_coolTime -= Time::GetInstance()->GetLocalDeltaTime();
	}

	// �U�����̏���
	if (m_isAttack)
	{
		// �o�ߎ��Ԃ��J�E���g
		m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

		if (m_elapsedTime >= AttackDelay)
		{
			// �U���J�n��
			if (m_collider->isEnabled == false)
			{
				// SE
				PlaySoundMem(m_seSwing, DX_PLAYTYPE_BACK);

				// �����蔻���L����
				m_collider->isEnabled = true;
			}

			// �U���I����
			if (m_elapsedTime >= AttackDelay + AttackDuration)
			{
				// �����蔻��𖳌���
				m_collider->isEnabled = false;

				// �ϐ������Z�b�g
				m_isAttack = false;
				m_elapsedTime = 0;
			}
		}
	}
}

void PlayerScoopAttack::OnCollision3D(const Actor3D* other)
{
	// �G�ɓ���������
	if (other->GetName() == "Zombie")
	{
		// �����蔻��𖳌���
		m_collider->isEnabled = false;

		// �ϐ������Z�b�g
		m_isAttack = false;
		m_elapsedTime = 0;

		// ���m���ŉ���
		if (rand() % 100 < BreakProbability)
		{
			ItemField::GetInstance()->ReduceItem(ItemField::FieldName::Inventory, m_inventoryNum);
		}

		// SE
		PlaySoundMem(m_seHit, DX_PLAYTYPE_BACK);
	}
}

bool const PlayerScoopAttack::OnAttack(int inventoryNum)
{
	// �N�[���^�C�����܂��c���Ă�����L�����Z��
	if (m_coolTime > 0) return false;

	m_isAttack = true;
	m_coolTime = AttackCoolTime;
	m_inventoryNum = inventoryNum;

	return true;
}