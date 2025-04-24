#include"../../../Utility/MyUtility.h"
#include"../Common/UserInterface.h"
#include"../Common/AnimationController.h"
#include"../../Camera.h"
#include "SordMan.h"

#define PLAYER_MODEL (ResourceManager::SRC::PLAYER_SORD)

SordMan::SordMan(std::weak_ptr<Camera> camera)
{
	camera_ = camera;
}

SordMan::~SordMan()
{
}

void SordMan::Init(std::weak_ptr<ActorBase> enemy)
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

void SordMan::InitTransform(void)
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

SceneManager::CHARA_ID SordMan::GetModelResource(void)
{
	return SceneManager::CHARA_ID::SORDMAN_ID;
}

float SordMan::GetAttackArea(void) const
{
	return FORWARD_ATTACK_AREA;
}

float SordMan::GetAvoidPower(void) const
{
	return AVOID_POWER;
}

float SordMan::GetJumpPower(void) const
{
	return JUMP_POWER;
}

float SordMan::GetDefencePower(void) const
{
	return DEFENCE_MAX;
}

Color3D SordMan::GetMyColor(void) const
{
	return MY_COLOR;
}

float SordMan::GetSpeed() const
{
	return SPEED;
}

float SordMan::GetAttackHappen(ActorBase::STATE type) const
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
		ans = 130.0f;
		break;
	case ActorBase::STATE::WEEK_4:
		ans = 30.0f;
		break;
	case ActorBase::STATE::WEEK_5:
		ans = 25.0f;
		break;
	case ActorBase::STATE::WEEK_6:
		ans = 50.0f;
		break;
	case ActorBase::STATE::STRONG_1:
		ans = 85.0f;
		break;
	case ActorBase::STATE::STRONG_2:
		ans = 60.0f;
		break;
	case ActorBase::STATE::STRONG_3:
		ans = 30.0f;
		break;
	default:
		break;
	}

	return ans;
}

bool SordMan::IsUnleashSkill(void) const
{
	return ((userInterface_->GetStateAttackLevel() >= 2) &&
		(userInterface_->GetStateDefenceLevel() >= 1) &&
		(userInterface_->GetStateAvoidLevel() >= 2) &&
		(userInterface_->GetStateSkill() >= UserInterface::SKILL_MAX));
}

void SordMan::SkillCamera(void)
{
	static bool CFlag = false;

	Quaternion angle = camera_.lock()->GetRot();

	//角度更新
	Quaternion rot;
	if (animController_->GetStepAnimTime() < 25.0f && !CFlag)
	{
		rot = Quaternion::AngleAxis(MyUtility::Deg2RadD(85.0f), MyUtility::AXIS_Y);
		angle = angle.Mult(rot);
		CFlag = true;
	}
	else if (CFlag && animController_->GetStepAnimTime() > 25.0f)
	{
		rot = Quaternion::AngleAxis(MyUtility::Deg2RadD(45.0f), MyUtility::AXIS_Y);
		angle = angle.Mult(rot);
		CFlag = false;
	}

	camera_.lock()->SetRot(angle);

	//理想の座標
	Position3DF localPos;
	if (animController_->GetStepAnimTime() < 25.0f)
	{
		localPos = VAdd(transform_->pos, angle.PosAxis(START_SKILL_CAMERA_POS));
		//視点
		camera_.lock()->SetTarget(enemy_.lock()->transform_->pos);
	}
	else
	{
		localPos = VAdd(transform_->pos, angle.PosAxis(END_SKILL_CAMERA_POS));
		//視点
		Position3DF pos = VAdd(transform_->pos, VScale(transform_->GetBackLocal(), 400.0f));

		camera_.lock()->SetTarget(pos);
	}

	//現在のカメラ座標
	Position3DF nowPos = camera_.lock()->GetPos();

	//徐々に理想の座標に近づけていく
	MyUtility::RelativeVector(Camera::CAMERA_SPEED, localPos, nowPos);
	camera_.lock()->SetPos(localPos);
}

