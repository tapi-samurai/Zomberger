#pragma once
#include "Transform3D.h"
#include "Animation3D.h"
#include <map>

/*
* Spriteクラスの3Dバージョン
*/

class Model
{
private:
	const char* m_modelName;							// モデルのパス
	int m_modelHandle;									// モデルデータ
	std::map<const char*, Animation3D> m_animationList;	// ＜アニメーション名：アニメーションデータ＞ペアのリスト
	const Animation3D* m_runningAnime;					// 再生中のアニメーションリストへのポインタ
	int m_attachIdx;									// アタッチされているアニメーションの番号
	float m_elapsedCount;								// 経過時間(コマ数)
	int m_baseFrameIndex;								// アニメーションによる移動を許可しない場合の基点フレーム番号

	void SetFrameUserLocalMatrix();	// モデルの指定フレーム座標に基点座標を合わせる

public:
	// コンストラクタ
	Model() :
		m_modelName(nullptr),
		m_modelHandle(0),
		m_runningAnime(nullptr),
		m_attachIdx(0),
		m_elapsedCount(0),
		m_baseFrameIndex(0) {}

	// モデルの読み込み
	void Load();

	// モデルの解放
	void Release();

	// 更新
	void Update();

	// 描画
	void Draw(const Transform3D& transform);

	// モデル登録
	void ModelRegister(const char* modelName, int baseFrameIndex);

	// アニメーション登録
	void AnimeRegister(const char* animeName, const Animation3D& animation);

	// アニメーション再生
	void Play(const char* animeName);

	// アニメーションの現在時間を取得
	float GetAnimeTime() { return m_elapsedCount; }

	// アニメーションの総再生時間を取得
	float GetAnimeTotalTime() { return m_runningAnime ? MV1GetAttachAnimTotalTime(m_modelHandle, m_attachIdx) : 0.0f; }

	// モデルハンドルを取得
	int GetModelHandle() { return m_modelHandle; }

	// アニメーションが終了したかどうか
	bool IsFinishAnime() { return m_runningAnime ? m_elapsedCount >= GetAnimeTotalTime() : true; }
 };
