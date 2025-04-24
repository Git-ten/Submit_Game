#include "../../UTILITY/Utility.h"
#include "../../Common/DataLoad/LoadDataEffect.h"
#include "../../Application/Application.h"
#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "EnemyBase.h"

#pragma region 定数宣言

const float ATTACK_COLL_SPHERE_SIZE = 400.0f;

const float FLINCH_RATE = 0.25f;
const float DOWN_RATE = 0.3f;

const float ENEMY_DAMAGE_EFFECT_OFFSET = 200.0f;

#pragma endregion

EnemyBase::EnemyBase(void):
	enemyLevel_(ActorBase::ENEMY_LEVEL_EASY),
	enemyState_(EnemyState::IDLE)
{
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::InitActorTransform(void)
{
}

void EnemyBase::InitActorAnimation(void)
{
}

void EnemyBase::InitActorUi(void)
{
	const Color3D color = {0, 0, 0};// 真っ黒

	LoadData::LoadDataInfo info = { "Enemy_UserInterfaceInfo.json", "EnemyHp" };

	actorUi_.push_back(std::make_shared<Image>(info,
		ResourceManager::RESOURCE_ID::ENEMY_HP_CASE,
		Image::IMAGE_TYPE::NORMAL));
	actorUi_.push_back(std::make_shared<Image>(info,
		ResourceManager::RESOURCE_ID::ENEMY_HP,
		Image::IMAGE_TYPE::GAGE));
	actorUi_[1]->SetUpImageGageRate(enemyData_.hp_, enemyData_.maxHp_, color);
	actorUi_[1]->SetUpImageEventGageRate(enemyData_.hp_);

	info.dataName = "DownGage";
	actorUi_.push_back(std::make_shared<Image>(info,
		ResourceManager::RESOURCE_ID::DOWN_GAGE_CASE,
		Image::IMAGE_TYPE::NORMAL));
	actorUi_.push_back(std::make_shared<Image>(info,
		ResourceManager::RESOURCE_ID::DOWN_GAGE,
		Image::IMAGE_TYPE::GAGE));
	actorUi_[3]->SetUpImageGageRate(enemyData_.downGage_, enemyData_.downGageMax_, color);

	info.dataName = "EnemyName";
	actorUi_.push_back(std::make_shared<Image>(info,
		enemyData_.enemyNameId_,
		Image::IMAGE_TYPE::NORMAL));
}

void EnemyBase::InitActorEffectInfo(void)
{
	LoadData::LoadDataInfo info = { "Enemy_EffectInfo.json", "DemonReady" };
	DataEffect data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::ATTACK_READY), data);

	info.dataName = "DemonClaw";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::NORMAL_ATTACK), data);

	info.dataName = "DemonBomb";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::ATTACK_BOMB), data);

	info.dataName = "AttackSmoke";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::ATTACK_SMOKE), data);

	info.dataName = "Awayke_1";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::AWAKE_1), data);

	info.dataName = "Awayke_2";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::AWAKE_2), data);

	info.dataName = "AwakeAttack";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::AWAKE_ATTACK), data);

	info.dataName = "StageEffect";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::STAGE_EFFECT), data);

	info.dataName = "DamageSpark";
	data = LoadDataEffect::LoadedDataEffect(Application::GetInstance().GetDataPaths(), info);
	effectInfo_.emplace(static_cast<int>(EFFECT_USES::DAMAGE_SPARK), data);

}

void EnemyBase::InitActorCameraControl(void)
{
	LoadData::LoadDataInfo info = { "CameraShakeInfo.json", "Direction_1" };

	AddStateCameraControlData(static_cast<int>(EnemyState::HIT_DOWN), info);
	AddStateCameraControlData(static_cast<int>(EnemyState::HIT_RIGHT), info);
	AddStateCameraControlData(static_cast<int>(EnemyState::HIT_LEFT), info);
}

void EnemyBase::InitActorSoundLaunch(void)
{
	InitSpecialSound();
}

void EnemyBase::InitDamageEffect(void)
{
	if(actorEffectLaunch_ == nullptr)
		actorEffectLaunch_ = std::make_unique<EffectPlayLaunch>();


	EffectResourceManager::EFFECT_RESOURCE_ID id = 
		EffectResourceManager::EFFECT_RESOURCE_ID::DAMAGE_SPARK;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);
}

void EnemyBase::InitSpecialSound(void)
{
	if (waveLaunch_ == nullptr)
		waveLaunch_ = std::make_unique<SoundPlayLaunch>();

	SoundResourceManager::SOUND_RESOURCE_ID id = 
		SoundResourceManager::SOUND_RESOURCE_ID::ENEMY_FLINCH;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::ENEMY_DOWN;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::SWORD_ATTACK;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);
}

