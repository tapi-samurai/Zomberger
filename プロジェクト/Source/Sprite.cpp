#include "Sprite.h"
#include "DxLib.h"
#include "Time.h"
#include "SpriteLoader.h"

// 画像の読み込み
void Sprite::Load()
{
	int beginTexture = 0;
	for (auto& pair : m_animationList)
	{
		// リストから＜アニメーション名：アニメーションデータ＞のペアを取り出す
		// pair.first ：アニメーション名
		// pair.second：アニメーションデータ

		// アニメーションデータから画像パスを取り出してロード
		pair.second.textureId = SpriteLoader::GetInstance()->Load(pair.second.textureName);

		if (!beginTexture)
		{
			beginTexture = pair.second.textureId;
		}
	}

	// グリッドサイズを指定しなかった場合は画像からサイズを取得する
	if(gridSize.IsZero())
	{
		int x, y;
		GetGraphSize(beginTexture, &x, &y);
		gridSize = Vector2(x, y);
	}
}

// 画像の解放
void Sprite::Release()
{
	for (auto& pair : m_animationList)
	{
		SpriteLoader::GetInstance()->Delete(pair.second.textureName);
	}
}

// 更新
void Sprite::Update()
{
	// アニメーション終了していないか
	if(IsFinishAnime())
	{
		return;
	}

	// 経過時間が総アニメーション時間を超えたらループ処理
	m_elapsedTime += Time::GetInstance()->GetDeltaTime();
	if(IsFinishAnime())
	{
		if(m_runningAnime->loopFlag)
		{
			// ループ有りなら最初のコマに戻る
			m_elapsedTime = 0;
		}
	}
}

// 描画
void Sprite::Draw(const Transform& transform, DrawMode mode)
{
	// 再生するアニメーションがなければ何もしない
	if (!m_runningAnime) return;

	// 描画するコマを計算
	int gridIndex;
	if(!IsFinishAnime())
	{
		// 経過時間に応じたコマ番号
		gridIndex = (int)(m_elapsedTime / GetFrameTime());
	}
	else
	{
		// アニメーションが終了していたら最後のコマ番号
		gridIndex = m_runningAnime->gridNum - 1;
	}

	int offsetX = 0;
	int offsetY = 0;
	int srcX = (int)(gridSize.x * gridIndex);
	int srcY = 0;
	int width = (int)gridSize.x;
	int height = (int)gridSize.y;

	// 描画
	switch (mode)
	{
	case DrawMode::RectRota:
		DrawRectRotaGraph(
			(int)transform.position.x + offsetX, (int)transform.position.y + offsetY,
			srcX, srcY,
			width, height,
			transform.scale,
			transform.rotation.z,
			m_runningAnime->textureId,
			true,
			flipX, flipY
		);
		break;

	case DrawMode::Extend:
		DrawExtendGraph(
			(int)transform.position.x - width / 2, (int)transform.position.y - height / 2,
			(int)transform.position.x + width / 2, (int)transform.position.y + height / 2,
			m_runningAnime->textureId, true
		);
		break;
	}

#ifndef NDEBUG
	// debug:grid枠の表示
	DrawBox(
		(int)(transform.position.x - gridSize.x * transform.scale / 2),
		(int)(transform.position.y - gridSize.y * transform.scale / 2),
		(int)(transform.position.x + gridSize.x * transform.scale / 2),
		(int)(transform.position.y + gridSize.y * transform.scale / 2),
		GetColor(255, 0, 0), false);
	// debug:基点の表示
	DrawCircle((int)transform.position.x, (int)transform.position.y, 2, 0);
#endif
}

void Sprite::Draw(const Vector2& position, DrawMode mode)
{
	Transform transform;
	transform.position = position;

	Draw(transform, mode);
}

// アニメーション登録
void Sprite::Register(const char* textureName)
{
	Register("-", Animation(textureName, 1, 0, false));
}

void Sprite::Register(const char* animeName, const Animation& animation)
{
	// アニメーションの登録
	m_animationList.emplace(animeName, animation);

	// 最初に駆動するアニメは初めて登録したもの
	if (!m_runningAnime)
	{
		m_runningAnime = &m_animationList.begin()->second;
	}
}

// アニメーション再生
void Sprite::Play(const char* animeName)
{
	// 指定されたアニメーションをリストから取得
	const auto& pair = m_animationList.find(animeName);

	// 登録されていないアニメ
	if (pair == m_animationList.end())
	{
		return;
	}

	// 現在と同じアニメ
	if (m_runningAnime == &pair->second)
	{
		return;
	}

	// アニメーション切り替え
	m_runningAnime = &pair->second;
	m_elapsedTime = 0;
}
