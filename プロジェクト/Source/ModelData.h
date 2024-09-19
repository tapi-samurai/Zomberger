#pragma once
#include "Vector3.h"
#include <vector>

// 3Dモデルの定数リスト
namespace ModelData
{
	// モデルの定数データを持つ構造体
	struct ModelConstantValue
	{
	public:
		const char* ModelName;
		const int BaseFrameIndex;
		const float OffsetY;
		const Vector3 BaseScale;

		constexpr ModelConstantValue(
			const char* modelName, int baseFrameIndex = 0,
			float offsetY = 0.0f, const Vector3& baseScale = Vector3(1.0f, 1.0f, 1.0f)
		) :
			ModelName(modelName),
			BaseFrameIndex(baseFrameIndex),
			OffsetY(offsetY),
			BaseScale(baseScale) {}
	};

	// モデルの識別用列挙体
	enum class Name
	{
		Player,
		Zombie,
		Scoop,
		Bed,
		Exclamation,
		Arrow,
		Object,
		Kitchen,
		Itembox,
		Sofa,
		Radio,
		ConcreteBarrier,
		ConcreteBarrierIron,
		Lamp,
	};

	enum class ObjectName
	{
		Lamp2,
		BarrelRed,
		BarrelYellow,
		BarrelGreen,
		Crate,

		Length,
	};

	// モデルのデータを持つ配列
	static const std::vector<ModelConstantValue> List =
	{
		{ ModelConstantValue("Resource/model/player/player.mv1", 0, 27.993f) },				// Player
		{ ModelConstantValue("Resource/model/zombie/zombie.mv1", 0, 27.993f) },				// Zombie
		{ ModelConstantValue("Resource/model/scoop/scoop.mv1") },							// Scoop
		{ ModelConstantValue("Resource/model/object/bed/bed.mv1", 0, 58.238f)},				// Bed
		{ ModelConstantValue("Resource/model/object/exclamation/exclamation.mv1")},			// Exclamation
		{ ModelConstantValue("Resource/model/object/arrow/arrow_nocolor.mv1")},				// Arrow
		{ ModelConstantValue("Resource/model/bokoboko/bokoboko.mv1")},						// Object
		{ ModelConstantValue("Resource/model/object/kitchen/kitchen.mv1", 0, 0.9f)},		// Kitchen
		{ ModelConstantValue("Resource/model/object/itembox/itembox.mv1", 0, 19.989f)},		// Itembox
		{ ModelConstantValue("Resource/model/object/sofa/sofa.mv1", 0, 11.605f)},			// Sofa
		{ ModelConstantValue("Resource/model/object/radio/radio.mv1")},						// Radio
		{ ModelConstantValue("Resource/model/object/concrete/concrete_barrier.mv1")},		// ConcreteBarrier
		{ ModelConstantValue("Resource/model/object/concrete/concrete_barrier_iron.mv1")},	// ConcreteBarrierIron
		{ ModelConstantValue("Resource/model/object/lamp/lamp.mv1")},						// Lamp
	};

	// 障害物用のモデルデータを持つ配列
	static const std::vector<ModelConstantValue> Obstacles =
	{
		{ ModelConstantValue("Resource/model/object/lamp/lamp2.mv1", 0, 0, Vector3(0.1f, 0.1f, 0.1f))},					// Lamp2
		{ ModelConstantValue("Resource/model/object/barrel/barrel_damage_red.mv1", 0, 0, Vector3(0.1f, 0.1f, 0.1f))},	// BarrelRed
		{ ModelConstantValue("Resource/model/object/barrel/barrel_yellow.mv1", 0, 0, Vector3(0.1f, 0.1f, 0.1f))},		// BarrelYellow
		{ ModelConstantValue("Resource/model/object/barrel/barrel_damage_green.mv1", 0, 0, Vector3(0.1f, 0.1f, 0.1f))},	// BarrelGreen
		{ ModelConstantValue("Resource/model/object/crate/crate.mv1", 0, 0, Vector3(0.06f, 0.06f, 0.06f))},				// Crate
	};
}