#include "SceneCamp.h"
#include "Node.h"
#include "Camera.h"
#include "Math.h"
#include "Quad3D.h"
#include "ItemField.h"
#include "SceneTransitionQuad.h"
#include "PlayerController.h"
#include "PlayerInteractLandmark.h"
#include "ModelData.h"
#include "SceneField.h"
#include "StageData.h"
#include "UiInventory.h"
#include "UiItembox.h"
#include "UiKitchen.h"
#include "SceneResult.h"
#include "UiElapsedDayTime.h"
#include "TimeProgressManager.h"
#include "NoActionObject.h"
#include "SleepBed.h"
#include "UiSwitchObject.h"
#include "PlayerState.h"
#include "UiTrader.h"
#include "PlayerScoopAttack.h"
#include "Time.h"
#include "SunLight.h"
#include "UiHealth.h"
#include "UiSatietyLevel.h"
#include "UiInteractable.h"
#include "NoActionTrader.h"
#include "ModelDisplay.h"
#include "CameraData.h"
#include "ItemField.h"
#include "ItemData.h"
#include "SoundLoader.h"
#include "SoundEffect.h"
#include "SoundData.h"
#include "UiTextMessage.h"

void SceneCamp::Initialize()
{
	m_rootNode = new Node();

	// ���ԑт�i�߂�
	if (m_addTimezoneValue != 0)
	{
		TimeProgressManager::GetInstance()->AddElapsedTimezone(m_addTimezoneValue);
	}
	else
	{
		TimeProgressManager::GetInstance()->TimezoneInitialization();
	}

	// �g���[�_�[���o�����邩���I
	m_canSpawnTrader = static_cast<int>(rand() % 100) < AppearanceRateAsTrader ? true : false;

	// �C���x���g������
	UiInventory* uiInventory = new UiInventory();

	// �C���^���N�g�\UI
	UiInteractable* const uiInteractable = new UiInteractable();

	// �e�L�X�g���b�Z�[�WUI
	UiTextMessage* const uiTextMessage = new UiTextMessage();

	// �J����
	int cameraNum = rand() % CameraData::Camp.size();
	m_rootNode->AddChild(new Camera(
		CameraData::Camp[cameraNum].CameraPosition,
		Quaternion::CreateRotationAxis(Vector3::Right(), Math::DegToRad(CameraData::Camp[cameraNum].CameraAngleX)) *
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(CameraData::Camp[cameraNum].CameraAngleY)) *
		Quaternion::CreateRotationAxis(Vector3::Forward(), Math::DegToRad(CameraData::Camp[cameraNum].CameraAngleZ)),
		1.0f, 200.0f
	));

	// ��
	m_rootNode->AddChild(new Quad3D(
		Vector3(), Quaternion::CreateRotationAxis(Vector3::Right(), Math::DegToRad(90.0f)), FloorHeight, FloorWidth,
		"Floor", StageData::List[static_cast<int>(StageData::Name::Camp)].FloorTextureName)
	);

	// �ǂƃV�[���J�ڗp�̓����蔻��𐶐�
	for (int i = 0; i < 4; i++)
	{
		// �\���p�̕ϐ���p��
		float length = (i + 1) % 2 == 0 ? FloorHeight / 2 : FloorWidth / 2;
		float wallWidth = (i + 1) % 2 == 0 ? FloorWidth : FloorHeight;
		bool isDisplay = i == 1 || i == 2 ? true : false;
		Quaternion rotation = Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(-90.0f + 90.0f * i));

		// �����玞�v���ɕǂ𐶐�
		m_rootNode->AddChild(new Quad3D(
			Vector3::Forward() * length * rotation + Vector3(0, WallHeight / 2, 0),
			rotation, WallHeight, wallWidth,
			"Wall", StageData::List[static_cast<int>(StageData::Name::Camp)].WallTextureName, isDisplay, GetColorU8(100, 100, 100, 200)
		));

		// �ŏ��ɐ��������ǂ̋߂��ɂ̂݃V�[���J�ڗp�̓����蔻������I�u�W�F�N�g�𐶐�
		// ��ɂȂ��Ă����琶�����Ȃ�
		if (i == 0 && TimeProgressManager::GetInstance()->GetElapsedTimezone() != TimeProgressManager::MaxTimezone)
		{
			m_rootNode->AddChild(new SceneTransitionQuad(
				uiInteractable, uiTextMessage, m_nextStagePointer, rand() % static_cast<int>(StageData::Name::Length),
				Vector3::Forward() * (length - SceneTransitionQuadToWallLength) * rotation + Vector3(0, WallHeight / 2, 0),
				rotation, WallHeight ,wallWidth
			));
		}
	}

	// �����Ȃ���(�x�b�h����E�[�̕ǂ܂�)
	m_rootNode->AddChild(new Quad3D(
		InvisibleWall1Position,
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(InvisibleWall1AngleY)),
		WallHeight, Wall1Width, "Wall", nullptr, false
	));

	// �����Ȃ���(�E�[�̕ǂ̎�O)
	m_rootNode->AddChild(new Quad3D(
		InvisibleWall2Position,
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(InvisibleWall2AngleY)),
		WallHeight, Wall2Width, "Wall", nullptr, false
	));

	// �v���C���[
	Vector3 playerPos = m_playerPos;
	Quaternion playerRot = Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(PlayerInitialAngleY));
	m_playerController = new PlayerController(playerPos, playerRot, uiInventory);
	m_rootNode->AddChild(m_playerController);

	// �X�R�b�v�U���I�u�W�F�N�g
	PlayerScoopAttack* playerScoopAttack = new PlayerScoopAttack(m_playerController);
	m_playerController->AddChild(playerScoopAttack);

	// �C���^���N�g�̌��m�ΏۃI�u�W�F�N�g
	m_playerController->AddChild(new PlayerInteractLandmark(m_playerController));
	
	// �x�b�h
	m_rootNode->AddChild(new SleepBed(
		uiInteractable, uiTextMessage, BedPosition, Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(BedAngleY)), m_nextStagePointer
	));

	// �A�C�e���{�b�N�X
	UiItembox* uiItembox = new UiItembox();
	m_rootNode->AddChild(new UiSwitchObject(
		uiItembox, uiInventory, uiInteractable, ItemboxPosition, Vector3(1, 1, 1), ItemboxInteractRadius)
	);

	// �A�C�e���{�b�N�X�̓����蔻��
	m_rootNode->AddChild(new NoActionObject(
		ItemboxPosition + Vector3(0, ModelData::List[static_cast<int>(ModelData::Name::Itembox)].OffsetY * ItemboxScale.y, 0),
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(ItemboxAngleY)), ItemboxScale,
		"Object", ModelData::List[static_cast<int>(ModelData::Name::Itembox)].ModelName, ItemboxInteractRadius
	));

	// �L�b�`��
	UiKitchen* uiKitchen = new UiKitchen();
	m_rootNode->AddChild(new UiSwitchObject(
		uiKitchen, uiInventory, uiInteractable, KitchenPosition, KitchenScale, KitchenInteractRadius)
	);
	m_rootNode->AddChild(new NoActionObject(
		KitchenPosition + Vector3(0, ModelData::List[static_cast<int>(ModelData::Name::Kitchen)].OffsetY * KitchenScale.y, 0),
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(KitchenAngleY)), KitchenScale,
		"Object", ModelData::List[static_cast<int>(ModelData::Name::Kitchen)].ModelName, KitchenCollisionRadius
	));

	// �g���[�_�[
	UiTrader* uiTrader = nullptr;
	if (m_canSpawnTrader)
	{
		uiTrader = new UiTrader();
		m_rootNode->AddChild(new NoActionTrader());
		m_rootNode->AddChild(new UiSwitchObject(
			uiTrader, uiInventory, uiInteractable, NoActionTrader::Position, NoActionTrader::Scale, NoActionTrader::InteractRadius));
	}

	// �����ڂ݂̂̃I�u�W�F�N�g --------------------------------------------------------------------

	// �\�t�@
	m_rootNode->AddChild(new ModelDisplay(
		"sofa",
		ModelData::List[static_cast<int>(ModelData::Name::Sofa)].ModelName,
		SofaPosition + Vector3(0, ModelData::List[static_cast<int>(ModelData::Name::Sofa)].OffsetY * SofaScale.y, 0),
		Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(SofaAngleY)), SofaScale
	));

	// ���W�I
	m_rootNode->AddChild(new ModelDisplay(
		"radio",
		ModelData::List[static_cast<int>(ModelData::Name::Radio)].ModelName,
		RadioPosition, Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(RadioAngleY)), RadioScale
	));

	// �R���N���P
	m_rootNode->AddChild(new ModelDisplay(
		"concrete",
		ModelData::List[static_cast<int>(ModelData::Name::ConcreteBarrier)].ModelName,
		Concrete1Position, Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(Concrete1AngleY)), Concrete1Scale
	));

	// �R���N���Q
	m_rootNode->AddChild(new ModelDisplay(
		"concrete",
		ModelData::List[static_cast<int>(ModelData::Name::ConcreteBarrierIron)].ModelName,
		Concrete2Position, Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(Concrete2AngleY)), Concrete2Scale
	));

	// �����v
	m_rootNode->AddChild(new ModelDisplay(
		"lamp",
		ModelData::List[static_cast<int>(ModelData::Name::Lamp)].ModelName,
		LampPosition, Quaternion::CreateRotationAxis(Vector3::Up(), Math::DegToRad(LampAngleY)), LampScale
	));

	// UI ------------------------------------------------------------------------------------------

	// ���z��
	m_rootNode->AddChild(new SunLight());

	// �̗�
	m_rootNode->AddChild(new UiHealth());

	// �󕠓x
	m_rootNode->AddChild(new UiSatietyLevel());

	// �C���x���g��
	m_rootNode->AddChild(uiInventory);

	// �A�C�e���{�b�N�X
	m_rootNode->AddChild(uiItembox);

	// �L�b�`��
	m_rootNode->AddChild(uiKitchen);

	if (m_canSpawnTrader && uiTrader != nullptr)
	{
		// �g���[�_�[
		m_rootNode->AddChild(uiTrader);
	}

	// �C���^���N�g�\UI
	m_rootNode->AddChild(uiInteractable);

	// �o�ߓ����Ǝ��ԑ�
	m_rootNode->AddChild(new UiElapsedDayTime());

	// �e�L�X�g���b�Z�[�WUI
	m_rootNode->AddChild(uiTextMessage);

	// SE�ǂݍ���
	m_seTransition = SoundLoader::GetInstance()->Load(SoundData::List[static_cast<int>(SoundData::Name::Transition)].SoundName, false);

	// SE�ݒ�
	SoundEffect::GetInstance()->SetSoundEffect2D(m_seTransition, SoundData::List[static_cast<int>(SoundData::Name::Transition)].volume);
}

