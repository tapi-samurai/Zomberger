#include "NoActionTrader.h"
#include "ModelData.h"
#include "CapsuleCollider3D.h"
#include "Math.h"

NoActionTrader::NoActionTrader() :
	Actor3D("Object", ModelData::List[static_cast<int>(ModelData::Name::Zombie)].ModelName)
{
	// Transformを設定
	m_transform.position = Position;
	m_transform.rotation = Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(RotationAngleY));
	m_transform.scale = Scale;

	// コライダーを設定
	m_collider = new CapsuleCollider3D(
		Vector3(0, ModelData::List[static_cast<int>(ModelData::Name::Zombie)].OffsetY * Scale.y, 0),
		ColliderPos2Offset, ColliderRadius
	);

	// アニメーション登録
	for (int animeNum = 0; animeNum < static_cast<int>(Anime::Length); animeNum++)
	{
		m_model->AnimeRegister(AnimeName[animeNum], AnimeData[animeNum]);
	}
}