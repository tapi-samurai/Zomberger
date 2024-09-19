#pragma once

/*
シーン基底クラス
シーンに最低限必要な関数を純粋仮想関数として準備する
*/

class SceneBase
{
public:
	virtual ~SceneBase() {}

	virtual void Initialize() = 0;		// 初期化
	virtual void Finalize() = 0;		// 終了処理
	virtual SceneBase* Update() = 0;	// 更新
	virtual void Draw() = 0;			// 描画
};
