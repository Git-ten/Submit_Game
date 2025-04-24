#include <cmath>
#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "../../APPLICATION/SINGLETON_MANAGER/InputManager/SimpleInput.h"
#include "LoadPlayerData.h"
#include "../../Common/DataLoad/LoadDataAnimation.h"
#include "../../Common/DataLoad/LoadDataEffect.h"
#include "../../Application/Application.h"
#include "../../Common/Physics.h"
#include "PlayerWeapon/PlayerWeaponSword.h"
#include "Player.h"

#pragma region

const float NORMAL_COLL_SIZE = 1.0f;
const float SKILL_COLL_SIZE = 1500.0f;
const float SKILL_ADD_POWER_MULT = 3.0f;

const float PARY_ADD_POWER_MULT = 10.0f;

const float SKILL_CHARGE = 0.7f;

const float DOWN_RATE = 100.0f;

#pragma endregion

Player::Player(void):
	playerState_(PlayerState::STATE::IDLE),
	isGravity_(false),
	isInertia_(false),
	isInvincible_(false),
	isSkill_(false),
	defAttackPower_(GAGE_INIT),
	nowSumDamage_(GAGE_INIT)
{
}

Player::~Player(void)
{
}

void Player::ActorBattleStart(void)
{
	if (!isBattleStart_)
	{
		playerState_ = PlayerState::WALK;
		EndGravity();
		EndInertia();
	}
}

bool Player::BattleStepCondition(void)
{
	return playerData_.hp_ <= ActorBase::HP_MIN;
}

bool Player::IsDash(void)
{
	return playerState_ & PlayerState::DASH;
}

bool Player::IsCharge(void)
{
	return playerState_ & PlayerState::CHARGE;
}

bool Player::IsTechnique(void)
{
	return playerState_ & PlayerState::TECHNIQUE;
}

bool Player::IsParysOrJustAvoid(void)
{
	return isInvincible_;
}

bool Player::IsSkill(void)
{
	return playerState_ & PlayerState::SKILL;
}

void Player::InitActorTransform(void)
{
	LoadData::LoadDataInfo info = { "PlayerInfo.json", "Hero" };
	
	actorTransform_ = std::make_unique<Transform>(info, ResourceManager::RESOURCE_ID::PLAYER_HERO);

	SceneManager::GetInstance().GetCamera().lock()->ChangeMode(Camera::MODE::FOLLOW);
	SceneManager::GetInstance().GetCamera().lock()->
		SetTargetTrans(actorTransform_);

	playerData_ = LoadPlayerData::LoadedPlayerData(Application::GetInstance().GetDataPaths(),info);
	
	playerWeapon_ = std::make_unique<PlayerWeaponSword>(actorTransform_->GetModelIdReference());
	playerWeapon_->Init();
	playerWeapon_->SetUpAttackPower(playerData_.attackPower_);

	defAttackPower_ = playerData_.attackPower_;
	


	playerData_.pos_ = &actorTransform_->GetPosReference();
	
	playerData_.collitionGravity_ = std::make_shared<Line3DF>(
		VAdd(*playerData_.pos_, VScale(actorTransform_->GetUp(), GRAVITY_UP_OFFSET)),
		VAdd(*playerData_.pos_, VScale(actorTransform_->GetDown(), GRAVITY_DOWN_OFFSET)));

	playerData_.collisionSphere_ = std::make_shared<SphereF>(
		VAdd(*playerData_.pos_, VScale(Utility::DIR_D, playerData_.collisionSphereOffset_)),
		playerData_.collitionSphereSize_);


	playerData_.collisionAttackSphere_ = std::make_shared<SphereF>(
		*playerData_.pos_, NORMAL_COLL_SIZE);


	myActorCollision_.lockOnPos = playerData_.pos_;
	myActorCollision_.collisionGravity = playerData_.collitionGravity_;
	myActorCollision_.collisionSphere = playerData_.collisionSphere_;
	myActorCollision_.collisionModelId = &actorTransform_->GetModelIdReference();

	myActorCollision_.collisionAttackSphere = playerData_.collisionAttackSphere_;

	myActorCollision_.isCollAttack_ = false;

	myActorCollision_.damagePower = &playerData_.attackPower_;

}

void Player::InitActorAnimation(void)
{
	actorAnime_ = std::make_unique<AnimationController>(actorTransform_->GetModelId());

	LoadData::LoadDataInfo info = { "PlayerInfo.json", "IDLE" };
	DataAnimation data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::IDLE), data);

	info.dataName = "MOVE";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::MOVE), data);

	info.dataName = "WALK";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::WALK), data);

	info.dataName = "DASH_MOVE";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::DASH | PlayerState::MOVE), data);

	info.dataName = "JUMP";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::JUMP), data);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::JUMP | PlayerState::STATE::MOVE), data);

	info.dataName = "AVOID";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::AVOID), data);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::AVOID | PlayerState::JUST_AVOID), data);

	info.dataName = "MIDDLE";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::MIDDLE_AVOID), data);

	info.dataName = "MIDDLE_ATTACK";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK_2 | PlayerState::STATE::MIDDLE_ATTACK), data);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK_3 | PlayerState::STATE::MIDDLE_ATTACK), data);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK_4 | PlayerState::STATE::MIDDLE_ATTACK), data);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK_5 | PlayerState::STATE::MIDDLE_ATTACK), data);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK_6 | PlayerState::STATE::MIDDLE_ATTACK), data);
	actorAnime_->Add(static_cast<int>(PlayerState::JUMP | PlayerState::SKY_ATTACK_1 | PlayerState::STATE::MIDDLE_ATTACK), data);
	actorAnime_->Add(static_cast<int>(PlayerState::JUMP | PlayerState::SKY_ATTACK_2 | PlayerState::STATE::MIDDLE_ATTACK), data);
	actorAnime_->Add(static_cast<int>(PlayerState::JUMP | PlayerState::FALL_ATTACK | PlayerState::STATE::MIDDLE_ATTACK), data);

	info.dataName = "ATTACK_1";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK | PlayerState::STATE::ATTACK_1), data);

	info.dataName = "ATTACK_2";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK | PlayerState::STATE::ATTACK_2), data);

	info.dataName = "ATTACK_3";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK | PlayerState::STATE::ATTACK_3), data);

	info.dataName = "ATTACK_4";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK | PlayerState::STATE::ATTACK_4), data);

	info.dataName = "ATTACK_5";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK | PlayerState::STATE::ATTACK_5), data);

	info.dataName = "ATTACK_6";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK | PlayerState::STATE::ATTACK_6), data);

	info.dataName = "CHARGE_ATTACK";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK | PlayerState::CHARGE | PlayerState::NORMAL_CHARGE), data);

	info.dataName = "CHARGE_HORIZONTAL_ATTACK";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK | PlayerState::CHARGE | PlayerState::HORIZONTAL_CHARGE), data);

	info.dataName = "CHARGE_FORWARD_ATTACK";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::ATTACK | PlayerState::CHARGE | PlayerState::FORWARD_CHARGE), data);

	info.dataName = "SKY_ATTACK_1";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::JUMP | PlayerState::STATE::ATTACK | PlayerState::STATE::SKY_ATTACK_1), data);

	info.dataName = "SKY_ATTACK_2";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::JUMP | PlayerState::STATE::ATTACK | PlayerState::STATE::SKY_ATTACK_2), data);

	info.dataName = "FALL_ATTACK";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::JUMP | PlayerState::STATE::ATTACK | PlayerState::STATE::FALL_ATTACK), data);

	info.dataName = "SKILL";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::SKILL), data);

	info.dataName = "TECHNIQUE";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::TECHNIQUE), data);

	info.dataName = "HIT_LEFT";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::HIT_LEFT), data);

	info.dataName = "HIT_RIGHT";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::STATE::HIT_RIGHT), data);

	info.dataName = "DEATH";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(PlayerState::DEATH), data);

	actorAnime_->Start(static_cast<int>(playerState_));
}

