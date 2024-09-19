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
	// Transform設定
	m_transform.scale = Scale;
	m_transform.rotation = Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(static_cast<float>(rand() % 359)));
	m_transform.position =
		position + Vector3(0, SpawnOffsetY, 0) + -m_transform.Forward() * SpawnOffsetBefore;

	// コライダーを設定・無効化
	m_collider = new CapsuleCollider3D(Vector3(0, ColliderRadius, 0), Colliderpos2Offset, ColliderRadius);
	m_collider->isEnabled = false;

	// 体力をランダムで設定
	m_health = (rand() % MaxHealth) + 1;

	// アニメーション登録
	for (int animeNum = 0; animeNum < static_cast<int>(Anime::Length); animeNum++)
	{
		m_model->AnimeRegister(AnimeName[animeNum], AnimeData[animeNum]);
	}

	// ボイスのインターバルを設定
	m_nextVoiceInterval = static_cast<float>(rand() % (MaxVoiceInterval - MinVoiceInterval + 1) + MinVoiceInterval);
}

void Zombie::Load()
{
	Actor3D::Load();

	// SE読み込み
	// ボイスはランダムで選択
	m_voiceNum = rand() % SoundData::ZombieVoice.size();
	m_seSpawn = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ZombieSpawn)].SoundName);
	m_seVoice = SoundLoader::GetInstance()->Load(SoundData::ZombieVoice[m_voiceNum].SoundName);

	// SE設定
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

	// SE削除
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ZombieSpawn)].SoundName);
	SoundLoader::GetInstance()->Delete(SoundData::ZombieVoice[m_voiceNum].SoundName);
}

void Zombie::Update()
{
	Actor3D::Update();

	m_elapsedTime += Time::GetInstance()->GetLocalDeltaTime();

	if (m_remainingCooldownTime > 0)
	{
		// クールタイムを減らす
		m_remainingCooldownTime -= Time::GetInstance()->GetLocalDeltaTime();

		if (m_remainingCooldownTime < 0)
		{
			// 追跡を再開
			m_state = State::OnChase;

			// コライダーを有効化
			m_collider->isEnabled = true;
		}
	}

	// 登場アニメーションが終了するまで待機
	switch (m_state)
	{
	case State::Spawn:
		// スポーン時のアニメーションを再生
		m_model->Play(AnimeName[static_cast<int>(Anime::Clim)]);

		// SE
		PlaySoundMem(m_seSpawn, DX_PLAYTYPE_BACK);
		
		// ステートを更新
		m_state = State::SpawnWait;
		break;

	case State::SpawnWait:
		// アニメーションが終了するまで待機
		if (m_model->IsFinishAnime())
		{
			// アニメーションが終わったら追跡開始
			m_state = State::OnChase;

			// 歩きアニメーション
			m_model->Play(AnimeName[static_cast<int>(Anime::WalkForward)]);

			// 座標を元に戻す
			m_transform.position -= Vector3(0, SpawnOffsetY, 0) + -m_transform.Forward() * SpawnOffsetBefore;

			// 当たり判定を有効化
			m_collider->isEnabled = true;
		}
		break;

	case State::Attack:
		// アニメーション
		m_model->Play(AnimeName[static_cast<int>(Anime::Attack)]);
		break;

	case State::OnChase:
		// 回転と移動
		LookAtPlayer();
		MoveForward();

		// アニメーション
		m_model->Play(AnimeName[static_cast<int>(Anime::WalkForward)]);
		break;

	case State::Die:
		// アニメーション
		m_model->Play(AnimeName[static_cast<int>(Anime::Die)]);

		// アニメーションが終わるまで待機
		if (m_model->IsFinishAnime())
		{
			// 自身を削除
			Destroy();
		}
	}

	if (m_elapsedTime >= m_nextVoiceInterval)
	{
		// ボイス
		PlaySoundMem(m_seVoice, DX_PLAYTYPE_BACK);

		// インターバルを設定
		m_nextVoiceInterval = static_cast<float>(rand() % (MaxVoiceInterval - MinVoiceInterval + 1) + MinVoiceInterval);

		m_elapsedTime = 0;
	}
}

