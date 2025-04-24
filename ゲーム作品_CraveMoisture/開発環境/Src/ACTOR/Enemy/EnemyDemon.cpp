#include "LoadEnemyData.h"
#include "EnemyState.h"
#include "../../Application/Application.h"
#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "../../Common/DataLoad/LoadDataAnimation.h"
#include "../../Common/Physics.h"
#include "../../Common/IsDistance.h"
#include "LoadEnemyData.h"
#include "EnemyDemon.h"

#pragma region 定数宣言

const float DEMON_AWAKE_MIN = 1.8f;
const float DEMON_AWAKE_MAX = 1.9f;

const float CLOW_SPEED = 200.0f;

#pragma endregion

EnemyDemon::EnemyDemon(void):
	EnemyBase(),
	isDemonAwake_(false)
{
}

EnemyDemon::~EnemyDemon(void)
{
}

void EnemyDemon::ActorBattleStart(void)
{
	// 叫び声
	if (!(enemyState_ & EnemyState::ROAR))
	{
		SoundResourceManager::SOUND_RESOURCE_ID soundId =
			SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_ROAR;
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
	}

	enemyState_ = EnemyState::ROAR;
}

bool EnemyDemon::BattleStepCondition(void)
{
	// アクターのbattleステップが一つ進んでいる
	if (actorBattleStep_ >= 1)
		return enemyData_.hp_ <= ActorBase::HP_MIN;

	if (isBattleStart_)
		actorBattleStep_++;

	return isBattleStart_;
}

void EnemyDemon::InitActorTransform(void)
{
	LoadData::LoadDataInfo info = { "EnemyInfo.json", "Demon" };

	actorTransform_ = std::make_unique<Transform>(info, ResourceManager::RESOURCE_ID::ENEMY_DEMON);

	enemyData_ = LoadEnemyData::LoadedEnemyData(Application::GetInstance().GetDataPaths(), info);

	// ゲームの難易度を適用
	SetUpEnemyLevelParam();

	SetUpEnemyPalam();

	enemyData_.enemyNameId_ = ResourceManager::RESOURCE_ID::EVIL_GOA_NAME;
}

void EnemyDemon::InitActorAnimation(void)
{
	actorAnime_ = std::make_unique<AnimationController>(actorTransform_->GetModelId());

	LoadData::LoadDataInfo info = { "EnemyInfo.json", "IDLE" };
	DataAnimation data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::IDLE), data);
	actorAnime_->Add(static_cast<int>(EnemyState::AWAKE | EnemyState::IDLE), data);

	info.dataName = "WALK";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::WALK), data);
	actorAnime_->Add(static_cast<int>(EnemyState::AWAKE | EnemyState::WALK), data);

	info.dataName = "ROAR";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::ROAR), data);

	info.dataName = "ATTACK_1";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::ATTACK | EnemyState::ATTACK_1), data);
	actorAnime_->Add(static_cast<int>(EnemyState::AWAKE | EnemyState::ATTACK | EnemyState::ATTACK_1), data);

	info.dataName = "ATTACK_2";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::ATTACK | EnemyState::ATTACK_2), data);
	actorAnime_->Add(static_cast<int>(EnemyState::AWAKE | EnemyState::ATTACK | EnemyState::ATTACK_2), data);

	info.dataName = "ATTACK_3";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::ATTACK | EnemyState::ATTACK_3), data);

	info.dataName = "ATTACK_4";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::ATTACK | EnemyState::ATTACK_4), data);
	actorAnime_->Add(static_cast<int>(EnemyState::AWAKE | EnemyState::ATTACK | EnemyState::ATTACK_4), data);

	info.dataName = "ATTACK_5";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::ATTACK | EnemyState::ATTACK_5), data);
	actorAnime_->Add(static_cast<int>(EnemyState::AWAKE | EnemyState::ATTACK | EnemyState::ATTACK_5), data);

	info.dataName = "ATTACK_6";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::ATTACK | EnemyState::ATTACK_6), data);
	actorAnime_->Add(static_cast<int>(EnemyState::AWAKE | EnemyState::ATTACK | EnemyState::ATTACK_6), data);

	info.dataName = "HIT_LEFT";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::HIT_LEFT), data);

	info.dataName = "HIT_RIGHT";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::HIT_RIGHT), data);

	info.dataName = "HIT_DOWN";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::HIT_DOWN), data);

	info.dataName = "GET_UP";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::GET_UP), data);

	info.dataName = "DEATH";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::DEATH), data);

	info.dataName = "AWAYKE";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::AWAKE), data);

	info.dataName = "ATTACK_3_AWAKE";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::AWAKE | EnemyState::ATTACK | EnemyState::ATTACK_3), data);

	info.dataName = "ATTACK_AWAKE";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::AWAKE | EnemyState::ATTACK | EnemyState::ATTACK_AWAKE), data);


	actorAnime_->Start(static_cast<int>(EnemyState::IDLE));
}

