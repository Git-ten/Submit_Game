#include "../../COMMON/Collider.h"
#include "../../Common/Physics.h"
#include "../APPLICATION/Application.h"
#include "../../APPLICATION/SINGLETON_MANAGER/SceneManager/SceneManager.h"
#include "../Common/DataLoad/LoadDataCameraControl.h"
#include "ActorBase.h"

ActorBase::ActorBase(void):
	actorMoveDir_(Dir3DF()),
	isTouchGround_(false),
	gravity_(VECTOR()),
	inertia_(VECTOR()),
	isSlowActor_(false),
	isBattleStart_(false),
	actorBattleStep_(0),
	isMySlow_(false)
{
}

ActorBase::~ActorBase(void)
{
}

void ActorBase::Init(void)
{
	InitActorTransform();
	InitActorAnimation();
	InitActorUi();
	InitActorEffectInfo();
	InitActorCameraControl();
	InitActorSoundLaunch();

	actorAttackRagTime_ = std::make_unique<Time>();
	actorAttackRagTime_->SetTotalNowTime(ATTACK_RAG_MAX);
}

void ActorBase::Update(void)
{
	if (actorTransform_ != nullptr)actorTransform_->PrevUpdate();

	UpdateSpecial();

	ActorCollision();

	if(actorAttackRagTime_ != nullptr)actorAttackRagTime_->UpdateTime();

	if (actorAnime_ != nullptr)actorAnime_->Update(isSlowActor_ || isMySlow_);

	if (actorTransform_ != nullptr)actorTransform_->Update();

	if (actorEffectLaunch_ != nullptr)actorEffectLaunch_->UpdateFollow();
}

void ActorBase::Draw(void)
{
	DrawSpecial();
	if (actorTransform_ != nullptr)actorTransform_->Draw();
}

void ActorBase::Release(void)
{
	if (actorEffectLaunch_ != nullptr)actorEffectLaunch_->AllStop();
}

void ActorBase::SetUpMySlow(bool isSlow)
{
	isMySlow_ = isSlow;
}

void ActorBase::SetOtherActorCollision(ACTOR_COLLISION& actorColl, const std::list<ACTOR_COLLISION*>& addActorList)
{
	otherActorCollisiton_.push_back(&actorColl);
	SetOtherActorCollisionSpecial(&actorColl);
	
	if (addActorList.size() <= 0)return;

	for (const auto& addList : addActorList)
		otherActorCollisiton_.push_back(addList);
}

ActorBase::ACTOR_COLLISION& ActorBase::GetMyCollision(void)
{
	return myActorCollision_;
}

const std::list<ActorBase::ACTOR_COLLISION*>& ActorBase::GetAddSpecialActorCollision(void)
{
	return addOtherActorCollision_;
}

void ActorBase::AddStateCameraControlData(const int state, const LoadData::LoadDataInfo& info)
{
	DataCameraControl data;
	data = LoadDataCameraControl::LoadedDataCameraControl(
		Application::GetInstance().GetDataPaths(), info);

	const auto& ite = cameraControle_.find(state);
	if (ite == cameraControle_.end())
	{
		std::vector<DataCameraControl>vec;
		cameraControle_.emplace(state, vec);
	}

	cameraControle_.at(state).push_back(data);
}

void ActorBase::SetOtherActorCollisionSpecial(ACTOR_COLLISION* actorColl)
{
}

void ActorBase::InitActorTransform(void)
{

}

void ActorBase::InitActorAnimation(void)
{
}

void ActorBase::InitActorUi(void)
{
}

void ActorBase::InitActorEffectLaunch(void)
{
}

void ActorBase::InitActorEffectInfo(void)
{
}

void ActorBase::InitActorCameraControl(void)
{
}

void ActorBase::InitActorSoundLaunch(void)
{
}

void ActorBase::InitSpecial(void)
{
}