void Player::InitActorUi(void)
{
	const Color3D colorBrack = { 0, 0, 0 };
	const Color3D colorGray = {128, 128, 128};
	const Color3D colorWhiteGray = { 100, 100, 100 };

	LoadData::LoadDataInfo info = { "PlayerInfo.json", "PlayerHp" };

	std::unique_ptr<Image>tempImage;

	actorUi_.push_back(std::make_shared<Image>(info,
		ResourceManager::RESOURCE_ID::PLAYER_HP_CASE,
		Image::IMAGE_TYPE::NORMAL));
	actorUi_.push_back(std::make_shared<Image>(info,
		ResourceManager::RESOURCE_ID::PLAYER_HP,
		Image::IMAGE_TYPE::GAGE));
	actorUi_[1]->SetUpImageGageRate(playerData_.hp_, playerData_.maxHp_, colorBrack);
	actorUi_[1]->SetUpImageEventGageRate(playerData_.hp_);

	info.dataName = "ContinueRepairIcon";
	actorUi_.push_back(std::make_shared<Image>(info,
		ResourceManager::RESOURCE_ID::REPAIR_ICON,
		Image::IMAGE_TYPE::GAGE));

	actorUi_[2]->SetUpImageGageRate(playerTechnique_.techniqueTimer.GetTotalNowTimeReference(),
		playerTechnique_.recastTimeMax, colorGray, 1.5f);
	
	info.dataName = "IconCase";
	actorUi_.push_back(std::make_shared<Image>(info,
		ResourceManager::RESOURCE_ID::ICON_CASE,
		Image::IMAGE_TYPE::NORMAL));


	info.dataName = "SkillGageCase";
	actorUi_.push_back(std::make_shared<Image>(info,
		ResourceManager::RESOURCE_ID::SKILL_GAGE_CASE,
		Image::IMAGE_TYPE::NORMAL));

	info.dataName = "SkillGage";
	tempImage = std::make_unique<Image>(info, ResourceManager::RESOURCE_ID::SKILL_GAGE_MAX);
	actorUi_.push_back(std::make_shared<Image>(info,
		ResourceManager::RESOURCE_ID::SKILL_GAGE,
		Image::IMAGE_TYPE::CIRCLE_GAGE, tempImage->GetId()));
	actorUi_[5]->SetUpImageGageRate(playerData_.skillGage_, playerData_.skillGageMax_, colorWhiteGray);


	info.dataName = "Numbers_Digit_1";
	imgTimesBig_ = std::make_unique<ImageDiv>(info, ResourceManager::RESOURCE_ID::NUMBERS_RED);
	info.dataName = "Numbers_Point_1";
	imgTimesSmall_ = std::make_unique<ImageDiv>(info, ResourceManager::RESOURCE_ID::NUMBERS_RED);
	info.dataName = "Numbers_Digit_10";
	std::unique_ptr<ImageDiv>temp = std::make_unique<ImageDiv>(info, ResourceManager::RESOURCE_ID::NUMBERS_RED);
	timesBig_Digit_10_Pos_ = temp->GetPos();

	info.dataName = "Numbers_Point_10";
	temp = std::make_unique<ImageDiv>(info, ResourceManager::RESOURCE_ID::NUMBERS_RED);
	timesSmall_Point_10_Pos_ = temp->GetPos();
}

void Player::InitActorEffectLaunch(void)
{
	actorEffectLaunch_ = std::make_unique<EffectPlayLaunch>();

	EffectResourceManager::EFFECT_RESOURCE_ID id = EffectResourceManager::EFFECT_RESOURCE_ID::SMOKE;

	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_RIGHT;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_LEFT;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_HORIZ;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_DUO;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_UP;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_TORIO;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_DEVIATION;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_ACCEL;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_FLASH;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::LIGHTNING;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::PARYS;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::SPARK;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::SONIC_BOOM;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::THUNDER_BOLT;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::THUNDER_BOLT_ONE;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::LIGHTNING;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::CHARGE;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::CHARGE_FINISH;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::REPAIR;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::PLAYER_DAMAGE_SPARK;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);
}

void Player::InitActorEffectInfo(void)
{
	LoadData::LoadDataInfo info = { "Player_EffectInfo.json", "MoveSmoke" };

	DataEffect data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::MOVE), data);

	info.dataName = "Smoke";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::ACTION_SMOKE), data);

	info.dataName = "Slash";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::ATTACK_SLASH), data);

	info.dataName = "Slash_Flash";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::ATTACK_SLASH_FLASH), data);

	info.dataName = "SlashAccel";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::ATTACK_SLASH_ACCEL), data);

	info.dataName = "Parys";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::PARYS), data);

	info.dataName = "Spark";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::SPARK), data);

	info.dataName = "SonicBoom";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::DASH_SONIC), data);

	info.dataName = "ThunderBolt";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::ATTACK_THUNDER), data);

	info.dataName = "FinishThunder";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::FINISH_SKILL_THUNDER), data);

	info.dataName = "Lightning";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::ATTACK_LIGHTNING), data);

	info.dataName = "Charge";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::CHARGE), data);

	info.dataName = "Repair";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::REPAIR), data);

	info.dataName = "DamageSpark";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::DAMAGE_SPARK), data);

}

