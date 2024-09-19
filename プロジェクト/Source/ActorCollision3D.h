#pragma once
#include <list>

class Actor3D;

// ���f���Ƃ��̑�(�����E���E�J�v�Z��)�̃��X�g�ǂ����𑍓�����Ŕ���
class ActorCollision3D
{
private:
	std::list<Actor3D*> m_actorList;	// �����蔻�������Actor���X�g

public:
	// �V���O���g��
	static ActorCollision3D* GetInstance()
	{
		static ActorCollision3D instance;
		return &instance;
	}

	// ���f���̓o�^
	void Register(Actor3D* actor)
	{
		m_actorList.push_back(actor);
	}

	// �폜
	void Remove(Actor3D* actor)
	{
		m_actorList.remove(actor);
	}

	// �X�V
	void Update();

	// �����蔻��͈̔͂�`��
	void DebugDraw() const;
};