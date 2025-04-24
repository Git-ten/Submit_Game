#include"../../../Utility/MyUtility.h"
#include"../Common/UserInterface.h"
#include"../Common/AnimationController.h"
#include"../../Camera.h"
#include "Fighter.h"

#define PLAYER_MODEL (ResourceManager::SRC::PLAYER_FIST)

Fighter::Fighter(std::weak_ptr<Camera> camera)
{
	camera_ = camera;
}

Fighter::~Fighter()
{
}

void Fighter::Init(std::weak_ptr<ActorBase>enemy)
{
	//敵のアドレス参照
	enemy_ = enemy;

	InitTransform();
	InitAnim();

	//UI
	userInterface_ = std::make_shared<UserInterface>(HP_MAX, ATTACK_POWER, STAMINA_MAX, DEFENCE_MAX, AVOID_POWER, SKILL_DEFAULT_MAX);
	userInterface_->Init();

	InitEffect();
	InitSound();
}

void Fighter::InitTransform(void)
{
	transform_ = std::make_unique<Transform>();

	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(PLAYER_MODEL));

	transform_->scl = PLAYER_SCALE;
	transform_->rot = MyUtility::VECTOR_ZERO;
	transform_->pos = MyUtility::VECTOR_ZERO;
	transform_->quaRot = Quaternion();
	transform_->quaRotLocal = Quaternion::Euler(MyUtility::Deg2RadV(DEFAULT_ANGLE));
	transform_->def_quaRotLocal = transform_->quaRotLocal;

	transform_->Update();
}

SceneManager::CHARA_ID Fighter::GetModelResource(void)
{
	return SceneManager::CHARA_ID::FIGHTER_ID;
}

float Fighter::GetAttackArea(void) const
{
	return FORWARD_ATTACK_AREA;
}

float Fighter::GetAvoidPower(void) const
{
	return AVOID_POWER;
}

float Fighter::GetJumpPower(void) const
{
	return JUMP_POWER;
}

float Fighter::GetDefencePower(void) const
{
	return DEFENCE_MAX;
}

Color3D Fighter::GetMyColor(void) const
{
	return MY_COLOR;
}

float Fighter::GetSpeed() const
{
	return SPEED;
}

float Fighter::GetAttackHappen(ActorBase::STATE type) const
{
	float ans = 0.0f;

	switch (type)
	{
	case ActorBase::STATE::WEEK_1:
		ans = 30.0f;
		break;
	case ActorBase::STATE::WEEK_2:
		ans = 30.0f;
		break;
	case ActorBase::STATE::WEEK_3:
		ans = 30.0f;
		break;
	case ActorBase::STATE::WEEK_4:
		ans = 20.0f;
		break;
	case ActorBase::STATE::WEEK_5:
		ans = 40.0f;
		break;
	case ActorBase::STATE::WEEK_6:
		ans = 50.0f;
		break;
	case ActorBase::STATE::STRONG_1:
		ans = 30.0f;
		break;
	case ActorBase::STATE::STRONG_2:
		ans = 50.0f;
		break;
	case ActorBase::STATE::STRONG_3:
		ans = 30.0f;
		break;
	default:
		break;
	}

	return ans;
}

bool Fighter::IsUnleashSkill(void) const
{
	return ((userInterface_->GetStateAttackLevel() >= 1) &&
		(userInterface_->GetStateDefenceLevel() >= 1) &&
		(userInterface_->GetStateAvoidLevel() >= 1) &&
		(userInterface_->GetStateSkill() >= UserInterface::SKILL_MAX));
}