void Player::InitActorCameraControl(void)
{
	LoadData::LoadDataInfo info = { "PlayerCameraInfo.json", "DashDirection_1" };

	// ダッシュカメラ
	AddStateCameraControlData(static_cast<int>(PlayerState::DASH), info);

	// 前チャージ攻撃カメラ
	info.dataName = "ChargeForwardDirection_1";
	AddStateCameraControlData(static_cast<int>(PlayerState::FORWARD_CHARGE), info);

	// 横チャージ攻撃カメラ
	info.dataName = "ChargeHorizontalDirection_1";
	AddStateCameraControlData(static_cast<int>(PlayerState::HORIZONTAL_CHARGE), info);
	info.dataName = "ChargeHorizontalDirection_2";
	AddStateCameraControlData(static_cast<int>(PlayerState::HORIZONTAL_CHARGE), info);

	// 回避カメラ
	info.dataName = "AvoidDirection_1";
	AddStateCameraControlData(static_cast<int>(PlayerState::MIDDLE_AVOID), info);

	// 必殺技カメラ
	info.dataName = "SkillDirection_1";
	AddStateCameraControlData(static_cast<int>(PlayerState::SKILL), info);
	info.dataName = "SkillDirection_2";
	AddStateCameraControlData(static_cast<int>(PlayerState::SKILL), info);
	info.dataName = "SkillDirection_3";
	AddStateCameraControlData(static_cast<int>(PlayerState::SKILL), info);
	info.dataName = "SkillDirection_4";
	AddStateCameraControlData(static_cast<int>(PlayerState::SKILL), info);

}

void Player::InitActorSoundLaunch(void)
{
	waveLaunch_ = std::make_unique<SoundPlayLaunch>();

	SoundResourceManager::SOUND_RESOURCE_ID id =
		SoundResourceManager::SOUND_RESOURCE_ID::WALK;

	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::RUN;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::SWING;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::SWORD_ATTACK;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::DAMAGE;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::PARY;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::JUST_AVOID;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::DASH_SONIC;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::AVOID;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::JUMP;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::ELECTRO;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::THUNDER;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::SPARK;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::SLASH_FINISH;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::REPAIR;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::CHARGE;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

}

void Player::InitSpecial(void)
{
	InitActorEffectLaunch();

	playerState_ = PlayerState::IDLE;

	playerControle_ = std::make_unique<PlayerControle>(playerData_, playerState_);
	playerControle_->Init();

	LoadData::LoadDataInfo info = { "DebugImages.json", "PlayerStrongGage" };

	playerTechnique_.techniqueTimer.SetTotalNowTime(playerTechnique_.recastTimeMax);

	addOtherActorCollision_.push_back(&playerWeapon_->GetMyCollision());

	playerWeapon_->InitSpecial();

	actorTransform_->InitVertexRenderer(Transform::TRANSFORM_TYPE::SKIN_MODEL);

}

void Player::CollisionedGravityActor(Position3DF pos)
{
	ActorMoveSet(playerData_, pos);
}

void Player::CollisionedHorizontalActor(void)
{
	VECTOR vec = VSub(actorTransform_->GetPrevPos(), *playerData_.pos_);
	vec.y = 0.0f;
	ActorMoveDistance(playerData_, VNorm(vec), VSize(vec));
}

void Player::CollisionedDamageActor(float* damagePower)
{
	if (damagePower == nullptr)return;

	float nowStep = actorAnime_->GetStepAnimTime();
	float endStep = actorAnime_->GetEndStepAnimTime();
	const float LIMIT_PERCENT = 0.4f;

	if ((playerState_ & PlayerState::AVOID | playerState_ & PlayerState::ATTACK) && 
		!(playerState_ & PlayerState::CHARGE) &&
		(nowStep / endStep) <= LIMIT_PERCENT)
	{
		if (playerState_ & PlayerState::AVOID)
		{
			playerState_ = static_cast<PlayerState::STATE>(playerState_ | PlayerState::JUST_AVOID);
			actorTransform_->StartEventVertex(Transform::EVENT_TYPE::RIMLIGHT_SKIN);

			// 効果音
			SoundResourceManager::SOUND_RESOURCE_ID soundId =
				SoundResourceManager::SOUND_RESOURCE_ID::JUST_AVOID;
			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
		}
		else if (playerState_ & PlayerState::ATTACK)
		{
			EffectResourceManager::EFFECT_RESOURCE_ID effectId =
				EffectResourceManager::EFFECT_RESOURCE_ID::PARYS;
			actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::PARYS)));
			actorEffectLaunch_->SetPos(effectId, *playerData_.pos_);

			// 効果音
			SoundResourceManager::SOUND_RESOURCE_ID soundId =
				SoundResourceManager::SOUND_RESOURCE_ID::PARY;
			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);

			playerData_.attackPower_ = defAttackPower_ * PARY_ADD_POWER_MULT;
		}

		ChagePlayerSkill(3.0f);

		isInvincible_ = true;

		SetSlowTime(0.3f);
	}
	else if(!isInvincible_)
	{
		nowSumDamage_ += *damagePower;

		playerData_.hp_ -= *damagePower;
		playerData_.flinchParam_ += *damagePower * DOWN_RATE;

		EffectResourceManager::EFFECT_RESOURCE_ID effectId =
			EffectResourceManager::EFFECT_RESOURCE_ID::PLAYER_DAMAGE_SPARK;
		actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::DAMAGE_SPARK)));

		SoundResourceManager::SOUND_RESOURCE_ID soundId =
			SoundResourceManager::SOUND_RESOURCE_ID::DAMAGE;
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
	}
}

void Player::UpdateSpecial(void)
{
	nowGameTime_.UpdateTime();
	SceneManager::GetInstance().SetUpGameToResultScene({ nowGameTime_.GetTotalNowTime(), nowSumDamage_ });

	if(!isSlowActor_)playerControle_->Update();

	Walk();
	Death();
	Move();
	Avoid();
	Jump();
	Attack();
	Technique();
	Skill();
	Flinch();

	if (isGravity_ || !isTouchGround_ && !isSlowActor_)Gravity(playerData_);
	if (isInertia_ && !isSlowActor_)				   Inertia(playerData_);

	ControleEnd();

	UpdateSlowTime();

	if (actorAttackRagTime_->GetTotalNowTime() > playerAttackRag_)
	{
		actorAttackRagTime_->InitTotalNowTime();
		playerWeapon_->OnCollAttack();
		myActorCollision_.isCollAttack_ = true;
	}

	playerTechnique_.techniqueTimer.UpdateTime();

	actorAnime_->Play(playerState_);

	playerWeapon_->UpdateLocus(actorAnime_->GetStepAnimTime(), actorAnime_->GetAttachAnimIndex());
	playerWeapon_->SyncroPlayerWeapon(actorTransform_->GetModelId(), playerData_.weaponBone);
}

void Player::SetSlowTime(const float time)
{
	slowTimeMax_ = time;
	slowTime_.InitTotalNowTime();
}

void Player::UpdateSlowTime(void)
{
	slowTime_.UpdateTime();

	if (slowTime_.GetTotalNowTime() < slowTimeMax_)
		isSlowActor_ = true;
	else
	{
		actorTransform_->EndEventVertex();
		isSlowActor_ = false;
	}

	// 自分以外にも共有
	for (auto otherSlow : otherSlowActor_)
		otherSlow(isSlowActor_);

	if (isMySlow_)
		isSlowActor_ = true;

}

void Player::Walk(void)
{
	if (!(playerState_ & PlayerState::WALK))return;

	const float PLAYER_SPEED_RATE = 0.8f;
	ActorMoveVector(playerData_, VNorm(GetActorOtherLockonDir(playerData_)), 
		static_cast<int>(static_cast<float>(-playerData_.speed_) * PLAYER_SPEED_RATE));
}