void EnemyDemon::InitActorEffectLaunch(void)
{
	actorEffectLaunch_ = std::make_unique<EffectPlayLaunch>();

	InitDamageEffect();

	EffectResourceManager::EFFECT_RESOURCE_ID id =
		EffectResourceManager::EFFECT_RESOURCE_ID::ENEMY_ATTACK_READY;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_LEFT;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_RIGHT;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_DUO;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::BOMB;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::BLACK_SMOKE;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_DESTROY;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_1;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_2;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_ATTACK_1;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_ATTACK_2;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

	id = EffectResourceManager::EFFECT_RESOURCE_ID::STAGE_FIRE;
	actorEffectLaunch_->AddPlayEffect(id,
		EffectResourceManager::GetInstance().Load(id).handleId_);

}

void EnemyDemon::InitActorSoundLaunch(void)
{
	waveLaunch_ = std::make_unique<SoundPlayLaunch>();

	InitSpecialSound();

	SoundResourceManager::SOUND_RESOURCE_ID id =
		SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_SLASH;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_BOM;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_ROAR;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_AWAKE_1;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_AWAKE_2;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);

	id = SoundResourceManager::SOUND_RESOURCE_ID::METEO;
	waveLaunch_->AddPlaySound(id, SoundResourceManager::GetInstance().Load(id).handleId_);
}

void EnemyDemon::InitSpecial(void)
{
	InitActorEffectLaunch();

	enemyAi_ = std::make_unique<EnemyAIBase>(enemyState_,
		[&]() {return actorTransform_->GetBack(); },
		enemyData_.ragTimeData_);

	enemyAi_->Init();

	enemyDownTime_.SetTotalNowTime(DOWN_TIME);

	actorTransform_->InitVertexRenderer(Transform::TRANSFORM_TYPE::SKIN_MODEL);
}

void EnemyDemon::UpdateSpecial(void)
{
	// バトルが始まらない限り、AIは動かない
	if(isBattleStart_)
		enemyAi_->Update();

	actorAnime_->Play(static_cast<int>(enemyState_));

	Death();
	EnemyMove();
	EnemyAwake();
	EnemyAttack();
	Flinch();
	Down();


	if (!isTouchGround_)Gravity(enemyData_);

	enemyDownTime_.UpdateTime();

	SpecialStateAnime();

}

void EnemyDemon::EnemyAttack(void)
{
	if (!(enemyState_ & EnemyState::ATTACK))
	{
		StopAttackEffect();
		myActorCollision_.isCollAttack_ = false;
		myActorCollision_.collisionAttackSphere.lock()->pos = *enemyData_.pos_;
		actorAttackRagTime_->InitTotalNowTime();
		return;
	}

	if (actorAttackRagTime_->GetTotalNowTime() > ENEMT_ATTACK_RAG)
	{
		actorAttackRagTime_->InitTotalNowTime();
		myActorCollision_.isCollAttack_ = true;
	}
	
	EnemyAttack_1();
	EnemyAttack_2();
	EnemyAttack_3();
	EnemyAttack_4();
	EnemyAttack_5();
	EnemyAttack_6();
	EnemyAwakeAttack();
}

