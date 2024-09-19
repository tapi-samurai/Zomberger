#include "ActorCollision3D.h"
#include "Collider3D.h"
#include "Actor3D.h"

void ActorCollision3D::Update()
{
	// 途中でリストへの追加や削除が発生してもいいようにコピーで回す
	list<Actor3D*> actorList(m_actorList);

	for (auto it1 = actorList.begin(); it1 != actorList.end(); it1++)
	{
		Actor3D* actor1 = *it1;

		for (auto it2 = it1; it2 != actorList.end(); it2++)
		{
			Actor3D* actor2 = *it2;

			// 当たり判定が無かったらスキップ
			// 当たり判定が無効化されていたらスキップ
			if (actor1->GetCollider3D() == nullptr || actor1->GetCollider3D()->isEnabled == false) continue;
			if (actor2->GetCollider3D() == nullptr || actor2->GetCollider3D()->isEnabled == false) continue;

			// 自分自身はスキップ
			if (actor1 == actor2) continue;

			// 当たり判定
			if (actor1->GetCollider3D()->CheckCollision(actor1->GetTransform(), actor2->GetTransform(), actor2->GetCollider3D()))
			{
				actor1->OnCollision3D(actor2);
				actor2->OnCollision3D(actor1);
			}
		}
	}
}

#ifdef _DEBUG
void ActorCollision3D::DebugDraw() const
{
	// コリジョンするリスト内のコライダーを表示
	for (Actor3D* model : m_actorList)
	{
		model->GetCollider3D()->DebugDraw(model->GetPosition(), model->GetRotation());
	}
}
#endif