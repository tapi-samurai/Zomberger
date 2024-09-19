#include "PlayerInteractLandmark.h"
#include "PlayerController.h"
#include "SceneTransitionQuad.h"
#include "BitFlag.h"
#include "SphereCollider3D.h"
#include "Input.h"
#include "StageData.h"

PlayerInteractLandmark::PlayerInteractLandmark(const PlayerController* playerController) :
	Actor3D("PlayerInteractLandmark"),
	m_playerController(playerController)
{
	// ���W��ݒ�
	m_transform.position = m_playerController->GetPosition() + Vector3(0, ColliderRadius, 0);

	// �R���C�_�[��ݒ�
	m_collider = new SphereCollider3D(Vector3(), ColliderRadius);
}

void PlayerInteractLandmark::Update()
{
	Actor3D::Update();

	// �v���C���[�ɒǏ]
	m_transform.position = m_playerController->GetPosition() + Vector3(0, ColliderRadius, 0);
}