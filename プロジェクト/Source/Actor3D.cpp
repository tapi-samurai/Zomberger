#include "Actor3D.h"
#include "ActorCollision3D.h"
#include "BitFlag.h"

#include <typeinfo>

// コンストラクタ
Actor3D::Actor3D(const char* name, const char* modelName, int baseFrameIndex, bool isActive) :
	m_name(name),
	m_model(nullptr),
	m_collider(nullptr),
	m_bitFlag(nullptr),
	m_isActive(isActive)
{;
	if (modelName)
	{
		m_model = new Model();
		m_model->ModelRegister(modelName, baseFrameIndex);
	}
}

// リソースの読み込み
void Actor3D::Load()
{
	if (m_model)
	{
		m_model->Load();
	}

	// 当たり判定システムに登録
	if (m_collider)
	{
		ActorCollision3D::GetInstance()->Register(this);
	}
}

// リソースの解放
void Actor3D::Release()
{
	if (m_model)
	{
		m_model->Release();
		delete m_model;
		m_model = nullptr;
	}

	// 当たり判定システムから除外
	if (m_collider)
	{
		ActorCollision3D::GetInstance()->Remove(this);
	}

	// ビットフラグを保持していれば削除
	if (m_bitFlag)
	{
		delete m_bitFlag;
		m_bitFlag = nullptr;
	}
}

// 描画
void Actor3D::Draw()
{
	if (m_isActive == false) return;

	if (m_model)
	{
		m_model->Draw(m_transform);
	}

#ifndef NDEBUG
	// DEBUG:基点の表示
	DrawLine3D(m_transform.position, m_transform.position + Vector3(3, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(m_transform.position, m_transform.position + Vector3(0, 3, 0), GetColor(0, 255, 0));
	DrawLine3D(m_transform.position, m_transform.position + Vector3(0, 0, 3), GetColor(0, 0, 255));
#endif
}

// 子ノードを含む更新
void Actor3D::TreeUpdate()
{
	// Node本来のTreeUpdateを実行
	Node::TreeUpdate();

	if (m_isActive == false) return;

	// モデルの更新
	if (m_model)
	{
		m_model->Update();
	}
}
