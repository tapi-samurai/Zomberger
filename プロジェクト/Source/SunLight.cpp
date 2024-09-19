#include "SunLight.h"
#include "Screen.h"
#include "TimeProgressManager.h"
#include "DxLib.h"

SunLight::SunLight()
{
	// �摜�̐F���`
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

	// �`��̓����x��ύX
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_sunLightData[TimeProgressManager::GetInstance()->GetElapsedTimezone()].alpha);

	// ���݂̎��ԑтɍ��킹���F�̋�`��\��
	DrawBox(0, 0, Screen::Width, Screen::Height, m_sunLightData[TimeProgressManager::GetInstance()->GetElapsedTimezone()].color, true);

	// �`��̓����x�����Z�b�g
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}