void ActorBase::ActorCollision(void)
{
	if (actorTransform_ == nullptr)return;

	for (auto otherCollisionData : otherActorCollisiton_)
	{
		ACTOR_COLLISION* myCollisionData = &GetMyCollision();

		if (myCollisionData->collisionSphere.lock() == nullptr)return;

		if (otherCollisionData->collisionGravityModelId != nullptr &&
			myCollisionData->collisionGravity.lock() != nullptr)
		{
			// 重力用の当たり判定
			auto dimG = Collider::CollisionLine2ModelInfo(
				*otherCollisionData->collisionGravityModelId, Collider::ALL_MODEL_INFO_FRAME_IDX,
				{ myCollisionData->collisionGravity.lock()->start, myCollisionData->collisionGravity.lock()->end });
			if (dimG.HitFlag)
			{
				CollisionedGravityActor(dimG.HitPosition);
				isTouchGround_ = true;
			}
			else
				isTouchGround_ = false;
		}
		if (otherCollisionData->collisionModelId != nullptr)
		{
			// 横方向の当たり判定
			auto dimH = Collider::CollisionSphere2ModelInfo(
				*otherCollisionData->collisionModelId, Collider::ALL_MODEL_INFO_FRAME_IDX,
				{ myCollisionData->collisionSphere.lock()->pos, myCollisionData->collisionSphere.lock()->r });

			if (otherCollisionData->isMyselfEnemy && !myCollisionData->isCollHorizontal)
				dimH.HitNum = 0;

			if (dimH.HitNum > 0)
				CollisionedHorizontalActor();

			MV1CollResultPolyDimTerminate(dimH);
		}

		// 攻撃当たり判定
		if (otherCollisionData->isCollAttack_)
		{
			bool isColl = false;

			if (otherCollisionData->collisiionAttackModelId_ != nullptr)
			{
				isColl = Collider::CollisionSphere2Model(
					*otherCollisionData->collisiionAttackModelId_, Collider::ALL_MODEL_INFO_FRAME_IDX,
					{ myCollisionData->collisionSphere.lock()->pos, myCollisionData->collisionSphere.lock()->r });
				
				if (isColl)
				{
					CollisionedDamageActor(otherCollisionData->damagePower);
					otherCollisionData->isCollAttack_ = false;
				}
			}
			if (otherCollisionData->collisionAttackSphere.lock() != nullptr && 
					 myCollisionData->collisionModelId != nullptr)
			{
				isColl = Collider::CollisionSphere2Model(
					*myCollisionData->collisionModelId, Collider::ALL_MODEL_INFO_FRAME_IDX,
					{ otherCollisionData->collisionAttackSphere.lock()->pos, otherCollisionData->collisionAttackSphere.lock()->r });
				
				if (isColl)
				{
					CollisionedDamageActor(otherCollisionData->damagePower);
					otherCollisionData->isCollAttack_ = false;
				}
			}
		}
	}
}

void ActorBase::CollisionedGravityActor(Position3DF pos)
{

}

void ActorBase::CollisionedHorizontalActor(void)
{

}

void ActorBase::CollisionedDamageActor(float* damagePower)
{
}

void ActorBase::StopAttackEffect(void)
{
}

void ActorBase::StopWaveSound(void)
{
}

void ActorBase::UpdateSpecial(void)
{
}

void ActorBase::DrawSpecial(void)
{
}

void ActorBase::Gravity(ActorData& data)
{
	ActorMoveDistance(data, VNorm(gravity_), VSize(gravity_));

	//重力処理
	Physics::Gravity(gravity_);
}

void ActorBase::Inertia(ActorData& data)
{
	ActorMoveDistance(data, VNorm(inertia_), VSize(inertia_));

	//重力処理
	Physics::Inertia(inertia_);
}

void ActorBase::SetActorSlow(bool isSlow)
{
	isSlowActor_ = isSlow;
}

void ActorBase::ActorMoveVector(ActorData& data, Dir3DF vec, int addPower)
{
	// vecは正規化されている前提
	ActorMove(data, vec, static_cast<float>(data.speed_ + addPower));
}

void ActorBase::ActorMoveDistance(ActorData& data, Dir3DF vec, float dis)
{
	// vecは正規化されている前提
	if (dis <= 0.0f)return;

	ActorMove(data, vec, dis);
}

void ActorBase::ActorMoveSet(ActorData& data, Position3DF pos)
{
	VECTOR moveVec = VSub(pos , *data.pos_);
	ActorMove(data, VNorm(moveVec), VSize(moveVec));
}

void ActorBase::ActorOtherLockOnRotation(const ActorData& data, bool isRotCamera,
	bool isRotActor, bool isLerpCamera)
{
	Dir3DF dir = VNorm(GetActorOtherLockonDir(data, isRotCamera, Position3DF(), nullptr, isLerpCamera));

	if(isRotActor)
		ActorRotationDir(data, dir);
}