void EnemyBase::CollisionedHorizontalActor(void)
{
	VECTOR vec = VSub(actorTransform_->GetPrevPos(), *enemyData_.pos_);
	vec.y = 0.0f;
	ActorMoveDistance(enemyData_, VNorm(vec), VSize(vec));
}

void EnemyBase::SetOtherActorCollisionSpecial(ACTOR_COLLISION* actorColl)
{
	if (enemyAi_ == nullptr)return;

	if (actorColl->lockOnPos != nullptr)
		enemyAi_->SetUpIsDistance(*enemyData_.pos_, *actorColl->lockOnPos);
}

void EnemyBase::InitSpecial(void)
{
}

float EnemyBase::NowHp(void)
{
	return enemyData_.hp_;
}

void EnemyBase::UpdateSpecial(void)
{
}

void EnemyBase::CollisionedGravityActor(Position3DF pos)
{
}

void EnemyBase::CollisionedDamageActor(float* damagePower)
{
	if (damagePower == nullptr)return;

	enemyData_.hp_ -= *damagePower;

	EffectResourceManager::EFFECT_RESOURCE_ID effectId =
		EffectResourceManager::EFFECT_RESOURCE_ID::DAMAGE_SPARK;
	actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::DAMAGE_SPARK)));
	actorEffectLaunch_->SetPos(effectId,
		VAdd(actorTransform_->GetPos(), VScale(actorTransform_->GetUp(), ENEMY_DAMAGE_EFFECT_OFFSET)));

	SoundResourceManager::SOUND_RESOURCE_ID soundId = 
		SoundResourceManager::SOUND_RESOURCE_ID::SWORD_ATTACK;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);

	SceneManager::GetInstance().GetCamera().lock()->
		SetUpCameraControlData(cameraControle_.at(static_cast<int>(EnemyState::HIT_RIGHT)));

	if (enemyState_ & EnemyState::HIT_DOWN ||
		enemyState_ & EnemyState::AWAKE)
	{
		enemyData_.flinchParam_ = GAGE_INIT;
		enemyData_.downGage_ = GAGE_INIT;
		return;
	}

	enemyData_.flinchParam_ += *damagePower * FLINCH_RATE;
	enemyData_.downGage_ += *damagePower * DOWN_RATE;

}

void EnemyBase::SetUpEnemyPalam(void)
{
	if (actorTransform_ == nullptr)return;

	enemyData_.pos_ = &actorTransform_->GetPosReference();

	enemyData_.collisionSphere_ = std::make_shared<SphereF>(
		VAdd(*enemyData_.pos_, VScale(Utility::DIR_D, enemyData_.collisionSphereOffset_)),
		enemyData_.collitionSphereSize_);


	enemyData_.collitionGravity_ = std::make_shared<Line3DF>(
		VAdd(*enemyData_.pos_, VScale(actorTransform_->GetUp(), GRAVITY_UP_OFFSET)),
		VAdd(*enemyData_.pos_, VScale(actorTransform_->GetDown(), GRAVITY_DOWN_OFFSET)));

	enemyData_.collisionAttackSphere_ = std::make_shared<SphereF>(
		*enemyData_.pos_, ATTACK_COLL_SPHERE_SIZE);

	myActorCollision_.lockOnPos = &actorTransform_->GetPosReference();
	myActorCollision_.collisionGravity = enemyData_.collitionGravity_;
	myActorCollision_.collisionSphere = enemyData_.collisionSphere_;
	myActorCollision_.collisionModelId = &actorTransform_->GetModelIdReference();

	myActorCollision_.collisionAttackSphere = enemyData_.collisionAttackSphere_;

	myActorCollision_.isMyselfEnemy = true;

	myActorCollision_.damagePower = &enemyData_.attackPower_;
}

void EnemyBase::SetUpEnemyLevelParam(void)
{
	SceneManager::SelectSceneToGameScene data = SceneManager::GetInstance().GetSelectToGameSceneData();
	enemyLevel_ = data.enemyLevel;

	enemyData_.hp_ *= data.enemyLevel;
	enemyData_.maxHp_ *= data.enemyLevel;
	enemyData_.attackPower_ *= data.enemyLevel;
	enemyData_.downGage_ *= enemyData_.downGageMax_ * (ENEMY_LEVEL_HARD - data.enemyLevel);
	enemyData_.flinchParam_ *= enemyData_.flinchParamMax_ * (ENEMY_LEVEL_HARD - data.enemyLevel);
}