void Player::Move(void)
{
	static bool isSonicEffect = false;

	if (!(playerState_ & PlayerState::MOVE))
	{
		isSonicEffect = false;
		return;
	}

	const Quaternion& cameraRot = SceneManager::GetInstance().GetCamera().lock()->GetRotXZ();

	if (playerState_ & PlayerState::MOVE_UP)   actorMoveDir_ = VAdd(actorMoveDir_, cameraRot.GetForward());
	if (playerState_ & PlayerState::MOVE_LEFT) actorMoveDir_ = VAdd(actorMoveDir_, cameraRot.GetLeft());
	if (playerState_ & PlayerState::MOVE_DOWN) actorMoveDir_ = VAdd(actorMoveDir_, cameraRot.GetBack());
	if (playerState_ & PlayerState::MOVE_RIGHT)actorMoveDir_ = VAdd(actorMoveDir_, cameraRot.GetRight());

	actorTransform_->RotationModelQuaRot(actorMoveDir_);
	
	SoundResourceManager::SOUND_RESOURCE_ID soundId;

	if (playerState_ & PlayerState::DASH)
	{
		if (!isSonicEffect)
		{
			isSonicEffect = true;
			EffectResourceManager::EFFECT_RESOURCE_ID effectId =
				EffectResourceManager::EFFECT_RESOURCE_ID::SONIC_BOOM;

			actorEffectLaunch_->FollowPlay(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::DASH_SONIC)),
				actorTransform_->GetPosReference(), &actorTransform_->GetRotReference());

			// ソニックブームの効果音
			soundId = SoundResourceManager::SOUND_RESOURCE_ID::DASH_SONIC;
			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);

			SceneManager::GetInstance().GetCamera().lock()->
				SetUpCameraControlData(cameraControle_.at(static_cast<int>(PlayerState::DASH)));

			ChagePlayerSkill();
		}

		ActorMoveVector(playerData_, actorMoveDir_, playerData_.dashSpeed_ - playerData_.speed_);

		if (!(playerState_ & PlayerState::JUMP))
		{
			// 効果音の再生
			soundId = SoundResourceManager::SOUND_RESOURCE_ID::RUN;
			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		ActorMoveVector(playerData_, actorMoveDir_);

		if (!(playerState_ & PlayerState::JUMP))
		{
			// 効果音の再生
			soundId = SoundResourceManager::SOUND_RESOURCE_ID::WALK;
			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
		}
	}
}

void Player::Attack(void)
{
	if (isSlowActor_ && !isMySlow_)return;

	if (!(playerState_ & PlayerState::ATTACK))
	{
		StopAttackEffect();
		return;
	}

	playerWeapon_->OnDrawLocus();

	bool isLerpCamera = false;

	EffectResourceManager::EFFECT_RESOURCE_ID effectId =
		EffectResourceManager::EFFECT_RESOURCE_ID::LIGHTNING;

	if (actorAttackRagTime_->GetTotalNowTime() > playerAttackRag_)
	{
		actorEffectLaunch_->FollowPlay(effectId,
			GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_LIGHTNING)),
			*playerData_.pos_, &actorTransform_->GetRotReference());

		Attack_1(isLerpCamera);
		Attack_2(isLerpCamera);
		Attack_3(isLerpCamera);
		Attack_4(isLerpCamera);
		Attack_5(isLerpCamera);
		Attack_6(isLerpCamera);
		Attack_NormalCharge(isLerpCamera);
		Attack_ForwardCharge(isLerpCamera);
		Attack_HorizontalCharge(isLerpCamera);
		Attack_Sky_1(isLerpCamera);
		Attack_Sky_2(isLerpCamera);
		Attack_Fall(isLerpCamera);
	}

	NormalChargeRun(isLerpCamera);
	ForwardChargeHorizontalChargeRun(isLerpCamera);

	LockOnEnemy(isLerpCamera);
}

void Player::Attack_1(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::ATTACK_1))return;

	EffectResourceManager::EFFECT_RESOURCE_ID effectId =
		EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_RIGHT;

	actorEffectLaunch_->FollowPlay(effectId,
		GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SLASH)),
		*playerData_.pos_, &actorTransform_->GetRotReference());

	isLerpCamera = true;

	ChagePlayerSkill();

	// 効果音の再生
	SoundResourceManager::SOUND_RESOURCE_ID soundId = 
		SoundResourceManager::SOUND_RESOURCE_ID::SWING;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
	soundId = SoundResourceManager::SOUND_RESOURCE_ID::ELECTRO;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
}

void Player::Attack_2(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::ATTACK_2))return;
	
	EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
		EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_LEFT;
	actorEffectLaunch_->FollowPlay(effectId,
		GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SLASH)),
		*playerData_.pos_, &actorTransform_->GetRotReference());

	isLerpCamera = true;

	ChagePlayerSkill();

	// 効果音の再生
	SoundResourceManager::SOUND_RESOURCE_ID soundId = 
		SoundResourceManager::SOUND_RESOURCE_ID::SWING;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
	soundId = SoundResourceManager::SOUND_RESOURCE_ID::ELECTRO;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
}

void Player::Attack_3(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::ATTACK_3))return;

	EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
		EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_HORIZ;
	actorEffectLaunch_->FollowPlay(effectId,
		GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SLASH)),
		*playerData_.pos_, &actorTransform_->GetRotReference());

	isLerpCamera = true;

	ChagePlayerSkill();

	// 効果音の再生
	SoundResourceManager::SOUND_RESOURCE_ID soundId = 
		SoundResourceManager::SOUND_RESOURCE_ID::SWING;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
	soundId = SoundResourceManager::SOUND_RESOURCE_ID::ELECTRO;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
}

void Player::Attack_4(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::ATTACK_4))return;

	EffectResourceManager::EFFECT_RESOURCE_ID effectId =
		EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_DUO;
	actorEffectLaunch_->FollowPlay(effectId,
		GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SLASH)),
		*playerData_.pos_, &actorTransform_->GetRotReference());

	isLerpCamera = true;

	ChagePlayerSkill();

	// 効果音の再生
	SoundResourceManager::SOUND_RESOURCE_ID soundId = 
		SoundResourceManager::SOUND_RESOURCE_ID::SWING;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
	soundId = SoundResourceManager::SOUND_RESOURCE_ID::ELECTRO;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
}

void Player::Attack_5(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::ATTACK_5))return;
	
	EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
		EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_UP;
	actorEffectLaunch_->FollowPlay(effectId,
		GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SLASH)),
		*playerData_.pos_, &actorTransform_->GetRotReference());

	isLerpCamera = true;

	ChagePlayerSkill();

	// 効果音の再生
	SoundResourceManager::SOUND_RESOURCE_ID soundId = 
		SoundResourceManager::SOUND_RESOURCE_ID::SWING;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
	soundId = SoundResourceManager::SOUND_RESOURCE_ID::ELECTRO;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
}