float ActorBase::GetDistanceActor(const ActorData& data)
{
	return VSize(GetActorOtherLockonDir(data));
}

void ActorBase::ActorRotationDir(const ActorData& data, Dir3DF dir)
{
	dir.y = 0.0f;

	if(actorTransform_ != nullptr)
		actorTransform_->RotationModelQuaRot(dir);
}

Dir3DF ActorBase::GetActorOtherLockonDir(const ActorData& data, bool isRotCamera,
	const Position3DF& pos, Position3DF* getPos, bool isLerpCamera)
{
	// 複数体にも対応可能なように、あえてこの処理方法
	for (const auto& actorColl : otherActorCollisiton_)
	{
		if (actorColl->lockOnPos != nullptr)
		{
			if(getPos != nullptr)
				*getPos = *actorColl->lockOnPos;

			VECTOR vec;
			if(!Utility::EqualsVZero(pos))
				vec = VSub(*actorColl->lockOnPos, pos);
			else
				vec = VSub(*actorColl->lockOnPos, *data.pos_);

			if (isRotCamera)
			{
				auto& camera = SceneManager::GetInstance().GetCamera();
				camera.lock()->SetLockOnTargetPos(*actorColl->lockOnPos, isLerpCamera);
			}

			return vec;
		}
	}
	return Dir3DF();
}

DataEffect ActorBase::GetEffectDataInfo(const int useNumber)
{
	const auto& ite = effectInfo_.find(useNumber);
	if (ite == effectInfo_.end())return DataEffect();

	return effectInfo_.at(useNumber);
}

const std::vector<std::shared_ptr<Image>>& ActorBase::GetImagesUi(void)
{
	return actorUi_;
}

void ActorBase::SetOtherActorSlowFunc(const std::function<void(bool)> func)
{
	otherSlowActor_.push_back(func);
}

const std::function<void(bool)> ActorBase::GetMySlowFunc(void)
{
	return [&](bool isSlow) { SetActorSlow(isSlow); };
}

void ActorBase::ActorBattleStart(void)
{
	isBattleStart_ = true;
}

void ActorBase::DrawIconRecastTime(void)
{
}

bool ActorBase::BattleStepCondition(void)
{
	return false;
}

int ActorBase::GetModelId(void)
{
	if (actorTransform_ == nullptr)return -1;

	return actorTransform_->GetModelId();
}

Position3DF ActorBase::GetPos(void)
{
	if (actorTransform_ == nullptr)return Position3DF();

	return actorTransform_->GetPos();
}

bool ActorBase::IsInertia(void)
{
	return !Utility::EqualsVZero(inertia_);
}

float ActorBase::NowHp(void)
{
	return 0.0f;
}

bool ActorBase::IsDash(void)
{
	return false;
}

bool ActorBase::IsCharge(void)
{
	return false;
}

bool ActorBase::IsTechnique(void)
{
	return false;
}

bool ActorBase::IsParysOrJustAvoid(void)
{
	return false;
}

bool ActorBase::IsSkill(void)
{
	return false;
}

bool ActorBase::IsTouchGround(void)
{
	return isTouchGround_;
}

void ActorBase::ActorMove(ActorData& data, Dir3DF dir, float dis)
{
	VECTOR moveVec = VScale(dir, dis);

	if (data.pos_ != nullptr)
	{
		data.pos_->x += moveVec.x;
		data.pos_->y += moveVec.y;
		data.pos_->z += moveVec.z;
	}

	if (data.collitionGravity_ != nullptr)
	{
		data.collitionGravity_->start.x += moveVec.x;
		data.collitionGravity_->start.y += moveVec.y;
		data.collitionGravity_->start.z += moveVec.z;
		data.collitionGravity_->end.x += moveVec.x;
		data.collitionGravity_->end.y += moveVec.y;
		data.collitionGravity_->end.z += moveVec.z;
	}

	if (data.collisionSphere_ != nullptr)
		data.collisionSphere_->pos = VAdd(*data.pos_, VScale(Utility::DIR_D, data.collisionSphereOffset_));

	if (data.collisionAttackSphere_ != nullptr)
		data.collisionAttackSphere_->pos = *data.pos_;
}