void EnemyDemon::EnemyAttack_1(void)
{
	if (!(enemyState_ & EnemyState::ATTACK_1))return;
	
	if (IsEndAction())
	{
		//AIを短時間思考停止
		enemyAi_->LockThinkShortTime();

		//ラグをセット
		enemyAi_->SetUpRagTime(EnemyAIBase::RAG_NAME::ATTACK_1_RAG_TIME);

		//AIのアクションの終了
		enemyAi_->EndAIAction();
	}

	SoundResourceManager::SOUND_RESOURCE_ID soundId =
		SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_SLASH;

	// 爪(右)エフェクト
	EffectResourceManager::EFFECT_RESOURCE_ID effectId =
		EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_RIGHT;

	const float MOVE_DIS = 40.0f;// 一フレームの移動距離

	if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_1)
	{
		ActorOtherLockOnRotation(enemyData_);
	}
	else if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_1 * ENEMY_ANIME_DIGIT_MIDLE)
	{
		if (GetDistanceActor(enemyData_) > IsDistance::CONDITION_MOST_NEAR_DISTANCE)
			ActorMoveDistance(enemyData_, actorTransform_->GetForward(), MOVE_DIS);

		HappenNormalClowEffect(effectId);

		// 土煙エフェクト
		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::BLACK_SMOKE;
		actorEffectLaunch_->FollowPlay(effectId,
			GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SMOKE)),
			actorTransform_->GetPosReference());

		// 効果音発生
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);

		SetUpCollSphere(ATTACK_BONE::RIGHT_HAND);
	}
	else if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_1 * ENEMY_ANIME_DIGIT_MAX)
	{
		// 爪(左)エフェクト
		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_LEFT;
		HappenNormalClowEffect(effectId);

		// 効果音発生
		if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_1 * ENEMY_ANIME_DIGIT_MIDLE)
			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);

		SetUpCollSphere(ATTACK_BONE::LEFT_HAND);
	}
}

void EnemyDemon::EnemyAttack_2(void)
{
	if (!(enemyState_ & EnemyState::ATTACK_2))return;

	if (IsEndAction())
	{
		//ラグをセット
		enemyAi_->SetUpRagTime(EnemyAIBase::RAG_NAME::ATTACK_2_RAG_TIME);

		//AIのアクションの終了
		enemyAi_->EndAIAction();
	}

	if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_2)
	{
		ActorOtherLockOnRotation(enemyData_);
	}
	else
	{
		// 土煙エフェクト
		EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
			EffectResourceManager::EFFECT_RESOURCE_ID::BLACK_SMOKE;
		actorEffectLaunch_->FollowPlay(effectId,
			GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SMOKE)),
			actorTransform_->GetPosReference());


		SetUpCollSphere(ATTACK_BONE::HEAD);
	}
}