void Player::Attack_6(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::ATTACK_6))return;
	
	EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
		EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_TORIO;
	actorEffectLaunch_->FollowPlay(effectId,
		GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SLASH)),
		*playerData_.pos_, &actorTransform_->GetRotReference());

	isLerpCamera = true;

	ChagePlayerSkill();

	// 効果音の再生
	SoundResourceManager::SOUND_RESOURCE_ID soundId = 
		SoundResourceManager::SOUND_RESOURCE_ID::SWING;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
	soundId = SoundResourceManager::SOUND_RESOURCE_ID::ELECTRO;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
}

void Player::Attack_NormalCharge(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::CHARGE))return;
	
	EffectResourceManager::EFFECT_RESOURCE_ID effectId;

	if (playerState_ & PlayerState::NORMAL_CHARGE)
	{
		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::SMOKE;
		actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::ACTION_SMOKE)));
		actorEffectLaunch_->SetPos(effectId, actorTransform_->GetPos());

		// アクセルエフェクト
		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_ACCEL;
		actorEffectLaunch_->FollowPlay(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SLASH_ACCEL)),
			actorTransform_->GetPosReference(), &actorTransform_->GetRotReference());

		const float INERTIA_POWER = 93.0f;

		if (myActorCollision_.isCollHorizontal)
			SetUpInertia(VNorm(GetActorOtherLockonDir(playerData_)), INERTIA_POWER);

		ChagePlayerSkill();
	}
}

void Player::NormalChargeRun(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::NORMAL_CHARGE))return;

	playerAttackRag_ = 0.1f;// 攻撃のラグを短くする

	const float PLAYER_COLL_NORMAL_CHARGE_END = 25.0f;
	if (actorAnime_->GetStepAnimTime() > PLAYER_COLL_NORMAL_CHARGE_END)
	{
		if (myActorCollision_.isCollHorizontal)
			isLerpCamera = true;

		myActorCollision_.isCollHorizontal = false;
	}
}

void Player::ForwardChargeHorizontalChargeRun(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::FORWARD_CHARGE ||
		playerState_ & PlayerState::HORIZONTAL_CHARGE))return;

	playerAttackRag_ = 1000.0f;// 適当な大きな数で当たり判定が発生しないようにする

	const float CHARGE_BIUS_INIT = 1.0f;
	static float chargeBius = CHARGE_BIUS_INIT;

	auto& input = SimpleInput::GetInstance();

	if (actorAnime_->GetStepAnimTime() < playerData_.chargeStep &&
		input.GetInput("charge"))
	{
		isMySlow_ = true;

		// チャージ中のエフェクト
		EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
			EffectResourceManager::EFFECT_RESOURCE_ID::CHARGE_FINISH;

		if (!actorEffectLaunch_->IsPlayEffect(effectId))
		{
			actorEffectLaunch_->Play(effectId,
				GetEffectDataInfo(static_cast<int>(EFFECT_USES::CHARGE)));
			Position3DF pos = actorTransform_->GetPos();
			pos = VAdd(pos, VScale(actorTransform_->GetUp(), 200.0f));
			actorEffectLaunch_->SetPos(effectId, pos);

			effectId = EffectResourceManager::EFFECT_RESOURCE_ID::CHARGE;
			actorEffectLaunch_->FollowPlay(effectId,
				GetEffectDataInfo(static_cast<int>(EFFECT_USES::CHARGE)),
				actorTransform_->GetPosReference());
			actorEffectLaunch_->SetScl(effectId,
				VScale(actorEffectLaunch_->GetScl(effectId), chargeBius));
			chargeBius += 0.1f;// バイアスを少しずつ上げる

			// 効果音
			SoundResourceManager::SOUND_RESOURCE_ID soundId = 
				SoundResourceManager::SOUND_RESOURCE_ID::CHARGE;
			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
		}
	}
	else
	{
		if (isMySlow_)
		{
			if (playerState_ & PlayerState::FORWARD_CHARGE)
				SceneManager::GetInstance().GetCamera().lock()->
				SetUpCameraControlData(cameraControle_.at(static_cast<int>(PlayerState::FORWARD_CHARGE)));
			else if (playerState_ & PlayerState::HORIZONTAL_CHARGE)
				SceneManager::GetInstance().GetCamera().lock()->
				SetUpCameraControlData(cameraControle_.at(static_cast<int>(PlayerState::HORIZONTAL_CHARGE)));

			ChagePlayerSkill();
		}

		playerAttackRag_ = 0.05f;// ラグをものすごく短くする
		playerData_.attackPower_ *= chargeBius;

		isMySlow_ = false;
		chargeBius = CHARGE_BIUS_INIT;
	}
}

void Player::Attack_ForwardCharge(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::FORWARD_CHARGE))return;
	
	EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
		EffectResourceManager::EFFECT_RESOURCE_ID::CHARGE;
	actorEffectLaunch_->FollowPlay(effectId,
		GetEffectDataInfo(static_cast<int>(EFFECT_USES::CHARGE)),
		actorTransform_->GetPosReference());

	SoundResourceManager::SOUND_RESOURCE_ID soundId =
		SoundResourceManager::SOUND_RESOURCE_ID::SWING;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
}

void Player::Attack_HorizontalCharge(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::HORIZONTAL_CHARGE))return;
	
	EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
		EffectResourceManager::EFFECT_RESOURCE_ID::CHARGE;
	actorEffectLaunch_->FollowPlay(effectId,
		GetEffectDataInfo(static_cast<int>(EFFECT_USES::CHARGE)),
		actorTransform_->GetPosReference());

	if (playerState_ & PlayerState::FORWARD_CHARGE)
	{
		SoundResourceManager::SOUND_RESOURCE_ID soundId =
			SoundResourceManager::SOUND_RESOURCE_ID::SWING;
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
	}
}

void Player::Attack_Sky_1(bool& isLerpCamera)
{
	const float GRAVITY_POWER = 50.0f;
	if (!(playerState_ & PlayerState::SKY_ATTACK_1))return;
	
	gravity_ = VScale(Physics::GRAVITY_DIR, -GRAVITY_POWER);

	// エフェクト発生
	EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
		EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_RIGHT;
	actorEffectLaunch_->FollowPlay(effectId, GetEffectDataInfo(
		static_cast<int>(EFFECT_USES::ATTACK_SLASH)),
		actorTransform_->GetPosReference(), &actorTransform_->GetRotReference());

	ChagePlayerSkill();

	// 効果音の再生
	SoundResourceManager::SOUND_RESOURCE_ID soundId = 
		SoundResourceManager::SOUND_RESOURCE_ID::SWING;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
	soundId = SoundResourceManager::SOUND_RESOURCE_ID::ELECTRO;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
}

