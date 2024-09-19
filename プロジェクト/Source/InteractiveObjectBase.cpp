#include "InteractiveObjectBase.h"
#include "Collider3D.h"
#include "Input.h"

InteractiveObjectBase::InteractiveObjectBase(Collider3D* collider, const char* name, const char* modelName) :
	Actor3D(name, modelName),
	m_isNear(false)
{
	// コライダーを設定
	m_collider = collider;
}

void InteractiveObjectBase::Update()
{
	Actor3D::Update();

	// フラグのリセット
	m_isNear = false;
}

void InteractiveObjectBase::OnCollision3D(const Actor3D* other)
{
	// 衝突したのが検出器でなければスキップ
	if (other->GetName() != "PlayerInteractLandmark") return;

	m_isNear = true;

	// 座標を保持
	m_playerPos = other->GetPosition();

	// Fキーが押されていなければスキップ
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_F) == false) return;

	OnInteract();
}