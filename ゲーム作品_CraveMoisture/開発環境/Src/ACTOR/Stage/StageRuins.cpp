#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "StageRuins.h"

StageRuins::StageRuins(void)
{
}

StageRuins::~StageRuins(void)
{
}

bool StageRuins::BattleStepCondition(void)
{
	return entrance_->IsOpenEntrance();
}

void StageRuins::SetOtherActorCollisionSpecial(ACTOR_COLLISION* actorColl)
{
	entrance_->SetOtherActorCollision(*actorColl);
}

void StageRuins::InitActorTransform(void)
{
	LoadData::LoadDataInfo info = { "StageInfo.json", "Ruins" };

	actorTransform_ = std::make_unique<Transform>(info, ResourceManager::RESOURCE_ID::STAGE_RUINS);

	myActorCollision_.collisionModelId = &actorTransform_->GetModelIdReference();
	myActorCollision_.collisionGravityModelId = &actorTransform_->GetModelIdReference();

	entrance_ = std::make_unique<EntranceObject>();
	entrance_->Init();
}

void StageRuins::InitSpecial(void)
{
	SceneManager::GetInstance().GetCamera().lock()->
		SetUpCollModel(actorTransform_->GetModelId());
	SceneManager::GetInstance().GetCamera().lock()->
		SetUpCollModel(entrance_->GetModelId());

	actorTransform_->InitVertexRenderer(Transform::TRANSFORM_TYPE::NORMAL_MODEL);

	addOtherActorCollision_.push_back(&entrance_->GetMyCollision());
}

void StageRuins::Draw(void)
{
	entrance_->Draw();
}

void StageRuins::UpdateSpecial(void)
{
	entrance_->Update();
}

void StageRuins::CollisionedGravityActor(Position3DF pos)
{
}