void Fighter::SkillCamera(void)
{
	static bool CFlag = false;

	Quaternion angle = camera_.lock()->GetRot();

	//角度更新
	Quaternion rot;
	if (animController_->GetStepAnimTime() < 35.0f && !CFlag)
	{
		rot = Quaternion::AngleAxis(MyUtility::Deg2RadD(60.0f), MyUtility::AXIS_Y);
		angle = angle.Mult(rot);
		CFlag = true;
	}
	else if (animController_->GetStepAnimTime() >= 35.0f &&
		animController_->GetStepAnimTime() < 120.0f)
	{
		angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-2.0f), MyUtility::AXIS_Y));
	}
	else if (animController_->GetStepAnimTime() >= 120.0f &&
		animController_->GetStepAnimTime() < 175.0f && CFlag)
	{
		CFlag = false;
		rot = Quaternion::AngleAxis(MyUtility::Deg2RadD(-60.0f), MyUtility::AXIS_Y);
		angle = angle.Mult(rot);
	}
	else if (animController_->GetStepAnimTime() >= 175.0f && !CFlag)
	{
		CFlag = true;
		rot = Quaternion::AngleAxis(MyUtility::Deg2RadD(-60.0f), MyUtility::AXIS_Y);
		angle = angle.Mult(rot);
	}
	else if (!animController_->IsPlayAnim())
	{
		CFlag = false;
	}

	camera_.lock()->SetRot(angle);

	//理想の座標
	Position3DF localPos;
	if (animController_->GetStepAnimTime() < 120.0f)
	{
		localPos = VAdd(transform_->pos, angle.PosAxis(START_SKILL_CAMERA_POS));
		//視点
		camera_.lock()->SetTarget(enemy_.lock()->transform_->pos);
	}
	else if (animController_->GetStepAnimTime() < 165.0f)
	{
		localPos = VAdd(transform_->pos, angle.PosAxis(END_SKILL_CAMERA_POS));
		//視点
		camera_.lock()->SetTarget(enemy_.lock()->transform_->pos);
	}
	else
	{
		localPos = VAdd(transform_->pos, angle.PosAxis(FINISH_SKILL_CAMERA_POS));
		if (animController_->GetStepAnimTime() < 185.0f)
		{
			//視点
			camera_.lock()->SetTarget(enemy_.lock()->transform_->pos);
		}
	}

	//現在のカメラ座標
	Position3DF nowPos = camera_.lock()->GetPos();

	//徐々に理想の座標に近づけていく
	MyUtility::RelativeVector(Camera::CAMERA_SPEED, localPos, nowPos);
	camera_.lock()->SetPos(localPos);
}

void Fighter::WeekFinishCamera(void)
{
	//初期化したか
	static bool initFlag = false;

	Quaternion angle = camera_.lock()->GetRot();
	
	if (!initFlag && animController_->GetStepAnimTime() <= 10.0f)
	{
		angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-15), MyUtility::AXIS_Y));
		//視点
		camera_.lock()->SetTarget(enemy_.lock()->transform_->pos);
		initFlag = true;
	}
	else if (animController_->GetStepAnimTime() > 10.0f &&
		animController_->GetStepAnimTime() <= 70.0f)
	{
		angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-1), MyUtility::AXIS_Y));
		//視点
		camera_.lock()->SetTarget(enemy_.lock()->transform_->pos);
	}
	else if (initFlag && !animController_->IsPlayAnim())
	{
		initFlag = false;
		angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-80), MyUtility::AXIS_Y));
	}

	camera_.lock()->SetRot(angle);

	//理想のカメラ座標
	Position3DF localPos = VAdd(transform_->pos, angle.PosAxis(FINISH_WEEK_CAMERA_POS));

	//現在のカメラ座標
	Position3DF nowPos = camera_.lock()->GetPos();

	//徐々に理想の座標に近づけていく
	MyUtility::RelativeVector(Camera::CAMERA_SPEED, localPos, nowPos);
	camera_.lock()->SetPos(localPos);
}

void Fighter::StrongFinishCamera(void)
{
	Quaternion angle = camera_.lock()->GetRot().Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-0.1), MyUtility::AXIS_Y));

	camera_.lock()->SetRot(angle);
	//視点
	camera_.lock()->SetTarget(enemy_.lock()->transform_->pos);

	//理想のカメラ座標
	Position3DF localPos = VAdd(transform_->pos, angle.PosAxis(FINISH_STRONG_CAMERA_POS));
	localPos = VAdd(localPos, VScale(transform_->GetForwardLocal(), animController_->GetStepAnimTime() * 2.0f));

	//現在のカメラ座標
	Position3DF nowPos = camera_.lock()->GetPos();

	//徐々に理想の座標に近づけていく
	MyUtility::RelativeVector(Camera::CAMERA_SPEED, localPos, nowPos);
	camera_.lock()->SetPos(localPos);
}