void EnemyDemon::EnemyAttack_3(void)
{
	if (!(enemyState_ & EnemyState::ATTACK_3))return;

	static bool isAwake = false;
	static bool isAwakeAttack = false;

	if (enemyState_ & EnemyState::AWAKE && !isAwake)
	{
		isAwake = true;
		enemyState_ = static_cast<EnemyState::STATE>(EnemyState::ATTACK & EnemyState::ATTACK_3);
	}

	if (IsEndAction())
	{
		if (isAwake && !isAwakeAttack)
		{
			isAwakeAttack = true;
			enemyState_ = static_cast<EnemyState::STATE>(EnemyState::AWAKE |
				EnemyState::ATTACK | EnemyState::ATTACK_3);
			actorAnime_->Play(enemyState_);
		}
		else
		{
			//AIを短時間思考停止
			enemyAi_->LockThinkShortTime();

			//ラグをセット
			enemyAi_->SetUpRagTime(EnemyAIBase::RAG_NAME::ATTACK_3_RAG_TIME);

			//AIのアクションの終了
			enemyAi_->EndAIAction();

			isAwake = false;
			isAwakeAttack = false;
		}
	}

	SoundResourceManager::SOUND_RESOURCE_ID soundId =
		SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_SLASH;

	EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
		EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW;

	if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_3)
	{
		ActorOtherLockOnRotation(enemyData_);
	}
	else if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_3 * ENEMY_ANIME_DIGIT_MOST_MIN)
	{
		actorEffectLaunch_->Stop(effectId);
		actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::NORMAL_ATTACK)));
		actorEffectLaunch_->SetPos(effectId, actorTransform_->GetPos());
		actorEffectLaunch_->SetRot(effectId, actorTransform_->GetRot());

		// 土煙エフェクト
		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::BLACK_SMOKE;
		actorEffectLaunch_->FollowPlay(effectId,
			GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SMOKE)),
			actorTransform_->GetPosReference());

		// 効果音発生
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);

	}
	else if (enemyState_ & EnemyState::AWAKE &&
		actorAnime_->GetStepAnimTime() > enemyData_.collStepData_.attackCollStep_3 * DEMON_AWAKE_MIN &&
		actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_3 * DEMON_AWAKE_MAX)
	{
		actorEffectLaunch_->Stop(effectId);
		actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::NORMAL_ATTACK)));
		actorEffectLaunch_->SetPos(effectId, actorTransform_->GetPos());
		actorEffectLaunch_->SetRot(effectId, actorTransform_->GetRot());

		// 土煙エフェクト
		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::BLACK_SMOKE;
		actorEffectLaunch_->FollowPlay(effectId,
			GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SMOKE)),
			actorTransform_->GetPosReference());

		// 効果音発生
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK, true);
	}

	actorEffectLaunch_->MoveEffect(effectId, VNorm(GetActorOtherLockonDir(enemyData_)), CLOW_SPEED * enemyLevel_);
	SetUpCollEffectSphere(effectId);
}

void EnemyDemon::EnemyAttack_4(void)
{
	if (!(enemyState_ & EnemyState::ATTACK_4))return;
	
	if (IsEndAction())
	{
		//AIを短時間思考停止
		enemyAi_->LockThinkShortTime();

		//ラグをセット
		enemyAi_->SetUpRagTime(EnemyAIBase::RAG_NAME::ATTACK_4_RAG_TIME);

		//AIのアクションの終了
		enemyAi_->EndAIAction();
	}

	if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_4)
	{
		ActorOtherLockOnRotation(enemyData_);
	}
	else
	{
		SoundResourceManager::SOUND_RESOURCE_ID soundId =
			SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_SLASH;

		EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
			EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW;
		HappenNormalClowEffect(effectId);

		// 土煙エフェクト
		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::BLACK_SMOKE;
		actorEffectLaunch_->FollowPlay(effectId,
			GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SMOKE)),
			actorTransform_->GetPosReference());

		SetUpCollSphere(ATTACK_BONE::LEFT_HAND);

		// 効果音発生
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
	}
}

