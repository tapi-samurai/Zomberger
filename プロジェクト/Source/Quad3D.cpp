#include "Quad3D.h"
#include "QuadCollider3D.h"
#include "SpriteLoader.h"
#include "Math.h"

Quad3D::Quad3D(
	const Vector3& position, const Quaternion& rotation, float height, float width,
	const char* name, const char* textureName, bool isDisplay, const COLOR_U8& color
	) :
	Actor3D(name),
	m_textureName(textureName),
	m_height(height),
	m_width(width),
	m_textureHandle(0),
	m_isDisplay(isDisplay)
{
	m_transform.position = position;
	m_transform.rotation = rotation;

	// コライダーを生成
	m_collider = new QuadCollider3D(Vector3(), height, width);

	// 頂点配列を初期化
	m_vertex[0].pos = m_transform.position + Vector3(-(m_width / 2), m_height / 2, 0) * m_transform.rotation;
	m_vertex[0].u = 0.0f;
	m_vertex[0].v = 0.0f;

	m_vertex[1].pos = m_transform.position + Vector3(m_width / 2, m_height / 2, 0) * m_transform.rotation;
	m_vertex[1].u = 1.0f;
	m_vertex[1].v = 0.0f;

	m_vertex[2].pos = m_transform.position + Vector3(m_width / 2, -m_height / 2, 0) * m_transform.rotation;
	m_vertex[2].u = 1.0f;
	m_vertex[2].v = 1.0f;

	m_vertex[3].pos = m_transform.position + Vector3(-(m_width / 2), -(m_height / 2), 0) * m_transform.rotation;
	m_vertex[3].u = 0.0f;
	m_vertex[3].v = 1.0f;

	// 座標とUV以外は共通の値で初期化
	for (int i = 0; i < VertexNum; i++)
	{
		m_vertex[i].norm = m_transform.Up();
		m_vertex[i].dif = color;
		m_vertex[i].spc = GetColorU8(0, 0, 0, 0);
	}
}

void Quad3D::Load()
{
	// 本来のロード処理
	Actor3D::Load();

	// 床用のテクスチャの読み込み
	// テクスチャがなければ画像なしの値を代入
	m_textureHandle = m_textureName == nullptr ? DX_NONE_GRAPH : SpriteLoader::GetInstance()->Load(m_textureName);
}

void Quad3D::Release()
{
	// 本来のリリース処理
	Actor3D::Release();

	if (m_textureName != nullptr)
	{
		// テクスチャの削除
		SpriteLoader::GetInstance()->Delete(m_textureName);
	}
}

void Quad3D::Update()
{
	// 本来の更新処理
	Actor3D::Update();

	// 頂点座標を更新
	m_vertex[0].pos = m_transform.position + Vector3(-(m_width / 2), m_height / 2, 0) * m_transform.rotation;
	m_vertex[1].pos = m_transform.position + Vector3(m_width / 2, m_height / 2, 0) * m_transform.rotation;
	m_vertex[2].pos = m_transform.position + Vector3(m_width / 2, -(m_height / 2), 0) * m_transform.rotation;
	m_vertex[3].pos = m_transform.position + Vector3(-(m_width / 2), -(m_height / 2), 0) * m_transform.rotation;
}

void Quad3D::Draw()
{
	// 本来の描画処理
	Actor3D::Draw();

	// 画像を表示
	if (m_isDisplay)
	{
		DrawPolygonIndexed3D(m_vertex, VertexNum, Index, IndexNum, m_textureHandle, true);
	}
}