#pragma once
#include "Actor3D.h"

// 当たり判定と画像の表示機能を持つActor3D継承クラス

class Quad3D : public Actor3D
{
private:
	static constexpr int VertexNum = 4;	// 三角形ポリゴンを形成する頂点の数
	static constexpr int IndexNum = 6;	// 頂点インデックスの数

	// 頂点インデックス定数配列
	static constexpr WORD Index[IndexNum] =
	{
		0,1,2,0,3,2,
	};		

	VERTEX3D m_vertex[VertexNum];	// 三角形ポリゴンを形成する頂点配列

	const char* m_textureName;

	float m_height;
	float m_width;

	int m_textureHandle;

	bool m_isDisplay;

protected:
	virtual void Load() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Draw() override;

public:
	Quad3D(
		const Vector3& position, const Quaternion& rotation, float height, float width,
		const char* name, const char* textureName = nullptr, bool isDisplay = true, const COLOR_U8& color = GetColorU8(255, 255, 255, 255)
		);

	const void SetPosition (const Vector3& position) { m_transform.position = position; }
};