void EnemyDemon::EnemyAttack_5(void)
{
	if (!(enemyState_ & EnemyState::ATTACK_5))return;
	
	if (IsEndAction())
	{
		//AIを短時間思考停止
		enemyAi_->LockThinkShortTime();

		//ラグをセット
		enemyAi_->SetUpRagTime(EnemyAIBase::RAG_NAME::ATTACK_5_RAG_TIME);

		//AIのアクションの終了
		enemyAi_->EndAIAction();
	}

	EffectResourceManager::EFFECT_RESOURCE_ID effectId;

	if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_5)
	{
		ActorOtherLockOnRotation(enemyData_);
	}
	else if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_5 * ENEMY_ANIME_DIGIT_MIDLE)
	{
		gravity_ = VScale(Physics::GRAVITY_DIR, static_cast<float>(-enemyData_.jumpPower_));
		isTouchGround_ = false;// 初期だけは強制的に地面判定を無効無効

		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::BLACK_SMOKE;
		actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SMOKE)));
		actorEffectLaunch_->SetPos(effectId, actorTransform_->GetPos());
	}
	else if (GetDistanceActor(enemyData_) > IsDistance::CONDITION_NEAR_DISTANCE)
	{
		const float MOVE_DIS = 40.0f;
		ActorMoveDistance(enemyData_, actorTransform_->GetForward(), MOVE_DIS);
	}

	// アニメステップの少し先
	if (actorAnime_->GetStepAnimTime() > enemyData_.collStepData_.attackCollStep_5 * (ENEMY_ANIME_DIGIT_MAX + 0.8f))
	{
		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_DUO;
		HappenNormalClowEffect(effectId);
		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::BOMB;
		actorEffectLaunch_->FollowPlay(effectId,
			GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_BOMB)),
			actorTransform_->GetPosReference());

		SoundResourceManager::SOUND_RESOURCE_ID soundId = 
			SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_BOM;
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);

		SetUpCollSphere(ATTACK_BONE::LEFT_HAND, true);
	}
}

void EnemyDemon::EnemyAttack_6(void)
{
	if (!(enemyState_ & EnemyState::ATTACK_6))return;

	static bool isAwake = false;
	static bool isAwakeAttack = false;

	SoundResourceManager::SOUND_RESOURCE_ID soundId =
		SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_SLASH;

	EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
		EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_DESTROY;

	if (enemyState_ & EnemyState::AWAKE && !isAwake)
	{
		isAwake = true;
		enemyState_ = static_cast<EnemyState::STATE>(EnemyState::ATTACK & EnemyState::ATTACK_6);
	}

	if (IsEndAction())
	{
		if (isAwake && !isAwakeAttack)
		{
			isAwakeAttack = true;

			enemyState_ = static_cast<EnemyState::STATE>(EnemyState::AWAKE |
				EnemyState::ATTACK | EnemyState::ATTACK_6);

			actorAnime_->Play(enemyState_);
			actorEffectLaunch_->Stop(effectId);
		}
		else
		{
			//AIを長時間思考停止
			enemyAi_->LockThinkLongTime();

			//ラグをセット
			enemyAi_->SetUpRagTime(EnemyAIBase::RAG_NAME::ATTACK_6_RAG_TIME);

			//AIのアクションの終了
			enemyAi_->EndAIAction();

			isAwake = false;
			isAwakeAttack = false;
		}
	}

	if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_6)
	{
		ActorOtherLockOnRotation(enemyData_);
	}
	else
	{
		actorEffectLaunch_->SetPos(effectId, actorTransform_->GetPos());
		actorEffectLaunch_->SetRot(effectId, actorTransform_->GetRot());

		SetUpCollEffectSphereDotDir(effectId);

		actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::NORMAL_ATTACK)));

		// 土煙エフェクト
		effectId = EffectResourceManager::EFFECT_RESOURCE_ID::BLACK_SMOKE;
		actorEffectLaunch_->FollowPlay(effectId,
			GetEffectDataInfo(static_cast<int>(EFFECT_USES::ATTACK_SMOKE)),
			actorTransform_->GetPosReference());

		// 効果音発生
		waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
	}
}

