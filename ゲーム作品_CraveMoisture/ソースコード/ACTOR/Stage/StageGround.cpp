#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "StageGround.h"

StageGround::StageGround(void)
{
}

StageGround::~StageGround(void)
{
}

void StageGround::InitSpecial(void)
{
	SceneManager::GetInstance().GetCamera().lock()->
		SetUpCollModel(actorTransform_->GetModelId());

	actorTransform_->InitVertexRenderer(Transform::TRANSFORM_TYPE::NORMAL_MODEL);
}

void StageGround::Draw(void)
{
}

bool StageGround::BattleStepCondition(void)
{
	return false;
}

void StageGround::InitActorTransform(void)
{
	LoadData::LoadDataInfo info = { "StageInfo.json", "TutorialGround" };

	actorTransform_ = std::make_unique<Transform>(info, ResourceManager::RESOURCE_ID::STAGE_GROUND);

	myActorCollision_.collisionModelId = &actorTransform_->GetModelIdReference();
	myActorCollision_.collisionGravityModelId = &actorTransform_->GetModelIdReference();
}

void StageGround::UpdateSpecial(void)
{
}

void StageGround::CollisionedGravityActor(Position3DF pos)
{
}
