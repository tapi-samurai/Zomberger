#pragma once
#include <list>

class Actor3D;

// モデルとその他(線分・球・カプセル)のリストどうしを総当たりで判定
class ActorCollision3D
{
private:
	std::list<Actor3D*> m_actorList;	// 当たり判定をするActorリスト

public:
	// シングルトン
	static ActorCollision3D* GetInstance()
	{
		static ActorCollision3D instance;
		return &instance;
	}

	// モデルの登録
	void Register(Actor3D* actor)
	{
		m_actorList.push_back(actor);
	}

	// 削除
	void Remove(Actor3D* actor)
	{
		m_actorList.remove(actor);
	}

	// 更新
	void Update();

	// 当たり判定の範囲を描画
	void DebugDraw() const;
};