void EnemyDemon::EnemyAwakeAttack(void)
{
	if (!(enemyState_ & EnemyState::ATTACK_AWAKE))return;
	
	if (IsEndAction())
	{
		//AIを長時間思考停止
		enemyAi_->LockThinkLongTime();

		//AIのアクションの終了
		enemyAi_->EndAIAction();

		//ラグをセット
		enemyAi_->SetUpRagTime(EnemyAIBase::RAG_NAME::ATTACK_AWAKE_RAG_TIME);
	}

	EffectResourceManager::EFFECT_RESOURCE_ID effectId = 
		EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_ATTACK_1;
	actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::STAGE_EFFECT)));

	SoundResourceManager::SOUND_RESOURCE_ID soundId = 
		SoundResourceManager::SOUND_RESOURCE_ID::METEO;
	waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);

	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_ATTACK_2;

	Position3DF pos;
	if (!actorEffectLaunch_->IsPlayEffect(effectId))
	{
		const float EFFECT_UP_OFFSET = 3000.0f;

		actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::AWAKE_ATTACK)));
		pos = actorTransform_->GetPos();
		pos = VAdd(pos, VScale(actorTransform_->GetUp(), EFFECT_UP_OFFSET));
		actorEffectLaunch_->SetPos(effectId, pos);
	}

	SetUpCollEffectSphere(effectId);


	const float EFFECT_METEO_MIN_HIGHT = 600.0f;
	pos = actorEffectLaunch_->GetPos(effectId);
	if (pos.y > EFFECT_METEO_MIN_HIGHT && actorAnime_->GetStepAnimTime() > enemyData_.collStepData_.attackCollAwake)
	{
		ActorOtherLockOnRotation(enemyData_);

		Dir3DF dir = GetActorOtherLockonDir(enemyData_, false, actorEffectLaunch_->GetPos(effectId));
		dir = VNorm(dir);

		const float MOVE_DIS = 100.0f;
		const float MOVE_DIS_SLOW = 2.0f;
		actorEffectLaunch_->MoveEffect(effectId, dir, MOVE_DIS);
		pos = actorEffectLaunch_->GetPos(effectId);
		if (pos.y <= EFFECT_METEO_MIN_HIGHT)
		{
			actorEffectLaunch_->SetSpeed(effectId, MOVE_DIS_SLOW);

			soundId = SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_BOM;
			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
		}
	}
}

void EnemyDemon::EnemyAwake(void)
{
	if (enemyState_ & EnemyState::DEATH)return;

	if (isDemonAwake_)return;

	SoundResourceManager::SOUND_RESOURCE_ID soundId = 
		SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_AWAKE_1;

	
	// HPの半分を切った
	if (enemyData_.hp_ < enemyData_.maxHp_ * 0.5f)
	{
		enemyAi_->PerfectLock();

		EffectResourceManager::EFFECT_RESOURCE_ID effectId =
			EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_1;

		const float DEMON_AWAKE_ANIME_NEXT_STEP = 130.0f;

		if (actorAnime_->GetStepAnimTime() < DEMON_AWAKE_ANIME_NEXT_STEP)
		{
			actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::AWAKE_1)));
			actorEffectLaunch_->SetPos(effectId, actorTransform_->GetPos());

			waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
		}
		else if(enemyState_ & EnemyState::AWAKE)
		{
			effectId = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_2;
			
			static bool isFirst = false;
			if (!isFirst)
			{
				isFirst = true;
				actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::AWAKE_1)));
				actorEffectLaunch_->SetPos(effectId, actorTransform_->GetPos());

				soundId = SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_AWAKE_2;
				waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
				soundId = SoundResourceManager::SOUND_RESOURCE_ID::EVIL_GOA_ROAR;
				waveLaunch_->Play(soundId, DX_PLAYTYPE_BACK);
			}
			else if (!actorEffectLaunch_->IsPlayEffect(effectId))
			{
				isFirst = false;
				isDemonAwake_ = true;
				// エネミーの完全ロックを解除
				enemyAi_->UnPerfectLock();
			}

			effectId = EffectResourceManager::EFFECT_RESOURCE_ID::STAGE_FIRE;
			actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::STAGE_EFFECT)));
			actorEffectLaunch_->SetPos(effectId, actorTransform_->GetPos());
		}

		enemyState_ = EnemyState::AWAKE;
	}
}

void EnemyDemon::SpecialStateAnimeAdd(void)
{
	if (enemyState_ & EnemyState::ROAR)
	{
		if (!actorAnime_->IsPlayAnim())
			isBattleStart_ = true;// 叫んだあとにバトルスタート
	}
}

