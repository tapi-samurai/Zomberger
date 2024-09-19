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
	// 初期位置を設定
	m_transform.position = position;
	m_transform.rotation = rotation;
	m_transform.scale = Scale;

	// コライダーを設定
	m_collider = new CapsuleCollider3D(ColliderBasePos, Colliderpos2Offset, ColliderRadius);

	// アニメーション登録
	for (int animeNum = 0; animeNum < static_cast<int>(Anime::Length); animeNum++)
	{
		m_model->AnimeRegister(AnimeName[animeNum], AnimeData[animeNum]);
	}

	// スコップのモデルを設定
	m_scoopObject = new ModelDisplay(
		"Scoop", ModelData::List[static_cast<int>(ModelData::Name::Scoop)].ModelName,
		Vector3(), Quaternion::CreateIdentity(), ScoopScale
	);
	AddChild(m_scoopObject);
}

void PlayerController::Load()
{
	Actor3D::Load();

	// SE読み込み
	m_seEat = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::ItemEat)].SoundName, false);

	// SE設定
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seEat, SoundData::List[static_cast<int>(SoundData::Name::ItemEat)].volume);
}

void PlayerController::Release()
{
	Actor3D::Release();

	// SE削除
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::ItemEat)].SoundName);
}

void PlayerController::Update()
{
	Actor3D::Update();

	if (m_canMove == true && m_isDeath == false && m_isInteract == false)
	{
		// キーの入力を初期化
		m_inputMove = Vector2();

		// キーの入力を取得
		m_inputMove.x += Input::GetInstance()->IsKeyPress(KEY_INPUT_D) ? 1 : 0;
		m_inputMove.x += Input::GetInstance()->IsKeyPress(KEY_INPUT_A) ? -1 : 0;
		m_inputMove.y += Input::GetInstance()->IsKeyPress(KEY_INPUT_W) ? 1 : 0;
		m_inputMove.y += Input::GetInstance()->IsKeyPress(KEY_INPUT_S) ? -1 : 0;
		
		// 左右回転
		OnLook(m_inputMove);

		// 前後移動
		OnMove(m_inputMove);

		// マウスが左クリックされたらインベントリのアイテムを使用
		if (
			Input::GetInstance()->IsMouseDown(KEY_INPUT_LEFT) &&
			m_uiInventory->IsActive() == true
			)
		{
			UseInventorySlotItem();
		}
	}

	// アニメーションが終了した場合
	if (m_model->IsFinishAnime())
	{
		// 移動可能にする
		m_canMove = true;
	}

	if (m_uiInventory->IsActive() == true)
	{
		// 手に持っているアイテムのアイテム番号を取得
		int haveItemNum = static_cast<int>(ItemField::GetInstance()->CheckInventory(
			ItemField::FieldName::Inventory, static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x)));

		// スコップを持っていたら表示
		if (haveItemNum == static_cast<int>(ItemData::OtherItem::Scoop))
		{
			m_scoopObject->SetActive(true);
		}
		else
		{
			m_scoopObject->SetActive(false);
		}
	}

	// スコップの移動・回転
	m_scoopObject->SetPosition(MV1GetFramePosition(GetModelHandle(), RightHandFrameIndex));
	m_scoopObject->SetRotation(m_transform.rotation);

	m_isInteract = false;
}

void PlayerController::OnCollision3D(const Actor3D* other)
{
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

		// プレイヤーと高さが同じとみなしたオブジェクトまでの距離を計算
		float length =
			(m_transform.position - Vector3(other->GetPosition().x, m_transform.position.y, other->GetPosition().z)).Magnitude();

		// 移動距離を計算
		float moveLength = radius + otherRadius - length;

		// 移動
		m_transform.position += (m_transform.position - other->GetPosition()).Normalize() * moveLength * Vector3(1, 0, 1);
	}
}

// 前後移動
void PlayerController::OnMove(const Vector2& inputMove)
{
	// 移動
	float speed = m_inputMove.y > 0 ? MoveForwardSpeed : MoveBackwardSpeed;
	m_velocity = -m_transform.Forward() * speed * m_inputMove.y * Time::GetInstance()->GetLocalDeltaTime();
	m_transform.position += m_velocity;

	// アニメーション
	if (m_inputMove.y > 0)
	{
		// 前進
		m_model->Play(AnimeName[static_cast<int>(Anime::WalkForward)]);
	}
	else if (m_inputMove.y < 0)
	{
		// 後退
		m_model->Play(AnimeName[static_cast<int>(Anime::WalkBack)]);
	}
	else
	{
		// 待機
		m_model->Play(AnimeName[static_cast<int>(Anime::Idle)]);
	}
}

// 左右回転
void PlayerController::OnLook(const Vector2& inputMove)
{
	// 回転
	m_transform.rotation =
		m_transform.rotation * Quaternion::CreateRotationAxis(
			Vector3::Up(),
			Math::DegToRad(DegAnglePerSecond) * Time::GetInstance()->GetLocalDeltaTime() * m_inputMove.x
		);
}

void PlayerController::UseInventorySlotItem()
{
	// アイテムがスコップかそれ以外か判定
	if (
		static_cast<int>(ItemField::GetInstance()->CheckInventory(
			ItemField::FieldName::Inventory, static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x)))
		== static_cast<int>(ItemData::OtherItem::Scoop)
		)
	{
		// スコップの場合

		// スクリプトがnullであればスキップ
		if (m_playerScoopAttack == nullptr) return;

		// 攻撃
		m_playerScoopAttack->OnAttack(static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x));

		// 移動不可にする
		m_canMove = false;

		// アニメーション
		m_model->Play(AnimeName[static_cast<int>(Anime::ScoopSwing)]);
	}
	else
	{
		// 体力と空腹度を回復
		PlayerState::GetInstance()->SetHealth(ItemData::List[ItemField::GetInstance()->CheckInventory(
			ItemField::FieldName::Inventory, static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x))].HealthRecoveryValue
		);
		PlayerState::GetInstance()->SetSatietyLevel(ItemData::List[ItemField::GetInstance()->CheckInventory(
			ItemField::FieldName::Inventory, static_cast<int>(ItemField::GetInstance()->GetSelectSlotNum().x))].SatietyRecoveryValue
		);

		// アイテムを削除
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

	// 死亡判定
	if (PlayerState::GetInstance()->GetHealth() <= 0)
	{
		m_isDeath = true;

		m_collider->isEnabled = false;

		// アニメーション
		m_model->Play(AnimeName[static_cast<int>(Anime::Die)]);
	}
}

void PlayerController::OnInteract()
{
	// インタラクト時の処理
	m_isInteract = true;

	m_model->Play(AnimeName[static_cast<int>(Anime::Search)]);
}