const bool EnemyBase::IsEndAction(void) const
{
	if (enemyAi_ == nullptr || actorAnime_ == nullptr)return true;

	return !actorAnime_->IsPlayAnim() && !enemyAi_->IsEndAIAction();
}

void EnemyBase::EnemyAttack(void)
{
}

void EnemyBase::EnemyMove(void)
{
	if (!(enemyState_ & EnemyState::WALK))return;

	ActorOtherLockOnRotation(enemyData_);

	ActorMoveVector(enemyData_, VNorm(GetActorOtherLockonDir(enemyData_)));

	//AIのアクションの終了
	if(enemyAi_ != nullptr)enemyAi_->EndAIAction();
}

void EnemyBase::Flinch(void)
{
	if (enemyState_ & EnemyState::AWAKE)return;
	if (enemyState_ & EnemyState::DEATH)return;
	if (enemyData_.flinchParam_ < enemyData_.flinchParamMax_)return;

	enemyData_.flinchParam_ = GAGE_INIT;

	if(enemyAi_ != nullptr)enemyAi_->PerfectLock();

	// ランダム
	const int RANDOM_MAX = 1;
	int rand = GetRand(RANDOM_MAX);

	// 0～1までのどちらか
	switch (rand)
	{
	case 0:
		enemyState_ = EnemyState::HIT_LEFT;
		break;
	case RANDOM_MAX:
		enemyState_ = EnemyState::HIT_RIGHT;
		break;
	default:
		break;
	}

	// 怯み効果音
	SoundResourceManager::SOUND_RESOURCE_ID soundId =
		SoundResourceManager::SOUND_RESOURCE_ID::ENEMY_FLINCH;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
}

void EnemyBase::Death(void)
{
	if (enemyData_.hp_ <= HP_MIN)
	{
		enemyAi_->PerfectLock();
		enemyState_ = EnemyState::DEATH;
	}
}

void EnemyBase::Down(void)
{
	if (enemyState_ & EnemyState::AWAKE)return;
	if (enemyState_ & EnemyState::DEATH)return;
	if (enemyData_.downGage_ < enemyData_.downGageMax_)return;

	enemyData_.downGage_ = GAGE_INIT;
	enemyState_ = EnemyState::HIT_DOWN;

	if(enemyAi_ != nullptr)enemyAi_->PerfectLock();

	enemyDownTime_.InitTotalNowTime();

	// 怯み効果音
	SoundResourceManager::SOUND_RESOURCE_ID soundId =
		SoundResourceManager::SOUND_RESOURCE_ID::ENEMY_DOWN;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
}

void EnemyBase::SpecialStateAnime(void)
{
	if (enemyState_ & EnemyState::HIT_LEFT ||
		enemyState_ & EnemyState::HIT_RIGHT)
	{
		if (!actorAnime_->IsPlayAnim())
			enemyAi_->UnPerfectLock();
	}

	if (enemyState_ & EnemyState::GET_UP)
	{
		if (!actorAnime_->IsPlayAnim())
			enemyAi_->UnPerfectLock();
	}

	if (enemyState_ & EnemyState::HIT_DOWN)
	{
		if (enemyDownTime_.GetTotalNowTime() > DOWN_TIME)
			enemyState_ = EnemyState::GET_UP;
	}

	SpecialStateAnimeAdd();
}

void EnemyBase::SpecialStateAnimeAdd(void)
{
}

void EnemyBase::SetUpCollSphere(const ATTACK_BONE& bone, bool isMyBody)
{
	if (myActorCollision_.collisionAttackSphere.lock() == nullptr)return;
	if (enemyData_.collBones_.size() - 1 < static_cast<int>(bone))return;

	if (isMyBody)
	{
		myActorCollision_.collisionAttackSphere.lock()->pos = *enemyData_.pos_;
		return;
	}

	int flameIdx = MV1SearchFrame(actorTransform_->GetModelId(),
		enemyData_.collBones_[static_cast<int>(bone)].c_str());

	myActorCollision_.collisionAttackSphere.lock()->pos =
		MV1GetFramePosition(actorTransform_->GetModelId(), flameIdx);
}

void EnemyBase::SetUpCollEffectSphere(EffectResourceManager::EFFECT_RESOURCE_ID effectId)
{
	if (!actorEffectLaunch_->IsPlayEffect(effectId))return;
	if (myActorCollision_.collisionAttackSphere.lock() == nullptr)return;

	myActorCollision_.collisionAttackSphere.lock()->pos = actorEffectLaunch_->GetPos(effectId);
}

void EnemyBase::DrawSpecial(void)
{
}