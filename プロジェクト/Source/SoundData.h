#pragma once
#include <vector>

// サウンドデータの定数リスト
namespace SoundData
{
	// サウンドの定数データを持つ構造体
	struct SoundConstantValue
	{
	public:
		const char* SoundName;
		const int volume;
		const int FrequencyPal;	// デフォルトの再生周波数

		constexpr SoundConstantValue(const char* soundName, int volume = 100, int frequencyPal = 44100) :
			SoundName(soundName),
			volume(volume),
			FrequencyPal(frequencyPal) {}
	};

	// サウンドの識別用列挙体
	enum class Name
	{
		Bgm,

		ScoopSwing,
		ScoopHit,
		ItemClick,
		ItemEat,
		Cook,
		Bed,
		Transition,
		ZombieSpawn,
	};

	// サウンドのデータを持つList
	static const std::vector<SoundConstantValue> List
	{
		{ SoundConstantValue("Resource/sound/zomburger_bgm.wav", 40) },
		{ SoundConstantValue("Resource/sound/scoop_swing.wav", 100, 48000) },
		{ SoundConstantValue("Resource/sound/scoop_hit.wav", 100, 48000) },
		{ SoundConstantValue("Resource/sound/item_click.wav") },
		{ SoundConstantValue("Resource/sound/item_eat.wav") },
		{ SoundConstantValue("Resource/sound/cook.wav") },
		{ SoundConstantValue("Resource/sound/bed.wav") },
		{ SoundConstantValue("Resource/sound/transition.wav", 50) },
		{ SoundConstantValue("Resource/sound/zombie_spawn.wav", 100, 22050) },
	};

	static const std::vector<SoundConstantValue> ZombieVoice
	{
		{ SoundConstantValue("Resource/sound/zombie_voice1.wav", 50) },
		{ SoundConstantValue("Resource/sound/zombie_voice2.wav", 50) },
		{ SoundConstantValue("Resource/sound/zombie_voice3.wav", 50) },
	};
}