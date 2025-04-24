#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "EntranceObject.h"

EntranceObject::EntranceObject(void):
	isOpen_(false)
{
}

EntranceObject::~EntranceObject(void)
{
}

void EntranceObject::OpenEntrance(void)
{
	isOpen_ = true;
	myActorCollision_.collisionModelId = nullptr;
}

void EntranceObject::CloseEntrance(void)
{
	isOpen_ = false;
	myActorCollision_.collisionModelId = &actorTransform_->GetModelIdReference();
}

bool EntranceObject::IsOpenEntrance(void)
{
	return isOpen_;
}

void EntranceObject::InitSpecial(void)
{
	SceneManager::GetInstance().GetCamera().lock()->
		SetUpCollModel(actorTransform_->GetModelId());
}

void EntranceObject::InitActorTransform(void)
{
	LoadData::LoadDataInfo info = { "StageInfo.json", "GateMist" };
	actorTransform_ = std::make_unique<Transform>(info, ResourceManager::RESOURCE_ID::GATE_RUINS);
	actorTransform_->InitVertexRenderer(Transform::TRANSFORM_TYPE::NOISE_MODEL);

	actorData_.collisionSphere_ = std::make_shared<SphereF>(actorTransform_->GetPos(), 800.0f);

	myActorCollision_.collisionModelId = &actorTransform_->GetModelIdReference();
	myActorCollision_.collisionSphere = actorData_.collisionSphere_;
}

void EntranceObject::UpdateSpecial(void)
{
	if (isOpen_)
	{
		dessolveTime_.UpdateTime();
		if (dessolveTime_.GetTotalNowTime() > 2.0f)// “ñ•bˆÈã‚½‚Â‚Æ•Â‚¶‚é
			CloseEntrance();
	}
	else
		dessolveTime_.InitTotalNowTime();

	mirageTime_.UpdateTime();

	actorTransform_->UpdateVertexPS(0,
		{ mirageTime_.GetTotalNowTime(), dessolveTime_.GetTotalNowTime(), 
		ModelRenderer::SHADER_NONE_VALUE, ModelRenderer::SHADER_NONE_VALUE });
}

void EntranceObject::DrawSpecial(void)
{
}

void EntranceObject::CollisionedHorizontalActor(void)
{
	OpenEntrance();
	actorData_.collisionSphere_ = nullptr;
}