void SordMan::WeekFinishCamera(void)
{
	static bool angFlag = false;

	Quaternion angle = camera_.lock()->GetRot();
	
	if (!angFlag && animController_->GetStepAnimTime() < 20.0f)
	{
		angFlag = true;
		angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(15.0), MyUtility::AXIS_Y));
	}
	else if(angFlag && animController_->GetStepAnimTime() > 50.0f)
	{
		angFlag = false;
		angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(140.0), MyUtility::AXIS_Y));
	}


	camera_.lock()->SetRot(angle);

	//理想のカメラ座標
	Position3DF localPos = VAdd(transform_->pos, angle.PosAxis(FINISH_WEEK_CAMERA_POS));

	//現在のカメラ座標
	Position3DF nowPos = camera_.lock()->GetPos();

	//徐々に理想の座標に近づけていく
	camera_.lock()->SetPos(localPos);
}

void SordMan::StrongFinishCamera(void)
{
	static bool angFlag = false;

	Quaternion angle = camera_.lock()->GetRot();

	if (!angFlag && animController_->GetStepAnimTime() < 5.0f)
	{
		angFlag = true;
		angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-20.0), MyUtility::AXIS_Y));
	}
	else
	{
		angFlag = false;
	}


	camera_.lock()->SetRot(angle);

	//理想のカメラ座標
	Position3DF localPos = VAdd(transform_->pos, angle.PosAxis(FINISH_STRONG_CAMERA_POS));

	camera_.lock()->SetPos(localPos);
}

void SordMan::InitAnim(void)
{
	animController_ = std::make_shared<AnimationController>(transform_->modelId);
	animController_->ClearAnimMap();

	std::string PATH_ANIM = "Data/Model/Player/Sord/";

	animController_->Add(INT_CAST(ActorBase::STATE::IDLE), PATH_ANIM + "Idle_β.mv1", 20.0f, 0.0f, 30.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_1), PATH_ANIM + "Week_1_β.mv1", 240.0f, 0.0f, 82.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_2), PATH_ANIM + "Week_2_β.mv1", 240.0f, 0.0f, 82.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_3), PATH_ANIM + "Week_3_β.mv1", 220.0f, 0.0f, 170.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_4), PATH_ANIM + "Week_4_β.mv1", 100.0f, 0.0f, 62.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_5), PATH_ANIM + "Week_5_β.mv1", 190.0f, 0.0f, 82.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_6), PATH_ANIM + "Week_6_β.mv1", 130.0f, 0.0f, 85.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STRONG_1), PATH_ANIM + "Strong_1_β.mv1", 140.0f, 0.0f, 120.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STRONG_2), PATH_ANIM + "Strong_2_β.mv1", 160.0f, 0.0f, 92.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STRONG_3), PATH_ANIM + "Strong_3_β.mv1", 140.0f, 0.0f, 80.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::DEFENCE), PATH_ANIM + "Defence_β.mv1", 50.0f, 0.0f, 52.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::JUMP), PATH_ANIM + "Jump_β.mv1", 46.0f, 30.0f, 50.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::MOVE), PATH_ANIM + "Move_β.mv1", 80.0f, 0.0f, 30.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::QUICK_MOVE), PATH_ANIM + "Move_β.mv1", 80.0f, 0.0f, 30.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::FALL_ATTACK), PATH_ANIM + "FallAttack_β.mv1", 160.0f, 0.0f, 52.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::AVOID), PATH_ANIM + "Avoid_β.mv1", 100.0f, 0.0f, 30.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::BACKSTEP), PATH_ANIM + "BackStep.mv1", 40.0f, 0.0f, 40.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::BACK_WARD), PATH_ANIM + "BackWard.mv1", 80.0f, 0.0f, 32.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::STAGGER_1), PATH_ANIM + "Stagger_1.mv1", 30.0f, 0.0f, 11.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STAGGER_2), PATH_ANIM + "Stagger_2.mv1", 30.0f, 0.0f, 11.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STAGGER_3), PATH_ANIM + "Stagger_3.mv1", 30.0f, 0.0f, 11.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::COUNTER_ATTACK), PATH_ANIM + "Week_5_β.mv1", 300.0f, 0.0f, 30.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::UNLEASH_SKILL), PATH_ANIM + "Skill_β.mv1", 20.0f, 0.0f, 80.0f, false);
}

