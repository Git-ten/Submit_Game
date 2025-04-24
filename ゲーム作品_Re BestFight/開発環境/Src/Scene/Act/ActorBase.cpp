#include"../Common/Physics.h"
#include"Common/UserInterface.h"
#include"Common/AnimationController.h"
#include"CharactorData/CharactorBase.h"
#include"CharactorData/Fighter.h"
#include"CharactorData/SordMan.h"
#include"CharactorData/Magician.h"
#include"Common/AI/ActorAi.h"
#include"Common/AI/NpcAi.h"
#include"Common/AI/FighterAi.h"
#include"Common/AI/SordManAi.h"
#include"Common/AI/MagicianAi.h"
#include"../Camera.h"
#include"../../Manager/SceneManager.h"
#include"../../Render/ModelMaterial.h"
#include"../../Render/ModelRender.h"
#include"../../Utility/MyUtility.h"
#include"../../Application.h"
#include"../Common/Collider.h"
#include "ActorBase.h"

ActorBase::ActorBase()
{
	collFlag_ = false;
	collObjFlag_ = false;
	damageAnimFlag_ = false;
	jumpVec_ = MyUtility::VECTOR_ZERO;
	state_ = STATE::NONE;
	stopFrame_ = 0.0f;
	swayVec_ = MyUtility::VECTOR_ZERO;
	updateFlg_ = true;
	weight_ = 0.0f;
	cpuFlag_ = false;
}

ActorBase::~ActorBase()
{
}

void ActorBase::InitCamera(void)
{
	actorCam_ = std::make_shared<Camera>();
	actorCam_->ChangeMode(Camera::MODE::FOLLOW);
	actorCam_->SetDefaultMode(Camera::MODE::FOLLOW);
	actorCam_->SetFollower(transform_, enemy_.lock()->transform_);
}

void ActorBase::SetDamageMotion(const std::function<void(void)>& func)
{
	damageAnimFlag_ = true;
	damageAnim_ = func;
}

void ActorBase::EndDamageMotion()
{
	damageAnimFlag_ = false;
	damageAnim_ = [&]() {NoneDamage();};
}

ActorBase::STATE ActorBase::GetState()const
{
	return state_;
}

bool ActorBase::IsDamageAnimFlag()const
{
	return damageAnimFlag_;
}

bool ActorBase::IsCollGroundFlag()const
{
	return collFlag_;
}

bool ActorBase::IsCollObjFlag() const
{
	return collObjFlag_;
}

Position3DF ActorBase::GetPos(void) const
{
	return transform_->pos;
}

SceneManager::CHARA_ID ActorBase::GetModelResource(void)
{
	return SceneManager::CHARA_ID::NONE;
}

void ActorBase::SetJumpVector(const VECTOR& vec)
{
	jumpVec_ = vec;
}

void ActorBase::SetSwayVector(const VECTOR& vec)
{
	swayVec_ = vec;
}

void ActorBase::SetWeight(float weight)
{
	weight_ = weight;
}

void ActorBase::SetDefaultWeight()
{
	weight_ = DEFAULT_WEIGHT;
}

void ActorBase::SetUpdateFlag(bool flg)
{
	updateFlg_ = flg;
}

void ActorBase::SetPos(const Position3DF& pos)
{
	transform_->pos = pos;
}

void ActorBase::SetStopFrame(float time)
{
	stopFrame_ = time;
}

void ActorBase::ChangeState(const STATE& state)
{
	state_ = state;
	animController_->Play(INT_CAST(state));
}

void ActorBase::PoligonVecNormInfo(VERTEX3D* ver, const VECTOR& vec)
{
	//		ver[1]
	//		*
	// 
	// *		*
	//ver[0]	ver[2]
	//


	//三角形の左下
	ver[0].pos = VGet(vec.x - 100.0f, vec.y - 100.0f, vec.z);
	ver[0].norm = VGet(0.0f, 0.0f, 1.0f);
	ver[0].dif = GetColorU8(255, 255, 255, 255);
	ver[0].spc = GetColorU8(0, 0, 0, 0);
	ver[0].u = 0.0f;
	ver[0].v = 0.0f;
	ver[0].su = 0.0f;
	ver[0].sv = 0.0f;

	//三角形の上
	ver[1].pos = VGet(vec.x, vec.y + 200.0f, vec.z);
	ver[1].norm = VGet(0.0f, 0.0f, 1.0f);
	ver[1].dif = GetColorU8(255, 255, 255, 255);
	ver[1].spc = GetColorU8(0, 0, 0, 0);
	ver[1].u = 0.0f;
	ver[1].v = 0.0f;
	ver[1].su = 0.0f;
	ver[1].sv = 0.0f;

	//三角形の右下
	ver[2].pos = VGet(vec.x + 100.0f, vec.y - 100.0f, vec.z);
	ver[2].norm = VGet(0.0f, 0.0f, 1.0f);
	ver[2].dif = GetColorU8(255, 255, 255, 255);
	ver[2].spc = GetColorU8(0, 0, 0, 0);
	ver[2].u = 0.0f;
	ver[2].v = 0.0f;
	ver[2].su = 0.0f;
	ver[2].sv = 0.0f;
}

