#include "SunLight.h"
#include "Screen.h"
#include "TimeProgressManager.h"
#include "DxLib.h"

SunLight::SunLight()
{
	// 画像の色を定義
	m_sunLightData = std::vector<SunLightSetting>
	{
		SunLightSetting(GetColor(255, 255, 255), 100),
		SunLightSetting(GetColor(255, 255, 255), 50),
		SunLightSetting(GetColor(255, 255, 255), 0),
		SunLightSetting(GetColor(202, 78, 0), 50),
		SunLightSetting(GetColor(0, 0, 0), 120),
	};
}

void SunLight::Draw()
{
	Node::Draw();

	// 描画の透明度を変更
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_sunLightData[TimeProgressManager::GetInstance()->GetElapsedTimezone()].alpha);

	// 現在の時間帯に合わせた色の矩形を表示
	DrawBox(0, 0, Screen::Width, Screen::Height, m_sunLightData[TimeProgressManager::GetInstance()->GetElapsedTimezone()].color, true);

	// 描画の透明度をリセット
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}