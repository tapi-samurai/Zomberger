#include "Model.h"
#include "Time.h"
#include "ModelLoader.h"

// モデルとアニメーションの読み込み
void Model::Load()
{
	// モデルデータがないなら何もロードしない
	if (!m_modelName) return;

	// モデル
	m_modelHandle = ModelLoader::GetInstance()->Load(m_modelName);

	// アニメーション
	for (auto& pair : m_animationList)
	{
		// リストから＜アニメーション名：アニメーションデータ＞のペアを取り出す
		// pair.first ：アニメーション名
		// pair.second：アニメーションデータ

		// アニメーションデータからアニメーションパスを取り出してロード
		pair.second.animationId = ModelLoader::GetInstance()->Load(pair.second.animationName);

		// 最初に駆動するアニメは初めて登録したもの
		if (!m_runningAnime)
		{
			m_runningAnime = &m_animationList.begin()->second;
			m_attachIdx = MV1AttachAnim(m_modelHandle, 0, m_runningAnime->animationId);
		}
	}

	// アニメーションによるモデルの移動を設定
	SetFrameUserLocalMatrix();
}

// モデルとアニメーションの解放
void Model::Release()
{
	if (m_modelHandle)
	{
		// オリジナルデータ
		ModelLoader::GetInstance()->Delete(m_modelName, m_modelHandle);
	}

	for (auto& pair : m_animationList)
	{
		// オリジナルデータ
		ModelLoader::GetInstance()->Delete(pair.second.animationName, pair.second.animationId);
	}
}

// 更新
void Model::Update()
{
	// アニメーションが終了していないか
	if (IsFinishAnime()) return;

	// 経過時間が総アニメーション時間を超えたらループ処理
	m_elapsedCount += Time::GetInstance()->GetLocalDeltaTime() * m_runningAnime->playSpeed * m_runningAnime->CountSec;

	if (IsFinishAnime())
	{
		if (m_runningAnime->loopFlag)
		{
			// ループ有りなら最初のコマに戻る
			m_elapsedCount = 0;
		}
	}
}

// 描画
void Model::Draw(const Transform3D& transform)
{
	// モデルがなければ何もしない
	if (!m_modelHandle) return;

	// 座標と回転と拡縮を取得
	Vector3 position = transform.position;
	Quaternion rotation = transform.rotation;
	Vector3 scale = transform.scale;

	// アニメーションがあれば設定
	if (m_runningAnime)
	{
		// アニメーションを設定
		MV1SetAttachAnimTime(m_modelHandle, m_attachIdx, m_elapsedCount);
	}

	// モデルの情報を行列で設定
	MATRIX matrix = MGetIdent();
	matrix = MMult(matrix, MGetScale(Vector3(scale.x, scale.y, scale.z)));	// 拡縮
	matrix = MMult(matrix, rotation);								// 回転
	matrix = MMult(matrix, MGetTranslate(position));				// 平行移動　の順を厳守
	MV1SetMatrix(m_modelHandle, matrix);

	// 表示
	MV1DrawModel(m_modelHandle);
}

// モデル登録
void Model::ModelRegister(const char* modelName, int baseFrameIndex)
{
	m_modelName = modelName;
	m_baseFrameIndex = baseFrameIndex;
}

// アニメーション登録
void Model::AnimeRegister(const char* animeName, const Animation3D& animation)
{
	// アニメーションの登録
	m_animationList.emplace(animeName, animation);
}

// アニメーション再生
void Model::Play(const char* animeName)
{
	// 登録されたアニメーションをリストから取得
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
	MV1DetachAnim(m_modelHandle, m_attachIdx);
	m_attachIdx = MV1AttachAnim(m_modelHandle, 0, m_runningAnime->animationId);
	m_elapsedCount = 0;

	// アニメーションによるモデルの移動を設定
	SetFrameUserLocalMatrix();
}

void Model::SetFrameUserLocalMatrix()
{
	// アニメーションが無ければスキップ
	if (m_runningAnime == nullptr) return;

	// モデルの指定フレーム座標に基点座標を合わせる
	if (m_runningAnime->animationMove == false)
	{
		MV1SetFrameUserLocalMatrix(m_modelHandle, m_baseFrameIndex, MGetIdent());
	}
	else
	{
		MV1ResetFrameUserLocalMatrix(m_modelHandle, m_baseFrameIndex);
	}
}