void ActorBase::PoligonVecNormUpInfo(VERTEX3D* ver, const VECTOR& vec)
{
	//		ver[1]
	//		*
	// 
	// *		*
	//ver[0]	ver[2]
	//


	//三角形の左下
	ver[0].pos = VGet(vec.x - 200.0f, vec.y, vec.z + 200.0f);
	ver[0].norm = VGet(0.0f, 0.0f, 1.0f);
	ver[0].dif = GetColorU8(255, 255, 255, 255);
	ver[0].spc = GetColorU8(0, 0, 0, 0);
	ver[0].u = 0.0f;
	ver[0].v = 0.0f;
	ver[0].su = 0.0f;
	ver[0].sv = 0.0f;

	//三角形の上
	ver[1].pos = VGet(vec.x, vec.y, vec.z - 400.0f);
	ver[1].norm = VGet(0.0f, 0.0f, 1.0f);
	ver[1].dif = GetColorU8(255, 255, 255, 255);
	ver[1].spc = GetColorU8(0, 0, 0, 0);
	ver[1].u = 0.0f;
	ver[1].v = 0.0f;
	ver[1].su = 0.0f;
	ver[1].sv = 0.0f;

	//三角形の右下
	ver[2].pos = VGet(vec.x + 200.0f, vec.y, vec.z + 200.0f);
	ver[2].norm = VGet(0.0f, 0.0f, 1.0f);
	ver[2].dif = GetColorU8(255, 255, 255, 255);
	ver[2].spc = GetColorU8(0, 0, 0, 0);
	ver[2].u = 0.0f;
	ver[2].v = 0.0f;
	ver[2].su = 0.0f;
	ver[2].sv = 0.0f;
}

float ActorBase::GetSpeed() const
{
	return 0.0f;
}

float ActorBase::GetAttackArea() const
{
	return 0.0f;
}

float ActorBase::GetAvoidPower() const
{
	return 0.0f;
}

float ActorBase::GetJumpPower() const
{
	return 0.0f;
}

float ActorBase::GetStateDefenceMax() const
{
	return 0.0f;
}

float ActorBase::GetAttackPower(void) const
{
	return 0.0f;
}

Color3D ActorBase::GetMyColor(void) const
{
	return Color3D();
}

float ActorBase::GetAttackHappen(void) const
{
	return 0.0f;
}

bool ActorBase::GetCpuFlag(void) const
{
	return cpuFlag_;
}

int ActorBase::GetPlayerId(void) const
{
	return 0;
}

void ActorBase::DamageRightStagger()
{
	if (enemy_.lock()->GetState() != ActorBase::STATE::STAGGER_1 &&
		(enemy_.lock()->GetState() != ActorBase::STATE::DEFENCE &&
		enemy_.lock()->GetState() != ActorBase::STATE::AVOID &&
			enemy_.lock()->GetState() != ActorBase::STATE::BACKSTEP) ||
		GetState() == ActorBase::STATE::UNLEASH_SKILL)
	{
		enemy_.lock()->ChangeState(ActorBase::STATE::STAGGER_1);
		enemy_.lock()->actorCam_->ChangeDefaultMode();
		enemy_.lock()->StopAllEffect();
	}

	else if (!enemy_.lock()->animController_->IsPlayAnim() ||
		enemy_.lock()->GetState() == ActorBase::STATE::DEFENCE ||
		enemy_.lock()->GetState() == ActorBase::STATE::AVOID ||
		enemy_.lock()->GetState() == ActorBase::STATE::BACKSTEP)
	{
		//ダメージ時のモーション終了処理
		enemy_.lock()->EndDamageMotion();
	}
}

void ActorBase::DamageLeftStagger()
{
	if (enemy_.lock()->GetState() != ActorBase::STATE::STAGGER_2 &&
		(enemy_.lock()->GetState() != ActorBase::STATE::DEFENCE &&
			enemy_.lock()->GetState() != ActorBase::STATE::AVOID &&
			enemy_.lock()->GetState() != ActorBase::STATE::BACKSTEP) ||
		GetState() == ActorBase::STATE::UNLEASH_SKILL)
	{
		enemy_.lock()->ChangeState(ActorBase::STATE::STAGGER_2);
		enemy_.lock()->actorCam_->ChangeDefaultMode();
		enemy_.lock()->StopAllEffect();
	}

	else if (!enemy_.lock()->animController_->IsPlayAnim() ||
		enemy_.lock()->GetState() == ActorBase::STATE::DEFENCE ||
		enemy_.lock()->GetState() == ActorBase::STATE::AVOID ||
		enemy_.lock()->GetState() == ActorBase::STATE::BACKSTEP)
	{
		//ダメージ時のモーション終了処理
		enemy_.lock()->EndDamageMotion();
	}
}

