#include "Camera.h"
#include "DxLib.h"
#include "Input.h"
#include "Screen.h"
#include "Math.h"

void Camera::Draw()
{
	Node::Draw();

	// SetDrawScreenを呼び出すたびに、描画距離設定が必須
	SetCameraNearFar(m_setNear, m_setFar);

	// カメラの座標設定
	Vector3 target = m_position + Vector3::Forward() * m_rotation;
	Vector3 up = Vector3::Up() * m_rotation;

	// カメラをセット
	SetCameraPositionAndTargetAndUpVec(m_position, target, up);

	// サウンドリスナーの位置設定
	Set3DSoundListenerPosAndFrontPos_UpVecY(m_position, up);

	// 視野角を設定
	SetupCamera_Perspective(Math::DegToRad(m_fov));

#ifndef NDEBUG
	// 最長描画距離が分かるように円を生成
	DrawSphere3D(m_position, m_setFar, 16, GetColor(0, 0, 255), GetColor(0, 0, 0), false);
#endif
}