void EnemyDemon::SetUpCollEffectSphereDotDir(EffectResourceManager::EFFECT_RESOURCE_ID effectId)
{
	const float INIT_ZERO = 0.0f;
	static float bius = INIT_ZERO;// 初期値

	if (!actorEffectLaunch_->IsPlayEffect(effectId))
		bius = INIT_ZERO;

	
	Dir3DF dir = VNorm(GetActorOtherLockonDir(enemyData_));

	// 軸
	Dir3DF vecDir = Utility::DIR_F;
	double angle = INIT_ZERO;// 初期値

	// 角度の倍数
	const double SEVENTEEN_VALUE = 17.0;

	double vecDirAngle = Utility::AngleDeg(vecDir, dir);
	double subAngle = vecDirAngle / SEVENTEEN_VALUE;

	angle = static_cast<double>(static_cast<int>(subAngle) * static_cast<int>(SEVENTEEN_VALUE));
	angle = Utility::Deg2RadD(angle);

	// 正負逆転
	if (VDot(Utility::DIR_L, dir) < 0.0f)
		angle *= -1.0;

	// 最終的な方向
	dir = Utility::RotXZPos(Utility::VECTOR_ZERO, vecDir, static_cast<float>(angle));
	dir = VNorm(dir);

	// エフェクトの速度に合わせて球も移動する
	bius += actorEffectLaunch_->GetSpeed(effectId) * EFFECT_DEFAULT_SPEED;

	myActorCollision_.collisionAttackSphere.lock()->pos = VAdd(actorTransform_->GetPos(), VScale(dir, bius));
	

	const float EFFECT_UP_OFFSET = 100.0f;
	myActorCollision_.collisionAttackSphere.lock()->pos = 
		VAdd(myActorCollision_.collisionAttackSphere.lock()->pos, 
			VScale(actorTransform_->GetUp(), EFFECT_UP_OFFSET));
}

void EnemyDemon::HappenNormalClowEffect(EffectResourceManager::EFFECT_RESOURCE_ID effectId)
{
	actorEffectLaunch_->Play(effectId, GetEffectDataInfo(static_cast<int>(EFFECT_USES::NORMAL_ATTACK)));

	const float EFFECT_UP_OFFSET = 100.0f;
	const float EFFECT_FORWARD_OFFSET = 1000.0f;
	Position3DF effectPos = VAdd(actorTransform_->GetPos(), VScale(actorTransform_->GetUp(), EFFECT_UP_OFFSET));
	effectPos = VAdd(effectPos, VScale(actorTransform_->GetForward(), EFFECT_FORWARD_OFFSET));

	actorEffectLaunch_->SetPos(effectId, effectPos);
	actorEffectLaunch_->SetRot(effectId, actorTransform_->GetRot());
}

void EnemyDemon::StopAttackEffect(void)
{
	EffectResourceManager::EFFECT_RESOURCE_ID effectId =
		EffectResourceManager::EFFECT_RESOURCE_ID::BOMB;
	actorEffectLaunch_->Stop(effectId);

	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW;
	actorEffectLaunch_->Stop(effectId);
	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_DESTROY;
	actorEffectLaunch_->Stop(effectId);
	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_DUO;
	actorEffectLaunch_->Stop(effectId);
	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_LEFT;
	actorEffectLaunch_->Stop(effectId);
	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_CLAW_RIGHT;
	actorEffectLaunch_->Stop(effectId);
	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::BLACK_SMOKE;
	actorEffectLaunch_->Stop(effectId);
	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::BLACK_SMOKE;
	actorEffectLaunch_->Stop(effectId);
	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::ENEMY_ATTACK_READY;
	actorEffectLaunch_->Stop(effectId);
	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_ATTACK_1;
	actorEffectLaunch_->Stop(effectId);
	effectId = EffectResourceManager::EFFECT_RESOURCE_ID::EVIL_GOA_AWAKE_ATTACK_2;
	actorEffectLaunch_->Stop(effectId);
}

void EnemyDemon::DrawSpecial(void)
{
}