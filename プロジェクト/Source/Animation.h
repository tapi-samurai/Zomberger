#pragma once

class Animation
{
public:
	const char* textureName;	// 画像のパス
	int textureId;				// 描画する画像
	int gridNum;				// アニメーションのコマ数
	int sampleRate;				// 1秒間にアニメーションのコマを切り替える回数
	bool loopFlag;				// アニメーションをループするかどうか

	Animation(const char* textureName, int gridNum = 1, int sampleRate = 10, bool loopFlag = true) :
		textureName(textureName),
		textureId(0),
		gridNum(gridNum),
		sampleRate(sampleRate),
		loopFlag(loopFlag) {}
};
