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
	// 座標を設定
	m_transform.position =
		m_playerController->GetPosition() + m_transform.Forward() * LengthAsPlayer * m_playerController->GetRotation();

	// 当たり判定を設定・無効化
	m_collider = new CapsuleCollider3D(Vector3(0, 6, 0), Vector3(0, 0, -8), ColliderRadius);
	m_collider->isEnabled = false;
}

void PlayerScoopAttack::Load()
{
	Actor3D::Load();

	// SE読み込み
	m_seSwing = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ScoopSwing)].SoundName);
	m_seHit = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ScoopHit)].SoundName);

	// SE設定
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seSwing, SoundData::List[static_cast<int>(SoundData::Name::ScoopSwing)].volume);
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seHit, SoundData::List[static_cast<int>(SoundData::Name::ScoopHit)].volume);
}

void PlayerScoopAttack::Release()
{
	Actor3D::Release();

	// SE削除
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ScoopSwing)].SoundName);
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ScoopHit)].SoundName);
}

void PlayerScoopAttack::Update()
{
	Actor3D::Update();

	// 座標と回転を同期
	m_transform.position = m_playerController->GetPosition() + -m_playerController->GetTransform().Forward() * LengthAsPlayer;
	m_transform.rotation = m_playerController->GetRotation();

	// クールタイムがあれば
	if (m_coolTime > 0)
	{
		// クールタイムを減らす
		m_coolTime -= Time::GetInstance()->GetLocalDeltaTime();
	}

	// 攻撃中の処理
	if (m_isAttack)
	{
		// 経過時間をカウント
		m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

		if (m_elapsedTime >= AttackDelay)
		{
			// 攻撃開始時
			if (m_collider->isEnabled == false)
			{
				// SE
				PlaySoundMem(m_seSwing, DX_PLAYTYPE_BACK);

				// 当たり判定を有効化
				m_collider->isEnabled = true;
			}

			// 攻撃終了時
			if (m_elapsedTime >= AttackDelay + AttackDuration)
			{
				// 当たり判定を無効化
				m_collider->isEnabled = false;

				// 変数をリセット
				m_isAttack = false;
				m_elapsedTime = 0;
			}
		}
	}
}

void PlayerScoopAttack::OnCollision3D(const Actor3D* other)
{
	// 敵に当たったら
	if (other->GetName() == "Zombie")
	{
		// 当たり判定を無効化
		m_collider->isEnabled = false;

		// 変数をリセット
		m_isAttack = false;
		m_elapsedTime = 0;

		// 一定確率で壊れる
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
	// クールタイムがまだ残っていたらキャンセル
	if (m_coolTime > 0) return false;

	m_isAttack = true;
	m_coolTime = AttackCoolTime;
	m_inventoryNum = inventoryNum;

	return true;
}