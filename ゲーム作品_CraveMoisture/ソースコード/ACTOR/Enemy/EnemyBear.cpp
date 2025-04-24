#include "LoadEnemyData.h"
#include "../../Application/Application.h"
#include "../../Common/DataLoad/LoadDataAnimation.h"
#include "../AI/TutorialBearAI.h"
#include "EnemyBear.h"

EnemyBear::EnemyBear(void) :
	EnemyBase()
{
}

EnemyBear::~EnemyBear(void)
{
}

void EnemyBear::InitSpecial(void)
{
	InitActorEffectLaunch();

	enemyAi_ = std::make_unique<TutorialBearAI>(enemyState_,
		[&]() {return actorTransform_->GetBack(); },
		enemyData_.ragTimeData_);

	enemyAi_->Init();


	enemyDownTime_.SetTotalNowTime(DOWN_TIME);

	actorTransform_->InitVertexRenderer(Transform::TRANSFORM_TYPE::SKIN_MODEL);
}

bool EnemyBear::BattleStepCondition(void)
{
	return false;
}

void EnemyBear::InitActorTransform(void)
{
	LoadData::LoadDataInfo info = { "BearInfo.json", "Bear" };

	actorTransform_ = std::make_unique<Transform>(info, ResourceManager::RESOURCE_ID::ENEMY_BEAR);

	enemyData_ = LoadEnemyData::LoadedEnemyData(Application::GetInstance().GetDataPaths(), info);

	SetUpEnemyPalam();

	enemyData_.enemyNameId_ = ResourceManager::RESOURCE_ID::WILD_BEAR_NAME;
}

void EnemyBear::InitActorEffectLaunch(void)
{
	actorEffectLaunch_ = std::make_unique<EffectPlayLaunch>();

	InitDamageEffect();
}

void EnemyBear::InitActorAnimation(void)
{
	actorAnime_ = std::make_unique<AnimationController>(actorTransform_->GetModelId());

	LoadData::LoadDataInfo info = { "BearInfo.json", "IDLE" };
	DataAnimation data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::IDLE), data);

	info.dataName = "WALK";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::WALK), data);

	info.dataName = "ATTACK_1";
	data = LoadDataAnimation::LoadedDataAnimation(Application::GetInstance().GetDataPaths(), info);
	actorAnime_->Add(static_cast<int>(EnemyState::ATTACK | EnemyState::ATTACK_1), data);

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

	actorAnime_->Start(static_cast<int>(EnemyState::IDLE));
}

void EnemyBear::UpdateSpecial(void)
{
	enemyAi_->Update();

	actorAnime_->Play(static_cast<int>(enemyState_));

	Death();
	EnemyMove();
	EnemyAttack();
	Flinch();
	Down();


	if (!isTouchGround_)Gravity(enemyData_);

	enemyDownTime_.UpdateTime();

	SpecialStateAnime();

}

void EnemyBear::EnemyAttack(void)
{
	if (!(enemyState_ & EnemyState::ATTACK))
	{
		StopAttackEffect();
		myActorCollision_.isCollAttack_ = false;
		actorAttackRagTime_->InitTotalNowTime();
		return;
	}

	if (actorAttackRagTime_->GetTotalNowTime() > ENEMT_ATTACK_RAG)
	{
		actorAttackRagTime_->InitTotalNowTime();
		myActorCollision_.isCollAttack_ = true;
	}

	if (IsEndAction())
	{
		//AIを短時間思考停止
		enemyAi_->LockThinkShortTime();

		//ラグをセット
		enemyAi_->SetUpRagTime(EnemyAIBase::RAG_NAME::ATTACK_1_RAG_TIME);

		//AIのアクションの終了
		enemyAi_->EndAIAction();
	}

	if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_1)
	{
		ActorOtherLockOnRotation(enemyData_);
	}
	else if (actorAnime_->GetStepAnimTime() < enemyData_.collStepData_.attackCollStep_1 * ENEMY_ANIME_DIGIT_MIDLE)
	{
		SetUpCollSphere(ATTACK_BONE::RIGHT_HAND);
	}
}

void EnemyBear::DrawSpecial(void)
{
}