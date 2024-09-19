#include "Camera.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "Math.h"

void Camera::Draw()
{
	Node::Draw();

	// SetDrawScreen���Ăяo�����тɁA�`�拗���ݒ肪�K�{
	SetCameraNearFar(m_setNear, m_setFar);

	// �J�����̍��W�ݒ�
	Vector3 target = m_position + Vector3::Forward() * m_rotation;
	Vector3 up = Vector3::Up() * m_rotation;

	// �J�������Z�b�g
	SetCameraPositionAndTargetAndUpVec(m_position, target, up);

	// �T�E���h���X�i�[�̈ʒu�ݒ�
	Set3DSoundListenerPosAndFrontPos_UpVecY(m_position, up);

	// ����p��ݒ�
	SetupCamera_Perspective(Math::DegToRad(m_fov));

#ifndef NDEBUG
	// �Œ��`�拗����������悤�ɉ~�𐶐�
	DrawSphere3D(m_position, m_setFar, 16, GetColor(0, 0, 255), GetColor(0, 0, 0), false);
#endif
}