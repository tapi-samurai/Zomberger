#include "ActorCollision3D.h"
#include "Collider3D.h"
#include "Actor3D.h"

void ActorCollision3D::Update()
{
	// �r���Ń��X�g�ւ̒ǉ���폜���������Ă������悤�ɃR�s�[�ŉ�
	list<Actor3D*> actorList(m_actorList);

	for (auto it1 = actorList.begin(); it1 != actorList.end(); it1++)
	{
		Actor3D* actor1 = *it1;

		for (auto it2 = it1; it2 != actorList.end(); it2++)
		{
			Actor3D* actor2 = *it2;

			// �����蔻�肪����������X�L�b�v
			// �����蔻�肪����������Ă�����X�L�b�v
			if (actor1->GetCollider3D() == nullptr || actor1->GetCollider3D()->isEnabled == false) continue;
			if (actor2->GetCollider3D() == nullptr || actor2->GetCollider3D()->isEnabled == false) continue;

			// �������g�̓X�L�b�v
			if (actor1 == actor2) continue;

			// �����蔻��
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
	// �R���W�������郊�X�g���̃R���C�_�[��\��
	for (Actor3D* model : m_actorList)
	{
		model->GetCollider3D()->DebugDraw(model->GetPosition(), model->GetRotation());
	}
}
#endif