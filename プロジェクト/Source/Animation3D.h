#pragma once

class Animation3D
{
public:
	static constexpr float CountSec = 30.0f;	// �A�j���[�V��������b�ŉ��R�}�i�߂邩

	const char* animationName;	// �摜�̃p�X
	int animationId;			// �A�j���[�V�����̃f�[�^
	int playSpeed;				// �Đ����x�̔{��
	bool loopFlag;				// �A�j���[�V���������[�v���邩�ǂ���
	bool animationMove;			// �A�j���[�V�����ɂ��ړ��������邩�ǂ���

	Animation3D(const char* animationName, bool loopFlag = true, int playSpeed = 1, bool animationMove = true) :
		animationName(animationName),
		animationId(0),
		playSpeed(playSpeed),
		loopFlag(loopFlag),
		animationMove(animationMove) {}
};