void Fighter::InitAnim(void)
{
	animController_ = std::make_shared<AnimationController>(transform_->modelId);
	animController_->ClearAnimMap();

	std::string PATH_ANIM = "Data/Model/Player/Fist/";

	animController_->Add(INT_CAST(ActorBase::STATE::IDLE), PATH_ANIM + "Idle.mv1", 20.0f, 0.0f, 30.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_1), PATH_ANIM + "Week_1.mv1", 210.0f, 0.0f, 72.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_2), PATH_ANIM + "Week_2.mv1", 210.0f, 0.0f, 72.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_3), PATH_ANIM + "Week_3.mv1", 190.0f, 0.0f, 102.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_4), PATH_ANIM + "Week_4.mv1", 190.0f, 0.0f, 82.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_5), PATH_ANIM + "Week_5.mv1", 160.0f, 0.0f, 122.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_6), PATH_ANIM + "Week_6.mv1", 100.0f, 0.0f, 85.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STRONG_1), PATH_ANIM + "Strong_1.mv1", 100.0f, 0.0f, 62.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STRONG_2), PATH_ANIM + "Strong_2.mv1", 100.0f, 0.0f, 62.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STRONG_3), PATH_ANIM + "Strong_3.mv1", 180.0f, 0.0f, 100.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::DEFENCE), PATH_ANIM + "Defence.mv1", 50.0f, 0.0f, 52.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::JUMP), PATH_ANIM + "Jump.mv1", 46.0f, 30.0f, 50.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::MOVE), PATH_ANIM + "Move.mv1", 80.0f, 0.0f, 30.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::QUICK_MOVE), PATH_ANIM + "Move.mv1", 80.0f, 0.0f, 30.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::FALL_ATTACK), PATH_ANIM + "FallAttack.mv1", 160.0f, 0.0f, 52.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::AVOID), PATH_ANIM + "Avoid.mv1", 5.0f, 0.0f, 20.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::BACKSTEP), PATH_ANIM + "BackStep.mv1", 40.0f, 0.0f, 40.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::BACK_WARD), PATH_ANIM + "BackWard.mv1", 80.0f, 0.0f, 32.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::STAGGER_1), PATH_ANIM + "Stagger_1.mv1", 30.0f, 0.0f, 11.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STAGGER_2), PATH_ANIM + "Stagger_2.mv1", 30.0f, 0.0f, 11.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STAGGER_3), PATH_ANIM + "Stagger_3.mv1", 30.0f, 0.0f, 11.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::COUNTER_ATTACK), PATH_ANIM + "CounterAttack.mv1", 300.0f, 0.0f, 30.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::UNLEASH_SKILL), PATH_ANIM + "Skill.mv1", 90.0f, 0.0f, 230.0f, false);
}

void Fighter::InitEffect(void)
{
	auto& effMng = EffectManager::GetInstance();
	std::vector<std::weak_ptr<Effect>>emptyVec;//空のベクトル配列
	effectsMap_.clear();

	//ジャンプエフェクト
	effectsMap_.emplace(EffectManager::EFF::JUMP, emptyVec);
	effectsMap_.at(EffectManager::EFF::JUMP).push_back(effMng.Load(EffectManager::EFF::JUMP));

	//強攻撃エフェクト
	effectsMap_.emplace(EffectManager::EFF::STRONG, emptyVec);
	effectsMap_.at(EffectManager::EFF::STRONG).push_back(effMng.Load(EffectManager::EFF::STRONG));
	effectsMap_.at(EffectManager::EFF::STRONG).push_back(effMng.Load(EffectManager::EFF::STRONG));

	//弱攻撃エフェクト
	effectsMap_.emplace(EffectManager::EFF::FLASH, emptyVec);
	effectsMap_.at(EffectManager::EFF::FLASH).push_back(effMng.Load(EffectManager::EFF::FLASH));

	//攻撃風
	effectsMap_.emplace(EffectManager::EFF::ATTACK_WIND, emptyVec);
	effectsMap_.at(EffectManager::EFF::ATTACK_WIND).push_back(effMng.Load(EffectManager::EFF::ATTACK_WIND));

	//落下攻撃エフェクト
	effectsMap_.emplace(EffectManager::EFF::FALL_ATTACK, emptyVec);
	effectsMap_.at(EffectManager::EFF::FALL_ATTACK).push_back(effMng.Load(EffectManager::EFF::FALL_ATTACK));

	//空振りエフェクト
	effectsMap_.emplace(EffectManager::EFF::SWING, emptyVec);
	effectsMap_.at(EffectManager::EFF::SWING).push_back(effMng.Load(EffectManager::EFF::SWING));

	//バリアエフェクト
	effectsMap_.emplace(EffectManager::EFF::BARIA, emptyVec);
	effectsMap_.at(EffectManager::EFF::BARIA).push_back(effMng.Load(EffectManager::EFF::BARIA));

	//高速移動エフェクト
	effectsMap_.emplace(EffectManager::EFF::QUICK_MOVE, emptyVec);
	effectsMap_.at(EffectManager::EFF::QUICK_MOVE).push_back(effMng.Load(EffectManager::EFF::QUICK_MOVE));

	//ジャストエフェクト
	effectsMap_.emplace(EffectManager::EFF::JUST, emptyVec);
	effectsMap_.at(EffectManager::EFF::JUST).push_back(effMng.Load(EffectManager::EFF::JUST));

	//必殺フレイム
	effectsMap_.emplace(EffectManager::EFF::SKILL_FLAME, emptyVec);
	effectsMap_.at(EffectManager::EFF::SKILL_FLAME).push_back(effMng.Load(EffectManager::EFF::SKILL_FLAME));

	//必殺フレイムレーザ
	effectsMap_.emplace(EffectManager::EFF::LASER_FIRE, emptyVec);
	effectsMap_.at(EffectManager::EFF::LASER_FIRE).push_back(effMng.Load(EffectManager::EFF::LASER_FIRE));

	//ブラー
	effectsMap_.emplace(EffectManager::EFF::BLUR, emptyVec);
	effectsMap_.at(EffectManager::EFF::BLUR).push_back(effMng.Load(EffectManager::EFF::BLUR));

	//ガードブレイク
	effectsMap_.emplace(EffectManager::EFF::GURD_BREAK, emptyVec);
	effectsMap_.at(EffectManager::EFF::GURD_BREAK).push_back(effMng.Load(EffectManager::EFF::GURD_BREAK));

	InitEffectState();
}