void Player::Attack_Sky_2(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::SKY_ATTACK_2))return;

	const float GRAVITY_POWER = 50.0f;
	gravity_ = VScale(Physics::GRAVITY_DIR, -GRAVITY_POWER);

	// エフェクト発生
	EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
		EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_LEFT;
	actorEffectLaunch_->FollowPlay(effectId, GetEffectDataInfo(
		static_cast<int>(EFFECT_USES::ATTACK_SLASH)),
		actorTransform_->GetPosReference(), &actorTransform_->GetRotReference());

	ChagePlayerSkill();

	// 効果音の再生
	SoundResourceManager::SOUND_RESOURCE_ID soundId = 
		SoundResourceManager::SOUND_RESOURCE_ID::SWING;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
	soundId = SoundResourceManager::SOUND_RESOURCE_ID::ELECTRO;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
}

void Player::Attack_Fall(bool& isLerpCamera)
{
	if (!(playerState_ & PlayerState::FALL_ATTACK))return;

	const float GRAVITY_POWER = 50.0f;
	gravity_ = VScale(Physics::GRAVITY_DIR, GRAVITY_POWER);

	// エフェクト発生
	EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
		EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_UP;
	actorEffectLaunch_->FollowPlay(effectId, GetEffectDataInfo(
		static_cast<int>(EFFECT_USES::ATTACK_SLASH)),
		actorTransform_->GetPosReference(), &actorTransform_->GetRotReference());

	ChagePlayerSkill();

	// 効果音の再生
	SoundResourceManager::SOUND_RESOURCE_ID soundId =
		SoundResourceManager::SOUND_RESOURCE_ID::SWING;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
	soundId = SoundResourceManager::SOUND_RESOURCE_ID::ELECTRO;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
}

void Player::Jump(void)
{
	if (!(playerState_ & PlayerState::JUMP))return;
	if (playerState_ & PlayerState::ATTACK)return;
	if (playerState_ & PlayerState::MIDDLE_ATTACK)return;

	static Dir3DF prevMoveDir = actorMoveDir_;
	if (playerState_ & PlayerState::JUMP && !isGravity_)
	{
		isGravity_ = true;
		gravity_ = VScale(Physics::GRAVITY_DIR, static_cast<float>(-playerData_.jumpPower_));
		prevMoveDir = actorMoveDir_;

		ChagePlayerSkill();

		// 効果音の再生
		SoundResourceManager::SOUND_RESOURCE_ID soundId =
			SoundResourceManager::SOUND_RESOURCE_ID::JUMP;
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
	}
	else if(!isTouchGround_)
	{
		ActorMoveDistance(playerData_, prevMoveDir, static_cast<float>(playerData_.jumpPower_));
	}
}

void Player::Avoid(void)
{
	if (!(playerState_ & PlayerState::AVOID))return;

	playerWeapon_->OnDrawLocus();

	if (playerState_ & PlayerState::AVOID && !isInertia_)
	{
		isGravity_ = true;// 一時的に重力も適用
		isInertia_ = true;

		Dir3DF dir = actorTransform_->GetForward();
		if (VSize(actorMoveDir_) > 0.0f)dir = actorMoveDir_;

		inertia_ = VScale(dir, static_cast<float>(playerData_.avoidSpeed_));

		ChagePlayerSkill();

		// 効果音の再生
		SoundResourceManager::SOUND_RESOURCE_ID soundId =
			SoundResourceManager::SOUND_RESOURCE_ID::AVOID;
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
	}
}

void Player::Technique(void)
{
	if (playerTechnique_.techniqueTimer.GetTotalNowTime() >= playerTechnique_.recastTimeMax)
		playerTechnique_.isCanTechnique = true;

	if (!(playerState_ & PlayerState::TECHNIQUE))return;
	
	// エフェクトの発生
	EffectResourceManager::EFFECT_RESOURCE_ID effectId =
		EffectResourceManager::EFFECT_RESOURCE_ID::REPAIR;

	if (!actorEffectLaunch_->IsPlayEffect(effectId) &&
		playerTechnique_.techniqueTimer.GetTotalNowTime() >= playerTechnique_.recastTimeMax)
	{
		actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::REPAIR)));
		actorEffectLaunch_->SetPos(effectId, actorTransform_->GetPos());

		// 効果音発生
		SoundResourceManager::SOUND_RESOURCE_ID soundId =
			SoundResourceManager::SOUND_RESOURCE_ID::REPAIR;
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);

		playerTechnique_.techniqueTimer.InitTotalNowTime();

		ChagePlayerSkill();
	}
	else if (playerTechnique_.isCanTechnique)
		RepairPlayerHp();
	else
		playerState_ = static_cast<PlayerState::STATE>(playerState_ & ~PlayerState::TECHNIQUE);
}

void Player::Skill(void)
{
	if (playerData_.skillGage_ >= playerData_.skillGageMax_)
		isSkill_ = true;

	if (!(playerState_ & PlayerState::SKILL))return;


	SoundResourceManager::SOUND_RESOURCE_ID soundId = 
		SoundResourceManager::SOUND_RESOURCE_ID::ELECTRO;

	// エフェクトの発生
	EffectResourceManager::EFFECT_RESOURCE_ID effectId =
		EffectResourceManager::EFFECT_RESOURCE_ID::LIGHTNING;

	if (!actorEffectLaunch_->IsPlayEffect(effectId) &&
		playerData_.skillGage_ >= playerData_.skillGageMax_)
	{
		actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_LIGHTNING)));
		actorEffectLaunch_->SetPos(effectId, actorTransform_->GetPos());
		playerData_.skillGage_ = 0.0f;

		SceneManager::GetInstance().GetCamera().lock()->
			SetUpCameraControlData(cameraControle_.at(static_cast<int>(PlayerState::SKILL)));
		SceneManager::GetInstance().GetCamera().lock()->
			ProhibicionCameraControl();
	}
	else if (isSkill_)
	{
		isInvincible_ = true;
		playerData_.collisionAttackSphere_->r = SKILL_COLL_SIZE;
		playerData_.attackPower_ = defAttackPower_ * SKILL_ADD_POWER_MULT;

		playerAttackRag_ = 0.7f;

		if(actorAttackRagTime_->GetTotalNowTime() > playerAttackRag_)
			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);


		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::THUNDER_BOLT;
		if (actorAnime_->GetStepAnimTime() < playerData_.attackCollSkill_ &&
			!actorEffectLaunch_->IsPlayEffect(effectId))
		{
			actorEffectLaunch_->FollowPlay(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_THUNDER)),
				actorTransform_->GetPosReference());
			actorEffectLaunch_->SetRot(effectId, actorTransform_->GetRot());
		}
		if (actorAnime_->GetStepAnimTime() > playerData_.attackCollSkill_)
		{
			actorAnime_->MultPlaySpeed(3.0f);
		}

		const float SKILL_ANIME_STEP_NEXT = 20.0f;
		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::THUNDER_BOLT_ONE;
		if (actorAnime_->GetStepAnimTime() > playerData_.attackCollSkill_ * SKILL_ANIME_STEP_NEXT &&
			!actorEffectLaunch_->IsPlayEffect(effectId))
		{
			actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::FINISH_SKILL_THUNDER)));

			Position3DF effectPos;
			GetActorOtherLockonDir(playerData_, false, Position3DF(), &effectPos);

			actorEffectLaunch_->SetPos(effectId, effectPos);

			// 効果音発生
			soundId = SoundResourceManager::SOUND_RESOURCE_ID::THUNDER;
			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
		}
	}
	else
		playerState_ = static_cast<PlayerState::STATE>(playerState_ & ~PlayerState::SKILL);
}