void ActorBase::DamageBackStagger()
{
	if (enemy_.lock()->GetState() != ActorBase::STATE::STAGGER_3 &&
		(enemy_.lock()->GetState() != ActorBase::STATE::DEFENCE &&
			enemy_.lock()->GetState() != ActorBase::STATE::AVOID &&
			enemy_.lock()->GetState() != ActorBase::STATE::BACKSTEP) ||
		GetState() == ActorBase::STATE::UNLEASH_SKILL)
	{
		enemy_.lock()->ChangeState(ActorBase::STATE::STAGGER_3);
		enemy_.lock()->actorCam_->ChangeDefaultMode();
		enemy_.lock()->StopAllEffect();
	}

	else if (!enemy_.lock()->animController_->IsPlayAnim() ||
		enemy_.lock()->GetState() == ActorBase::STATE::DEFENCE ||
		enemy_.lock()->GetState() == ActorBase::STATE::AVOID ||
		enemy_.lock()->GetState() == ActorBase::STATE::BACKSTEP)
	{
		//ダメージ時のモーション終了処理
		enemy_.lock()->EndDamageMotion();
	}
}

void ActorBase::DamageBlowStagger()
{
	if (enemy_.lock()->GetState() != ActorBase::STATE::BACK_WARD &&
		(enemy_.lock()->GetState() != ActorBase::STATE::DEFENCE &&
			enemy_.lock()->GetState() != ActorBase::STATE::AVOID &&
			enemy_.lock()->GetState() != ActorBase::STATE::BACKSTEP) ||
		GetState() == ActorBase::STATE::UNLEASH_SKILL)
	{
		enemy_.lock()->ChangeState(ActorBase::STATE::BACK_WARD);
		enemy_.lock()->actorCam_->ChangeDefaultMode();
		enemy_.lock()->StopAllEffect();
	}

	else if ((enemy_.lock()->IsCollGroundFlag() && enemy_.lock()->animController_->GetStepAnimTime() > 10.0f) ||
		enemy_.lock()->GetState() == ActorBase::STATE::DEFENCE ||
		enemy_.lock()->GetState() == ActorBase::STATE::AVOID ||
		enemy_.lock()->GetState() == ActorBase::STATE::BACKSTEP)
	{
		//ダメージ時のモーション終了処理
		enemy_.lock()->EndDamageMotion();
	}
}

void ActorBase::DamageGurdBreak(void)
{
	if (GetState() != ActorBase::STATE::STAGGER_3)
	{
		ChangeState(ActorBase::STATE::STAGGER_3);
		actorCam_->ChangeDefaultMode();
	}

	else if (!animController_->IsPlayAnim())
	{
		//ダメージ時のモーション終了処理
		EndDamageMotion();
	}
}

void ActorBase::NoneDamage()
{
}

void ActorBase::PlayEffect(const EffectManager::EFF& type, const USE_EFFECT& number, const Position3DF& pos, const Rot3DF& rot)
{
	effectsMap_.at(type)[INT_CAST(number)].lock()->Play();
	effectsMap_.at(type)[INT_CAST(number)].lock()->SetPos(pos);
	effectsMap_.at(type)[INT_CAST(number)].lock()->SetScl(effectsState_.at(type).scl);
	effectsMap_.at(type)[INT_CAST(number)].lock()->SetRot(rot);
	effectsMap_.at(type)[INT_CAST(number)].lock()->SetSpeed(effectsState_.at(type).speed);
}

void ActorBase::StopEffect(const EffectManager::EFF& type, const USE_EFFECT& number)
{
	effectsMap_.at(type)[INT_CAST(number)].lock()->Stop();
}

bool ActorBase::IsPlayEffect(const EffectManager::EFF& type, const USE_EFFECT& number)
{
	return effectsMap_.at(type)[INT_CAST(number)].lock()->IsCheckPlay();
}

void ActorBase::SetEffectPos(const EffectManager::EFF& type, const USE_EFFECT& number, const Position3DF& pos)
{
	effectsMap_.at(type)[INT_CAST(number)].lock()->SetPos(pos);
}