void SordMan::InitEffect(void)
{
	auto& effMng = EffectManager::GetInstance();
	std::vector<std::weak_ptr<Effect>>emptyVec;//空のベクトル配列
	effectsMap_.clear();

	//ジャンプエフェクト
	effectsMap_.emplace(EffectManager::EFF::JUMP, emptyVec);
	effectsMap_.at(EffectManager::EFF::JUMP).push_back(effMng.Load(EffectManager::EFF::JUMP));

	//強攻撃エフェクト×2
	effectsMap_.emplace(EffectManager::EFF::STRONG, emptyVec);
	effectsMap_.at(EffectManager::EFF::STRONG).push_back(effMng.Load(EffectManager::EFF::STRONG));
	effectsMap_.at(EffectManager::EFF::STRONG).push_back(effMng.Load(EffectManager::EFF::STRONG));

	//弱攻撃エフェクト
	effectsMap_.emplace(EffectManager::EFF::FLASH, emptyVec);
	effectsMap_.at(EffectManager::EFF::FLASH).push_back(effMng.Load(EffectManager::EFF::FLASH));

	//攻撃風
	effectsMap_.emplace(EffectManager::EFF::ATTACK_WIND, emptyVec);
	effectsMap_.at(EffectManager::EFF::ATTACK_WIND).push_back(effMng.Load(EffectManager::EFF::ATTACK_WIND));

	//斬撃エフェクト×3
	effectsMap_.emplace(EffectManager::EFF::SORD, emptyVec);
	effectsMap_.at(EffectManager::EFF::SORD).push_back(effMng.Load(EffectManager::EFF::SORD));
	effectsMap_.at(EffectManager::EFF::SORD).push_back(effMng.Load(EffectManager::EFF::SORD));
	effectsMap_.at(EffectManager::EFF::SORD).push_back(effMng.Load(EffectManager::EFF::SORD));

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

	//ソード必殺レーザ
	effectsMap_.emplace(EffectManager::EFF::SKILL_SORD, emptyVec);
	effectsMap_.at(EffectManager::EFF::SKILL_SORD).push_back(effMng.Load(EffectManager::EFF::SKILL_SORD));

	//ライトニング
	effectsMap_.emplace(EffectManager::EFF::LIGHTNING, emptyVec);
	effectsMap_.at(EffectManager::EFF::LIGHTNING).push_back(effMng.Load(EffectManager::EFF::LIGHTNING));

	//ブラー
	effectsMap_.emplace(EffectManager::EFF::BLUR, emptyVec);
	effectsMap_.at(EffectManager::EFF::BLUR).push_back(effMng.Load(EffectManager::EFF::BLUR));

	//ガードブレイク
	effectsMap_.emplace(EffectManager::EFF::GURD_BREAK, emptyVec);
	effectsMap_.at(EffectManager::EFF::GURD_BREAK).push_back(effMng.Load(EffectManager::EFF::GURD_BREAK));


	InitEffectState();
}

void SordMan::InitEffectState(void)
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

	//斬撃エフェクト
	effState.scl = VGet(90.0f, 90.0f, 90.0f);
	effState.speed = 2.0f;
	effectsState_.emplace(EffectManager::EFF::SORD, effState);

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

	//ソード必殺レーザ
	effState.scl = VGet(200.0f, 200.0f, 200.0f);
	effState.speed = 2.0f;
	effectsState_.emplace(EffectManager::EFF::SKILL_SORD, effState);

	//必殺ライトニング
	effState.scl = VGet(50.0f, 50.0f, 50.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::LIGHTNING, effState);

	//ブラー
	effState.scl = VGet(300.0f, 300.0f, 300.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::BLUR, effState);

	//ガードブレイク
	effState.scl = VGet(200.0f, 200.0f, 200.0f);
	effState.speed = 0.1f;
	effectsState_.emplace(EffectManager::EFF::GURD_BREAK, effState);

}

void SordMan::InitSound(void)
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

	//抜刀音
	soundsMap_.emplace(SoundManager::SOD::SORD_FINISH,
		SoundManager::GetInstance().Load(SoundManager::SOD::SORD_FINISH));
	
	//剣が当たった音
	soundsMap_.emplace(SoundManager::SOD::SORD_HIT,
		SoundManager::GetInstance().Load(SoundManager::SOD::SORD_HIT));
	
	//剣が当たった音
	soundsMap_.emplace(SoundManager::SOD::THUNDER_BOLT,
		SoundManager::GetInstance().Load(SoundManager::SOD::THUNDER_BOLT));

	//必殺技音
	soundsMap_.emplace(SoundManager::SOD::SKILL_THUNDER,
		SoundManager::GetInstance().Load(SoundManager::SOD::SKILL_THUNDER));
}