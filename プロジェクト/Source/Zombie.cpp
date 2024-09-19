#include "Zombie.h"
#include "CapsuleCollider3D.h"
#include "PlayerController.h"
#include "Math.h"
#include "Time.h"
#include "ModelData.h"
#include "PlayerController.h"
#include "PlayerState.h"
#include "SoundLoader.h"
#include "SoundEffect.h"
#include "SoundData.h"

Zombie::Zombie(PlayerController* playerController, const Vector3& position) :
	Actor3D(
		"Zombie",
		ModelData::List[static_cast<int>(ModelData::Name::Zombie)].ModelName,
		ModelData::List[static_cast<int>(ModelData::Name::Zombie)].BaseFrameIndex
		),
	m_playerController(playerController),
	m_state(State::Spawn),
	m_velocity(Vector3()),
	m_remainingCooldownTime(0),
	m_seSpawn(0),
	m_seVoice(0),
	m_voiceNum(0),
	m_elapsedTime(0)
{
	// Transform�ݒ�
	m_transform.scale = Scale;
	m_transform.rotation = Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(static_cast<float>(rand() % 359)));
	m_transform.position =
		position + Vector3(0, SpawnOffsetY, 0) + -m_transform.Forward() * SpawnOffsetBefore;

	// �R���C�_�[��ݒ�E������
	m_collider = new CapsuleCollider3D(Vector3(0, ColliderRadius, 0), Colliderpos2Offset, ColliderRadius);
	m_collider->isEnabled = false;

	// �̗͂������_���Őݒ�
	m_health = (rand() % MaxHealth) + 1;

	// �A�j���[�V�����o�^
	for (int animeNum = 0; animeNum < static_cast<int>(Anime::Length); animeNum++)
	{
		m_model->AnimeRegister(AnimeName[animeNum], AnimeData[animeNum]);
	}

	// �{�C�X�̃C���^�[�o����ݒ�
	m_nextVoiceInterval = static_cast<float>(rand() % (MaxVoiceInterval - MinVoiceInterval + 1) + MinVoiceInterval);
}

void Zombie::Load()
{
	Actor3D::Load();

	// SE�ǂݍ���
	// �{�C�X�̓����_���őI��
	m_voiceNum = rand() % SoundData::ZombieVoice.size();
	m_seSpawn = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ZombieSpawn)].SoundName);
	m_seVoice = SoundLoader::GetInstance()->Load(SoundData::ZombieVoice[m_voiceNum].SoundName);

	// SE�ݒ�
	SoundEffect::GetInstance()->SetSoundEffect2D(
		m_seSpawn,
		SoundData::List[static_cast<int>(SoundData::Name::ZombieSpawn)].volume,
		SoundData::List[static_cast<int>(SoundData::Name::ZombieSpawn)].FrequencyPal
	);
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seVoice, SoundData::List[m_voiceNum].volume);
}

void Zombie::Release()
{
	Actor3D::Release();

	// SE�폜
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ZombieSpawn)].SoundName);
	SoundLoader::GetInstance()->Delete(SoundData::ZombieVoice[m_voiceNum].SoundName);
}

void Zombie::Update()
{
	Actor3D::Update();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	if (m_remainingCooldownTime > 0)
	{
		// �N�[���^�C�������炷
		m_remainingCooldownTime -= Time::GetInstance()->GetLocalDeltaTime();

		if (m_remainingCooldownTime < 0)
		{
			// �ǐՂ��ĊJ
			m_state = State::OnChase;

			// �R���C�_�[��L����
			m_collider->isEnabled = true;
		}
	}

	// �o��A�j���[�V�������I������܂őҋ@
	switch (m_state)
	{
	case State::Spawn:
		// �X�|�[�����̃A�j���[�V�������Đ�
		m_model->Play(AnimeName[static_cast<int>(Anime::Clim)]);

		// SE
		PlaySoundMem(m_seSpawn, DX_PLAYTYPE_BACK);
		
		// �X�e�[�g���X�V
		m_state = State::SpawnWait;
		break;

	case State::SpawnWait:
		// �A�j���[�V�������I������܂őҋ@
		if (m_model->IsFinishAnime())
		{
			// �A�j���[�V�������I�������ǐՊJ�n
			m_state = State::OnChase;

			// �����A�j���[�V����
			m_model->Play(AnimeName[static_cast<int>(Anime::WalkForward)]);

			// ���W�����ɖ߂�
			m_transform.position -= Vector3(0, SpawnOffsetY, 0) + -m_transform.Forward() * SpawnOffsetBefore;

			// �����蔻���L����
			m_collider->isEnabled = true;
		}
		break;

	case State::Attack:
		// �A�j���[�V����
		m_model->Play(AnimeName[static_cast<int>(Anime::Attack)]);
		break;

	case State::OnChase:
		// ��]�ƈړ�
		LookAtPlayer();
		MoveForward();

		// �A�j���[�V����
		m_model->Play(AnimeName[static_cast<int>(Anime::WalkForward)]);
		break;

	case State::Die:
		// �A�j���[�V����
		m_model->Play(AnimeName[static_cast<int>(Anime::Die)]);

		// �A�j���[�V�������I���܂őҋ@
		if (m_model->IsFinishAnime())
		{
			// ���g���폜
			Destroy();
		}
	}

	if (m_elapsedTime >= m_nextVoiceInterval)
	{
		// �{�C�X
		PlaySoundMem(m_seVoice, DX_PLAYTYPE_BACK);

		// �C���^�[�o����ݒ�
		m_nextVoiceInterval = static_cast<float>(rand() % (MaxVoiceInterval - MinVoiceInterval + 1) + MinVoiceInterval);

		m_elapsedTime = 0;
	}
}

