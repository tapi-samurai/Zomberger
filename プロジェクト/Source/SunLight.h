#pragma once
#include "Node.h"
#include <vector>

class SunLight : public Node
{
private:
	struct SunLightSetting
	{
	public:
		const unsigned int color;
		const int alpha;

		SunLightSetting(const unsigned int color, const int alpha) :
			color(color),
			alpha(alpha) {}
	};

	std::vector<SunLightSetting> m_sunLightData;

protected:
	void Draw() override;

public:
	SunLight();
};