void SceneCamp::Finalize()
{
	// �V�[���J�ڒ��̃t�F�[�h�A�E�g���ɌĂ΂��
	// ���[�h�����摜�̉�������Ȃǂ��s��

	// SE�폜
	SoundLoader::GetInstance()->Delete(SoundData::List[static_cast<int>(SoundData::Name::Transition)].SoundName);

	m_rootNode->TreeRelease();
	delete m_rootNode;
	m_rootNode = nullptr;
}

SceneBase* SceneCamp::Update()
{
	// �V�[�����̌v�Z�������s��

	// �m�[�h�̍X�V
	m_rootNode->TreeUpdate();

	// �V�[���J�ڂ��\�ɂȂ��Ă�����
	if (m_nextStagePointer != nullptr)
	{
		// �J�ڐ悪�L�����v�V�[�����ǂ�������
		if (*m_nextStagePointer == static_cast<int>(StageData::Name::Camp))
		{
			// �󕠓x������炷
			PlayerState::GetInstance()->SetSatietyLevel(-1);

			// �󕠓x��0�ɂȂ�����Q�[���I�[�o�[
			if (PlayerState::GetInstance()->GetSatietyLevel() <= 0)
			{
				return new SceneResult();
			}
			else
			{
				return new SceneCamp(0, m_playerController->GetPosition());
			}
		}
		else
		{
			// SE
			PlaySoundMem(m_seTransition, DX_PLAYTYPE_BACK);

			return new SceneField(*m_nextStagePointer, 0);
		}
	}

	return this;
}

void SceneCamp::Draw()
{
	// �V�[�����̕`�揈�����s��

	// �m�[�h�̕`��
	m_rootNode->TreeDraw();

#ifndef NDEBUG
	// DEBUG:��_�̕\��
	DrawLine3D(Vector3(), Vector3(25, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(Vector3(), Vector3(0, 25, 0), GetColor(0, 255, 0));
	DrawLine3D(Vector3(), Vector3(0, 0, 25), GetColor(0, 0, 255));
#endif
}