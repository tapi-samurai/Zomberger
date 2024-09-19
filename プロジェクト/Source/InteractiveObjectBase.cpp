#include "InteractiveObjectBase.h"
#include "Collider3D.h"
#include "Input.h"

InteractiveObjectBase::InteractiveObjectBase(Collider3D* collider, const char* name, const char* modelName) :
	Actor3D(name, modelName),
	m_isNear(false)
{
	// �R���C�_�[��ݒ�
	m_collider = collider;
}

void InteractiveObjectBase::Update()
{
	Actor3D::Update();

	// �t���O�̃��Z�b�g
	m_isNear = false;
}

void InteractiveObjectBase::OnCollision3D(const Actor3D* other)
{
	// �Փ˂����̂����o��łȂ���΃X�L�b�v
	if (other->GetName() != "PlayerInteractLandmark") return;

	m_isNear = true;

	// ���W��ێ�
	m_playerPos = other->GetPosition();

	// F�L�[��������Ă��Ȃ���΃X�L�b�v
	if (Input::GetInstance()->IsKeyPress(KEY_INPUT_F) == false) return;

	OnInteract();
}