void Zombie::OnCollision3D(const Actor3D* other)
{
	// プレイヤーに当たったらダメージを与える
	if (other->GetName() == m_playerController->GetName())
	{
		// ダメージ
		m_playerController->AddDamage(DamageValue);

		// 追跡のクールタイムを設定
		m_remainingCooldownTime = AttackCooldownTime;

		// 攻撃状態に移行
		m_state = State::Attack;

		// コライダーを無効化
		m_collider->isEnabled = false;
	}

	// スコップに当たったら
	if (other->GetName() == "Scoop")
	{
		// ダメージを受ける
		m_health -= 1;

		// 体力が0になったら死亡処理
		if (m_health <= 0)
		{
			Die();
		}
	}

	// 壁に当たったら移動量を考慮した場所に移動させる
	if (other->GetName() == "Wall")
	{
		// 面の裏表を取得
		// 表なら１、裏なら-１
		Vector3 otherForward = other->GetTransform().Forward();
		Vector3 playerVec = (m_transform.position - other->GetPosition()).Normalize();
		float frontBack = Vector3::Dot(otherForward, playerVec) >= 0.0f ? 1.0f : -1.0f;

		Vector3 n = other->GetTransform().Forward();				// 平面の法線
		Vector3 x = other->GetPosition();							// 平面の座標
		Vector3 x0 = m_transform.position;							// ベクトルの始点
		Vector3 m = other->GetTransform().Forward() * (-frontBack);	// ベクトルの方向
		float h = Vector3::Dot(n, x);								// 平面の法線と座標の内積

		// 交点座標を計算
		Vector3 intersectPos = x0 + ((h - Vector3::Dot(n, x0)) / (Vector3::Dot(n, m))) * m;

		// 移動距離を計算
		float magnitude = (intersectPos - m_transform.position).Magnitude();
		float moveLength = m_collider->GetRadius() - magnitude;

		m_transform.position += other->GetTransform().Forward() * frontBack * moveLength;
	}

	if (other->GetName() == "Object")
	{
		// 互いの半径を取得
		float radius = m_collider->GetRadius();
		float otherRadius = other->GetCollider3D()->GetRadius();

		// ゾンビと高さが同じとみなしたオブジェクトまでの距離を計算
		float length =
			(m_transform.position - Vector3(other->GetPosition().x, m_transform.position.y, other->GetPosition().z)).Magnitude();

		// 移動距離を計算
		float moveLength = radius + otherRadius - length;

		// 移動
		m_transform.position += (m_transform.position - other->GetPosition()).Normalize() * moveLength * Vector3(1, 0, 1);
	}
}

void Zombie::MoveForward()
{
	// 移動量を計算
	m_velocity = -m_transform.Forward() * MoveSpeed * Time::GetInstance()->GetLocalDeltaTime();

	// 前方に進む
	m_transform.position += m_velocity;
}

// プレイヤーの方向を向くように一定速度で回転
void Zombie::LookAtPlayer()
{
	// ターゲットまでのベクトルを計算
	Vector3 targetVec = m_playerController->GetPosition() - m_transform.position;

	// 縦の角度の計算用に距離と高さを計算
	float height = targetVec.y;
	targetVec.y = 0;
	float length = targetVec.Magnitude();

	// ベクトルを横の角度の計算用に高さを除いて正規化
	targetVec = -targetVec.Normalize();

	// 高さを除いた正面ベクトルを計算
	Vector3 forward = (m_transform.Forward() * Vector3(1, 0, 1)).Normalize();

	// 現在の回転とプレイヤーの方向を向く回転の角度を計算
	// 値をラジアンに変換
	float angle = Vector3::Angle(forward, targetVec);
	angle = acos(Math::Clamp(angle, -1.0f, 1.0f));

	// 外積で左右を判定して、必要であれば角度を反転
	Vector2 from = Vector2(forward.x, forward.z);
	Vector2 to = Vector2(targetVec.x, targetVec.z);

	if (Vector2::Cross(from, to) > 0)
	{
		angle = -angle;
	}

	// 横回転を反映
	m_transform.rotation = Quaternion::RotateToWards(
		m_transform.rotation,
		m_transform.rotation * Quaternion::CreateRotationAxis(Vector3::Up(), angle),
		Math::DegToRad(DegAnglePerSecond) * Time::GetInstance()->GetLocalDeltaTime()
	);
}

void Zombie::Die()
{
	// 死亡状態にする
	m_state = State::Die;

	// 当たり判定を無効化
	m_collider->isEnabled = false;
}