void Zombie::OnCollision3D(const Actor3D* other)
{
	// �v���C���[�ɓ���������_���[�W��^����
	if (other->GetName() == m_playerController->GetName())
	{
		// �_���[�W
		m_playerController->AddDamage(DamageValue);

		// �ǐՂ̃N�[���^�C����ݒ�
		m_remainingCooldownTime = AttackCooldownTime;

		// �U����ԂɈڍs
		m_state = State::Attack;

		// �R���C�_�[�𖳌���
		m_collider->isEnabled = false;
	}

	// �X�R�b�v�ɓ���������
	if (other->GetName() == "Scoop")
	{
		// �_���[�W���󂯂�
		m_health -= 1;

		// �̗͂�0�ɂȂ����玀�S����
		if (m_health <= 0)
		{
			Die();
		}
	}

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

		// �]���r�ƍ����������Ƃ݂Ȃ����I�u�W�F�N�g�܂ł̋������v�Z
		float length =
			(m_transform.position - Vector3(other->GetPosition().x, m_transform.position.y, other->GetPosition().z)).Magnitude();

		// �ړ��������v�Z
		float moveLength = radius + otherRadius - length;

		// �ړ�
		m_transform.position += (m_transform.position - other->GetPosition()).Normalize() * moveLength * Vector3(1, 0, 1);
	}
}

void Zombie::MoveForward()
{
	// �ړ��ʂ��v�Z
	m_velocity = -m_transform.Forward() * MoveSpeed * Time::GetInstance()->GetLocalDeltaTime();

	// �O���ɐi��
	m_transform.position += m_velocity;
}

// �v���C���[�̕����������悤�Ɉ�葬�x�ŉ�]
void Zombie::LookAtPlayer()
{
	// �^�[�Q�b�g�܂ł̃x�N�g�����v�Z
	Vector3 targetVec = m_playerController->GetPosition() - m_transform.position;

	// �c�̊p�x�̌v�Z�p�ɋ����ƍ������v�Z
	float height = targetVec.y;
	targetVec.y = 0;
	float length = targetVec.Magnitude();

	// �x�N�g�������̊p�x�̌v�Z�p�ɍ����������Đ��K��
	targetVec = -targetVec.Normalize();

	// ���������������ʃx�N�g�����v�Z
	Vector3 forward = (m_transform.Forward() * Vector3(1, 0, 1)).Normalize();

	// ���݂̉�]�ƃv���C���[�̕�����������]�̊p�x���v�Z
	// �l�����W�A���ɕϊ�
	float angle = Vector3::Angle(forward, targetVec);
	angle = acos(Math::Clamp(angle, -1.0f, 1.0f));

	// �O�ςō��E�𔻒肵�āA�K�v�ł���Ίp�x�𔽓]
	Vector2 from = Vector2(forward.x, forward.z);
	Vector2 to = Vector2(targetVec.x, targetVec.z);

	if (Vector2::Cross(from, to) > 0)
	{
		angle = -angle;
	}

	// ����]�𔽉f
	m_transform.rotation = Quaternion::RotateToWards(
		m_transform.rotation,
		m_transform.rotation * Quaternion::CreateRotationAxis(Vector3::Up(), angle),
		Math::DegToRad(DegAnglePerSecond) * Time::GetInstance()->GetLocalDeltaTime()
	);
}

void Zombie::Die()
{
	// ���S��Ԃɂ���
	m_state = State::Die;

	// �����蔻��𖳌���
	m_collider->isEnabled = false;
}