void Player::Flinch(void)
{
	if (playerState_ & PlayerState::DEATH)return;

	EffectResourceManager::EFFECT_RESOURCE_ID effectId =
		EffectResourceManager::EFFECT_RESOURCE_ID::PLAYER_DAMAGE_SPARK;
	
	const float DAMAGE_EFFECT_OFFSET = 200.0f;
	// ダメージエフェクトを追従
	actorEffectLaunch_->SetPos(effectId,
			VAdd(actorTransform_->GetPos(), VScale(actorTransform_->GetUp(), DAMAGE_EFFECT_OFFSET)));

	if (playerData_.flinchParam_ < playerData_.flinchParamMax_)return;

	isMySlow_ = false;

	playerData_.flinchParam_ = GAGE_INIT;

	// 少し吹っ飛ぶ
	const float INERTIA_POWER = 35.0f;
	SetUpInertia(VNorm(GetActorOtherLockonDir(playerData_)), -INERTIA_POWER);

	// ランダム(0～1)
	const int RANDOM_MAX = 1;
	int rand = GetRand(RANDOM_MAX);

	switch (rand)
	{
	case 0:
		playerState_ = PlayerState::HIT_LEFT;
		break;
	case RANDOM_MAX:
		playerState_ = PlayerState::HIT_RIGHT;
		break;
	default:
		break;
	}
}

void Player::Death(void)
{
	if (playerState_ & PlayerState::DEATH)return;

	if (playerData_.hp_ <= ActorBase::HP_MIN)
		playerState_ = PlayerState::DEATH;
}

void Player::StopAttackEffect(void)
{
	EffectResourceManager::EFFECT_RESOURCE_ID effectId =
		EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_ACCEL;
	actorEffectLaunch_->Stop(effectId);

	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::CHARGE;
	actorEffectLaunch_->Stop(effectId);

	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::CHARGE_FINISH;
	actorEffectLaunch_->Stop(effectId);

}

void Player::StopWaveSound(void)
{
	SoundResourceManager::SOUND_RESOURCE_ID id =
		SoundResourceManager::SOUND_RESOURCE_ID::RUN;
	waveLaunch_->Stop(id);

	id = SoundResourceManager::SOUND_RESOURCE_ID::SWING;
	waveLaunch_->Stop(id);

	id = SoundResourceManager::SOUND_RESOURCE_ID::DASH_SONIC;
	waveLaunch_->Stop(id);

	id = SoundResourceManager::SOUND_RESOURCE_ID::ELECTRO;
	waveLaunch_->Stop(id);

	id = SoundResourceManager::SOUND_RESOURCE_ID::WALK;
	waveLaunch_->Stop(id);
}

void Player::LockOnEnemy(bool isLerp)
{
	ActorOtherLockOnRotation(playerData_, true, myActorCollision_.isCollHorizontal, isLerp);
}

void Player::ControleEnd(void)
{
	EndTechnique();
	EndSkill();
	EndAvoid();
	EndMove();
	EndAttack();
	EndJump();

	EndFlinch();

	EndWalk();

	if (playerState_ & PlayerState::IDLE)
	{
		isInvincible_ = false;// 無敵状態を解除
		playerData_.collisionAttackSphere_->r = NORMAL_COLL_SIZE;
		playerWeapon_->OffDrawLocus();

		playerAttackRag_ = ATTACK_RAG_MAX;

		StopWaveSound();
	}

	actorMoveDir_ = Dir3DF();
}

void Player::EndTechnique(void)
{
	if (!(playerState_ & PlayerState::TECHNIQUE))return;
	
	playerState_ = PlayerState::TECHNIQUE;
	if (actorAnime_->IsPlayAnim())return;
	
	playerTechnique_.isCanTechnique = false;
	playerState_ = PlayerState::IDLE;
}

void Player::EndSkill(void)
{
	if (!(playerState_ & PlayerState::SKILL))return;
	
	playerState_ = PlayerState::SKILL;
	if (actorAnime_->IsPlayAnim())return;
	
	isSkill_ = false;
	playerData_.attackPower_ = defAttackPower_;
	playerState_ = PlayerState::IDLE;
}

void Player::EndAvoid(void)
{
	if (!(playerState_ & PlayerState::AVOID ||
		playerState_ & PlayerState::MIDDLE_AVOID &&
		!(playerState_ & PlayerState::DASH)))return;
	
	if (playerState_ & PlayerState::MIDDLE_AVOID)
		playerState_ = PlayerState::MIDDLE_AVOID;
	else
		playerState_ = static_cast<PlayerState::STATE>(playerState_ &
			(PlayerState::JUST_AVOID | PlayerState::AVOID));

	if (VSize(inertia_) <= 0.0f && !actorAnime_->IsPlayAnim())
	{
		EndInertia();
		EndGravity();
		playerState_ = PlayerState::IDLE;
	}
	else if (!actorAnime_->IsPlayAnim())
	{
		playerState_ = PlayerState::MIDDLE_AVOID;

		SceneManager::GetInstance().GetCamera().lock()->
			SetUpCameraControlData(cameraControle_.at(static_cast<int>(PlayerState::MIDDLE_AVOID)));

		EffectResourceManager::EFFECT_RESOURCE_ID effectId =
			EffectResourceManager::EFFECT_RESOURCE_ID::SPARK;
		actorEffectLaunch_->FollowPlay(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::SPARK)),
			actorTransform_->GetPosReference());

		// 効果音発生
		SoundResourceManager::SOUND_RESOURCE_ID soundId =
			SoundResourceManager::SOUND_RESOURCE_ID::SPARK;
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);

	}
}

void Player::EndMove(void)
{
	if (!(playerState_ & PlayerState::MOVE ||
		playerState_ & PlayerState::DASH))return;
	
	playerState_ = static_cast<PlayerState::STATE>(playerState_ &
		(PlayerState::MOVE | PlayerState::JUMP | PlayerState::DASH));

	EndInertia();

	if (playerState_ & PlayerState::JUMP)return;

	EndGravity();

	auto& input = SimpleInput::GetInstance();
	if (!input.GetInput("moveForward") &&
		!input.GetInput("moveLeft") &&
		!input.GetInput("moveBack") &&
		!input.GetInput("moveRight"))
		playerState_ = PlayerState::IDLE;
}