void Fighter::InitEffectState(void)
{
	effectsState_.clear();

	ActorBase::EFFECT_STATE effState = ActorBase::EFFECT_STATE();
	//ジャンプエフェクト
	effState.scl = VGet(50.0f, 50.0f, 50.0f);
	effState.speed = 3.0f;
	effectsState_.emplace(EffectManager::EFF::JUMP, effState);

	//強攻撃
	effState.scl = VGet(100.0f, 100.0f, 100.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::STRONG, effState);

	//弱攻撃エフェクト
	effState.scl = VGet(150.0f, 150.0f, 150.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::FLASH, effState);

	//攻撃風
	effState.scl = VGet(80.0f, 80.0f, 80.0f);
	effState.speed = 3.0f;
	effectsState_.emplace(EffectManager::EFF::ATTACK_WIND, effState);

	//落下攻撃エフェクト
	effState.scl = VGet(90.0f, 90.0f, 90.0f);
	effState.speed = 3.0f;
	effectsState_.emplace(EffectManager::EFF::FALL_ATTACK, effState);

	//空振りエフェクト
	effState.scl = VGet(100.0f, 100.0f, 100.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::SWING, effState);

	//バリアエフェクト
	effState.scl = VGet(60.0f, 60.0f, 60.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::BARIA, effState);

	//高速移動エフェクト
	effState.scl = VGet(100.0f, 100.0f, 100.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::QUICK_MOVE, effState);

	//ジャストエフェクト	
	effState.scl = VGet(50.0f, 50.0f, 50.0f);
	effState.speed = 8.0f;
	effectsState_.emplace(EffectManager::EFF::JUST, effState);

	//必殺フレイム
	effState.scl = VGet(130.0f, 130.0f, 130.0f);
	effState.speed = 2.0f;
	effectsState_.emplace(EffectManager::EFF::SKILL_FLAME, effState);

	//必殺フレイムレーザ
	effState.scl = VGet(50.0f, 50.0f, 50.0f);
	effState.speed = 3.0f;
	effectsState_.emplace(EffectManager::EFF::LASER_FIRE, effState);

	//ブラー
	effState.scl = VGet(300.0f, 300.0f, 300.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::BLUR, effState);
	
	//ガードブレイク
	effState.scl = VGet(200.0f, 200.0f, 200.0f);
	effState.speed = 0.1f;
	effectsState_.emplace(EffectManager::EFF::GURD_BREAK, effState);

}

void Fighter::InitSound(void)
{
	//弱攻撃音
	soundsMap_.emplace(SoundManager::SOD::ATTACK_CRASH,
		SoundManager::GetInstance().Load(SoundManager::SOD::ATTACK_CRASH));

	//強攻撃音
	soundsMap_.emplace(SoundManager::SOD::ATTACK_STRONG,
		SoundManager::GetInstance().Load(SoundManager::SOD::ATTACK_STRONG));

	//空振り音
	soundsMap_.emplace(SoundManager::SOD::SWING,
		SoundManager::GetInstance().Load(SoundManager::SOD::SWING));

	//ジャスト音
	soundsMap_.emplace(SoundManager::SOD::JUST,
		SoundManager::GetInstance().Load(SoundManager::SOD::JUST));

	//ガードブレイク音
	soundsMap_.emplace(SoundManager::SOD::GURD_BREAK,
		SoundManager::GetInstance().Load(SoundManager::SOD::GURD_BREAK));
	
	//高速移動音
	soundsMap_.emplace(SoundManager::SOD::QUICK_MOVE,
		SoundManager::GetInstance().Load(SoundManager::SOD::QUICK_MOVE));
	
	//不発音
	soundsMap_.emplace(SoundManager::SOD::NOT_POSSIBLE,
		SoundManager::GetInstance().Load(SoundManager::SOD::NOT_POSSIBLE));

	//必殺技音
	soundsMap_.emplace(SoundManager::SOD::SKILL_FLAME,
		SoundManager::GetInstance().Load(SoundManager::SOD::SKILL_FLAME));
}