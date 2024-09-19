#pragma once

class Animation3D
{
public:
	static constexpr float CountSec = 30.0f;	// アニメーションを一秒で何コマ進めるか

	const char* animationName;	// 画像のパス
	int animationId;			// アニメーションのデータ
	int playSpeed;				// 再生速度の倍率
	bool loopFlag;				// アニメーションをループするかどうか
	bool animationMove;			// アニメーションによる移動を許可するかどうか

	Animation3D(const char* animationName, bool loopFlag = true, int playSpeed = 1, bool animationMove = true) :
		animationName(animationName),
		animationId(0),
		playSpeed(playSpeed),
		loopFlag(loopFlag),
		animationMove(animationMove) {}
};