void Player::EndAttack(void)
{
	if (playerState_ & PlayerState::MIDDLE_ATTACK)
	{
		if (!actorAnime_->IsPlayAnim())
			playerState_ = PlayerState::IDLE;
	}

	if (!(playerState_ & PlayerState::ATTACK))
	{
		playerWeapon_->OffCollAttack();
		if (myActorCollision_.isCollAttack_)
			actorAttackRagTime_->SetTotalNowTime(ATTACK_RAG_MAX);

		return;
	}

	playerState_ = static_cast<PlayerState::STATE>(playerState_ &
		(~(PlayerState::IDLE | PlayerState::MIDDLE_ATTACK)));

	if (actorAnime_->IsPlayAnim())return;

	playerData_.attackPower_ = defAttackPower_;
	EndGravity();
	StopWaveSound();

	if (playerState_ & PlayerState::CHARGE)
	{
		EffectResourceManager::EFFECT_RESOURCE_ID effectId;
		SoundResourceManager::SOUND_RESOURCE_ID soundId;

		if (playerState_ & PlayerState::NORMAL_CHARGE)
		{
			myActorCollision_.isCollHorizontal = true;
			EndInertia();
		}
		else if (playerState_ & PlayerState::FORWARD_CHARGE)
		{
			effectId = EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_DEVIATION;
			actorEffectLaunch_->FollowPlay(effectId,
				GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SLASH)),
				actorTransform_->GetPosReference(), &actorTransform_->GetRotReference());
			soundId = SoundResourceManager::SOUND_RESOURCE_ID::SWORD_ATTACK;
			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
		}
		else if (playerState_ & PlayerState::HORIZONTAL_CHARGE)
		{
			effectId = EffectResourceManager::EFFECT_RESOURCE_ID::SLASH_FLASH;
			actorEffectLaunch_->Play(effectId,
				GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SLASH)));

			Position3DF effectPos = actorTransform_->GetPos();

			const float SLASH_EFFECT_OFFSET = 200.0f;
			effectPos = VAdd(effectPos, VScale(actorTransform_->GetUp(), SLASH_EFFECT_OFFSET));
			actorEffectLaunch_->SetPos(effectId, effectPos);
			actorEffectLaunch_->SetRot(effectId, actorTransform_->GetRot());

			soundId = SoundResourceManager::SOUND_RESOURCE_ID::SWORD_ATTACK;
			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
		}

		playerState_ = PlayerState::IDLE;
	}
	else if (playerState_ & PlayerState::ATTACK_1)
		playerState_ = static_cast<PlayerState::STATE>(PlayerState::ATTACK_2 | PlayerState::MIDDLE_ATTACK);
	else if (playerState_ & PlayerState::ATTACK_2)
		playerState_ = static_cast<PlayerState::STATE>(PlayerState::ATTACK_3 | PlayerState::MIDDLE_ATTACK);
	else if (playerState_ & PlayerState::ATTACK_3)
		playerState_ = static_cast<PlayerState::STATE>(PlayerState::ATTACK_4 | PlayerState::MIDDLE_ATTACK);
	else if (playerState_ & PlayerState::ATTACK_4)
		playerState_ = static_cast<PlayerState::STATE>(PlayerState::ATTACK_5 | PlayerState::MIDDLE_ATTACK);
	else if (playerState_ & PlayerState::ATTACK_5)
		playerState_ = static_cast<PlayerState::STATE>(PlayerState::ATTACK_6 | PlayerState::MIDDLE_ATTACK);
	else if (playerState_ & PlayerState::ATTACK_6)
		playerState_ = PlayerState::IDLE;
	else if (playerState_ & PlayerState::SKY_ATTACK_1)
		playerState_ = static_cast<PlayerState::STATE>(PlayerState::JUMP |
			PlayerState::SKY_ATTACK_2 | PlayerState::MIDDLE_ATTACK);
	else if (playerState_ & PlayerState::SKY_ATTACK_2)
		playerState_ = static_cast<PlayerState::STATE>(PlayerState::JUMP |
			PlayerState::FALL_ATTACK | PlayerState::MIDDLE_ATTACK);
	else if (playerState_ & PlayerState::FALL_ATTACK)
		playerState_ = PlayerState::IDLE;
}

void Player::EndJump(void)
{
	if (!(playerState_ & PlayerState::JUMP &&
		!(playerState_ & PlayerState::ATTACK) &&
		!(playerState_ & PlayerState::MIDDLE_ATTACK)))return;
	
	playerState_ = static_cast<PlayerState::STATE>(PlayerState::JUMP | (playerState_ & PlayerState::MOVE));

	if (isTouchGround_ && !actorAnime_->IsPlayAnim())
	{
		EndGravity();
		playerState_ = PlayerState::IDLE;
	}
}

void Player::EndFlinch(void)
{
	if (!(playerState_ & PlayerState::HIT_LEFT ||
		playerState_ & PlayerState::HIT_RIGHT))return;
	
	if (actorAnime_->IsPlayAnim())return;
	
	EndInertia();
	playerState_ = PlayerState::IDLE;
}

void Player::EndWalk(void)
{
	if (!(playerState_ & PlayerState::WALK))return;
	
	if (actorAnime_->IsPlayAnim())return;
	
	isBattleStart_ = true;
	playerState_ = PlayerState::IDLE;
}

void Player::ChagePlayerSkill(float mul)
{
	playerData_.skillGage_ += SKILL_CHARGE * mul;

	if (playerData_.skillGage_ > playerData_.skillGageMax_)
		playerData_.skillGage_ = playerData_.skillGageMax_;
}

void Player::RepairPlayerHp(void)
{
	playerData_.hp_ += 0.4f;
	if (playerData_.hp_ > playerData_.maxHp_)
		playerData_.hp_ = playerData_.maxHp_;
}

void Player::SetUpInertia(Dir3DF dir, float power)
{
	inertia_ = VScale(dir, power);
	isInertia_ = true;
}

void Player::EndGravity(void)
{
	isGravity_ = false;
	gravity_ = VECTOR();
}

void Player::EndInertia(void)
{
	isInertia_ = false;
	inertia_ = VECTOR();
}

void Player::DrawSpecial(void)
{
	playerWeapon_->Draw();
}

void Player::DrawIconRecastTime(void)
{
	if (playerTechnique_.techniqueTimer.GetTotalNowTime() >= playerTechnique_.recastTimeMax)return;

	// 秒数を計測
	// 10の位
	const int TEN_RANK = 10;

	float param = playerTechnique_.recastTimeMax - playerTechnique_.techniqueTimer.GetTotalNowTime();
	int rateD = static_cast<int>(param);

	imgTimesBig_->DrawImageDiv(rateD / TEN_RANK, timesBig_Digit_10_Pos_);
	imgTimesBig_->DrawImageDiv(rateD % TEN_RANK);

	param = param - static_cast<float>(rateD);

	// 小数点以下の数字の描画

	// 少数第2位まで小数点を移動させる
	param *= 100.0f;

	rateD = static_cast<int>(param);

	imgTimesSmall_->DrawImageDiv(rateD / TEN_RANK, timesSmall_Point_10_Pos_);
	imgTimesSmall_->DrawImageDiv(rateD % TEN_RANK);
}