void ActorBase::SetEffectRot(const EffectManager::EFF& type, const USE_EFFECT& number, const Rot3DF& rot)
{
	effectsMap_.at(type)[INT_CAST(number)].lock()->SetRot(rot);
}

Position3DF ActorBase::GetEffectPos(const EffectManager::EFF& type, const USE_EFFECT& number) const
{
	return effectsMap_.at(type)[INT_CAST(number)].lock()->GetPos();
}

void ActorBase::SoundPlay(const SoundManager::SOD& soundType, int playType)
{
	soundsMap_.at(soundType).lock()->Play(playType);
}

void ActorBase::SoundStop(const SoundManager::SOD& soundType)
{
	soundsMap_.at(soundType).lock()->Stop();
}

Position3DF ActorBase::GetPlayerForwardPos(const float& scl) const
{
	return Position3DF();
}

Rot3DF ActorBase::GetCollectionPlayerRot() const
{
	return Rot3DF();
}

bool ActorBase::IsUnleashSkill(void) const
{
	return true;
}

void ActorBase::SkillCamera(void)
{
}

void ActorBase::WeekFinishCamera(void)
{
}

void ActorBase::StrongFinishCamera(void)
{
}

void ActorBase::StopAllEffect(void)
{
}

CapsuleF ActorBase::Model2Capsule()
{
	CapsuleF cap = CapsuleF();

	cap.top = VScale(transform_->GetUp(), 200.0f);
	cap.top = VAdd(cap.top, transform_->pos);

	cap.down = VScale(transform_->GetDown(), 50.0f);
	cap.down = VAdd(cap.down, transform_->pos);

	cap.r = 100;

	return cap;
}

CapsuleF ActorBase::Model2Capsule(float scl, unsigned int r)
{
	CapsuleF cap = CapsuleF();

	cap.top = VScale(transform_->GetUp(), scl);
	cap.top = VAdd(cap.top, transform_->pos);

	cap.down = VScale(transform_->GetDown(), scl / 0.4f);
	cap.down = VAdd(cap.down, transform_->pos);

	cap.r = r;

	return cap;
}

SphereF ActorBase::Model2Sphere()
{
	SphereF sph = SphereF();
	sph.pos = transform_->pos;
	sph.r = 100;

	return sph;
}

SphereF ActorBase::Model2Sphere(unsigned int r)
{
	SphereF sph = SphereF();
	sph.pos = transform_->pos;
	sph.r = r;

	return sph;
}

void ActorBase::InitShader(void)
{

}

void ActorBase::InitCharactor(SceneManager::CHARA_ID id)
{
	switch(id)
	{ 
	case SceneManager::CHARA_ID::FIGHTER_ID:
		charactor_ = std::make_unique<Fighter>(camera_);
		break;
	case SceneManager::CHARA_ID::SORDMAN_ID:
		charactor_ = std::make_unique<SordMan>(camera_);
		break;
	case SceneManager::CHARA_ID::MAGICIAN_ID:
		charactor_ = std::make_unique<Magician>(camera_);
		break;
	default:
		break;
	}
}

void ActorBase::InitCharactorAi(SceneManager::CHARA_ID id)
{
	switch (id)
	{
	case SceneManager::CHARA_ID::FIGHTER_ID:
		actAi_ = std::make_shared<FighterAi>(shared_from_this());
		break;
	case SceneManager::CHARA_ID::SORDMAN_ID:
		actAi_ = std::make_shared<SordManAi>(shared_from_this());
		break;
	case SceneManager::CHARA_ID::MAGICIAN_ID:
		actAi_ = std::make_shared<MagicianAi>(shared_from_this());
		break;
	default:
		break;
	}
}

void ActorBase::MarkRotation(void)
{
	mark_->pos = VAdd(transform_->pos, VScale(transform_->GetUpLocal(), 300.0f));

	Quaternion angle;
	angle = Quaternion::AngleAxis(MyUtility::Deg2RadD(1.0), MyUtility::AXIS_Y);

	mark_->quaRotLocal = mark_->quaRotLocal.Mult(angle);
	mark_->Update();
}

void ActorBase::Gravity()
{
	//ジャンプ(上下方向)ベクトルによる移動
	//重力の影響による移動もこのベクトルで行う
	transform_->pos = VAdd(transform_->pos, jumpVec_);

	//重力処理
	Physics::Gravity(jumpVec_, weight_);
}

void ActorBase::Friction()
{
	//移動による摩擦
	transform_->pos = VAdd(transform_->pos, swayVec_);

	//摩擦処理
	Physics::Friction(swayVec_, weight_);
}

void ActorBase::UpdateStopFlame(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();
	if (stopFrame_ > 0.0f)
	{
		stopFrame_ -= delta;
	}
	else
	{
		//初期化
		stopFrame_ = 0.0f;
	}
}