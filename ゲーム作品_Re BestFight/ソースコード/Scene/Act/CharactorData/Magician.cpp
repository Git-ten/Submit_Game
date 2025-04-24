#include"../../../Utility/MyUtility.h"
#include"../Common/UserInterface.h"
#include"../Common/AnimationController.h"
#include"../../Camera.h"
#include "Magician.h"

#define PLAYER_MODEL (ResourceManager::SRC::PLAYER_MAGIC)

Magician::Magician(std::weak_ptr<Camera> camera)
{
	camera_ = camera;
}

Magician::~Magician()
{
}

void Magician::Init(std::weak_ptr<ActorBase> enemy)
{
	//敵のアドレス参照
	enemy_ = enemy;

	InitTransform();
	InitAnim();

	//UI
	userInterface_ = std::make_shared<UserInterface>(HP_MAX, ATTACK_POWER, STAMINA_MAX, DEFENCE_MAX, AVOID_POWER, SKILL_DEFAULT_MAX);
	userInterface_->Init();
	userInterface_->SetMagicPower(true);

	InitEffect();
	InitSound();
}

void Magician::InitTransform(void)
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

SceneManager::CHARA_ID Magician::GetModelResource(void)
{
	return SceneManager::CHARA_ID::MAGICIAN_ID;
}

float Magician::GetAttackArea(void) const
{
	return FORWARD_ATTACK_AREA;
}

float Magician::GetAvoidPower(void) const
{
	return AVOID_POWER;
}

float Magician::GetJumpPower(void) const
{
	return JUMP_POWER;
}

float Magician::GetDefencePower(void) const
{
	return DEFENCE_MAX;
}

Color3D Magician::GetMyColor(void) const
{
	return MY_COLOR;
}

float Magician::GetSpeed() const
{
	return SPEED;
}

float Magician::GetAttackHappen(ActorBase::STATE type) const
{
	float ans = 0.0f;

	switch (type)
	{
	case ActorBase::STATE::WEEK_1:
		ans = 25.0f;
		break;
	case ActorBase::STATE::WEEK_2:
		ans = 25.0f;
		break;
	case ActorBase::STATE::WEEK_3:
		ans = 25.0f;
		break;
	case ActorBase::STATE::WEEK_4:
		ans = 40.0f;
		break;
	case ActorBase::STATE::WEEK_5:
		ans = 50.0f;
		break;
	case ActorBase::STATE::WEEK_6:
		ans = 50.0f;
		break;
	case ActorBase::STATE::STRONG_1:
		ans = 25.0f;
		break;
	case ActorBase::STATE::STRONG_2:
		ans = 25.0f;
		break;
	case ActorBase::STATE::STRONG_3:
		ans = 50.0f;
		break;
	default:
		break;
	}

	return ans;
}

bool Magician::IsUnleashSkill(void) const
{
	return ((userInterface_->GetStateAttackLevel() >= 3) &&
		(userInterface_->GetStateDefenceLevel() >= 3) &&
		(userInterface_->GetStateAvoidLevel() >= 3) &&
		(userInterface_->GetStateSkill() >= UserInterface::SKILL_MAX));
}

void Magician::SkillCamera(void)
{
	Quaternion angle = camera_.lock()->GetRot();

	if (animController_->GetStepAnimTime() < 600.0f)
	{
		angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(1.0), MyUtility::AXIS_Y));
	}

	camera_.lock()->SetRot(angle);

	//視点
	Position3DF effPos = effectsMap_.at(EffectManager::EFF::METEO)
		[INT_CAST(ActorBase::USE_EFFECT::METEO)].lock()->GetPos();

	effPos.y -= 900.0f;

	camera_.lock()->SetTarget(effPos);
	//camera_.lock()->SetTarget(enemy_.lock()->transform_->pos);

	//理想のカメラ座標
	Position3DF localPos = VAdd(transform_->pos, angle.PosAxis(FINISH_SKILL_CAMERA_POS));

	camera_.lock()->SetPos(localPos);
}

void Magician::WeekFinishCamera(void)
{
	static bool initFlag = false;

	Quaternion angle = camera_.lock()->GetRot();

	if (!initFlag && animController_->GetStepAnimTime() < 5.0f)
	{
		initFlag = true;
		angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(45.0), MyUtility::AXIS_Y));

	}
	else if(animController_->GetStepAnimTime() < 30.0f)
	{
		angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(-3.0), MyUtility::AXIS_Y));
	}
	else if (initFlag && animController_->GetStepAnimTime() > 35.0f)
	{
		initFlag = false;
	}

	camera_.lock()->SetRot(angle);

	//視点
	camera_.lock()->SetTarget(enemy_.lock()->transform_->pos);

	//理想のカメラ座標
	Position3DF localPos = VAdd(transform_->pos, angle.PosAxis(START_WEEK_CAMERA_POS));

	if (animController_->GetStepAnimTime() > 25.0f)
	{
		localPos = VAdd(transform_->pos, angle.PosAxis(FINISH_WEEK_CAMERA_POS));
		//現在のカメラ座標
		Position3DF nowPos = camera_.lock()->GetPos();

		//徐々に理想の座標に近づけていく
		MyUtility::RelativeVector(Camera::CAMERA_SPEED, localPos, nowPos);
		camera_.lock()->SetPos(nowPos);
	}
	else
	{
		camera_.lock()->SetPos(localPos);
	}
}

void Magician::StrongFinishCamera(void)
{
	static bool initFlag = false;

	Quaternion angle = camera_.lock()->GetRot();

	if (!initFlag && animController_->GetStepAnimTime() < 5.0f)
	{
		initFlag = true;
		angle = angle.Mult(Quaternion::AngleAxis(MyUtility::Deg2RadD(35.0), MyUtility::AXIS_Y));

	}
	else if (initFlag && animController_->GetStepAnimTime() > 30.0f)
	{
		initFlag = false;
	}

	camera_.lock()->SetRot(angle);

	//視点
	camera_.lock()->SetTarget(enemy_.lock()->transform_->pos);

	//理想のカメラ座標
	Position3DF localPos = VAdd(transform_->pos, angle.PosAxis(FINISH_STRONG_CAMERA_POS));

	camera_.lock()->SetPos(localPos);
}

void Magician::InitAnim(void)
{
	animController_ = std::make_shared<AnimationController>(transform_->modelId);
	animController_->ClearAnimMap();

	std::string PATH_ANIM = "Data/Model/Player/Magic/";

	animController_->Add(INT_CAST(ActorBase::STATE::IDLE), PATH_ANIM + "Idle_Γ.mv1", 20.0f, 0.0f, 30.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_1), PATH_ANIM + "Week_1_Γ.mv1", 210.0f, 0.0f, 72.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_2), PATH_ANIM + "Week_2_Γ.mv1", 210.0f, 0.0f, 72.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_3), PATH_ANIM + "Week_3_Γ.mv1", 190.0f, 0.0f, 102.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_4), PATH_ANIM + "Week_4_Γ.mv1", 60.0f, 0.0f, 82.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_5), PATH_ANIM + "Week_5_Γ.mv1", 70.0f, 0.0f, 122.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::WEEK_6), PATH_ANIM + "Week_6_Γ.mv1", 100.0f, 0.0f, 125.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STRONG_1), PATH_ANIM + "Strong_1_Γ.mv1", 100.0f, 0.0f, 75.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STRONG_2), PATH_ANIM + "Strong_2_Γ.mv1", 100.0f, 0.0f, 72.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STRONG_3), PATH_ANIM + "Strong_3_Γ.mv1", 110.0f, 0.0f, 120.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::DEFENCE), PATH_ANIM + "Defence_Γ.mv1", 50.0f, 0.0f, 52.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::JUMP), PATH_ANIM + "Jump_Γ.mv1", 86.0f, 0.0f, 50.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::MOVE), PATH_ANIM + "Move_Γ.mv1", 80.0f, 0.0f, 30.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::QUICK_MOVE), PATH_ANIM + "Move_Γ.mv1", 80.0f, 0.0f, 30.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::FALL_ATTACK), PATH_ANIM + "FallAttack_Γ.mv1", 160.0f, 0.0f, 52.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::AVOID), PATH_ANIM + "Avoid_Γ.mv1", 5.0f, 0.0f, 20.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::BACKSTEP), PATH_ANIM + "BackStep.mv1", 40.0f, 0.0f, 40.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::BACK_WARD), PATH_ANIM + "BackWard.mv1", 80.0f, 0.0f, 32.0f, true);
	animController_->Add(INT_CAST(ActorBase::STATE::STAGGER_1), PATH_ANIM + "Stagger_1.mv1", 30.0f, 0.0f, 11.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STAGGER_2), PATH_ANIM + "Stagger_2.mv1", 30.0f, 0.0f, 11.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::STAGGER_3), PATH_ANIM + "Stagger_3.mv1", 30.0f, 0.0f, 11.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::COUNTER_ATTACK), PATH_ANIM + "Week_4_Γ.mv1", 300.0f, 0.0f, 30.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::MISS), PATH_ANIM + "Miss_Γ.mv1", 50.0f, 0.0f, 60.0f, false);
	animController_->Add(INT_CAST(ActorBase::STATE::UNLEASH_SKILL), PATH_ANIM + "Skill_Γ.mv1", 120.0f, 0.0f, 980.0f, false);
}

void Magician::InitEffect(void)
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

	//ブラー
	effectsMap_.emplace(EffectManager::EFF::BLUR, emptyVec);
	effectsMap_.at(EffectManager::EFF::BLUR).push_back(effMng.Load(EffectManager::EFF::BLUR));

	//レーザーエフェクト
	effectsMap_.emplace(EffectManager::EFF::LASER_MAGIC, emptyVec);
	effectsMap_.at(EffectManager::EFF::LASER_MAGIC).push_back(effMng.Load(EffectManager::EFF::LASER_MAGIC));
	
	//レーザー(暴)エフェクト
	effectsMap_.emplace(EffectManager::EFF::LASER_VIOLENCE, emptyVec);
	effectsMap_.at(EffectManager::EFF::LASER_VIOLENCE).push_back(effMng.Load(EffectManager::EFF::LASER_VIOLENCE));
	
	//オーロラボールエフェクト
	effectsMap_.emplace(EffectManager::EFF::AURA, emptyVec);
	effectsMap_.at(EffectManager::EFF::AURA).push_back(effMng.Load(EffectManager::EFF::AURA));
	
	//ビッグバンエフェクト
	effectsMap_.emplace(EffectManager::EFF::BIG_BANG, emptyVec);
	effectsMap_.at(EffectManager::EFF::BIG_BANG).push_back(effMng.Load(EffectManager::EFF::BIG_BANG));
	
	//トルネードエフェクト
	effectsMap_.emplace(EffectManager::EFF::TRNADE, emptyVec);
	effectsMap_.at(EffectManager::EFF::TRNADE).push_back(effMng.Load(EffectManager::EFF::TRNADE));

	//ライトニング
	effectsMap_.emplace(EffectManager::EFF::LIGHTNING, emptyVec);
	effectsMap_.at(EffectManager::EFF::LIGHTNING).push_back(effMng.Load(EffectManager::EFF::LIGHTNING));
	
	//幻影
	effectsMap_.emplace(EffectManager::EFF::FHANTASM, emptyVec);
	effectsMap_.at(EffectManager::EFF::FHANTASM).push_back(effMng.Load(EffectManager::EFF::FHANTASM));
	
	//狙撃
	effectsMap_.emplace(EffectManager::EFF::SOGEKI, emptyVec);
	effectsMap_.at(EffectManager::EFF::SOGEKI).push_back(effMng.Load(EffectManager::EFF::SOGEKI));
	
	//必殺隕石
	effectsMap_.emplace(EffectManager::EFF::METEO, emptyVec);
	effectsMap_.at(EffectManager::EFF::METEO).push_back(effMng.Load(EffectManager::EFF::METEO));
	
	//魔法陣
	effectsMap_.emplace(EffectManager::EFF::MAHOUJIN, emptyVec);
	effectsMap_.at(EffectManager::EFF::MAHOUJIN).push_back(effMng.Load(EffectManager::EFF::MAHOUJIN));
	
	//ガードブレイク
	effectsMap_.emplace(EffectManager::EFF::GURD_BREAK, emptyVec);
	effectsMap_.at(EffectManager::EFF::GURD_BREAK).push_back(effMng.Load(EffectManager::EFF::GURD_BREAK));

	InitEffectState();
}

void Magician::InitEffectState(void)
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

	//ブラー
	effState.scl = VGet(300.0f, 300.0f, 300.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::BLUR, effState);

	//レーザエフェクト
	effState.scl = VGet(200.0f, 200.0f, 200.0f);
	effState.speed = 4.0f;
	effectsState_.emplace(EffectManager::EFF::LASER_MAGIC, effState);

	//レーザ(暴)エフェクト
	effState.scl = VGet(100.0f, 100.0f, 100.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::LASER_VIOLENCE, effState);

	//オーロラエフェクト
	effState.scl = VGet(100.0f, 100.0f, 100.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::AURA, effState);

	//ビッグバンエフェクト
	effState.scl = VGet(130.0f, 130.0f, 130.0f);
	effState.speed = 5.0f;
	effectsState_.emplace(EffectManager::EFF::BIG_BANG, effState);

	//トルネードエフェクト
	effState.scl = VGet(100.0f, 100.0f, 100.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::TRNADE, effState);

	//ライトニングエフェクト
	effState.scl = VGet(30.0f, 30.0f, 30.0f);
	effState.speed = 3.0f;
	effectsState_.emplace(EffectManager::EFF::LIGHTNING, effState);

	//幻影エフェクト
	effState.scl = VGet(200.0f, 200.0f, 200.0f);
	effState.speed = 4.0f;
	effectsState_.emplace(EffectManager::EFF::FHANTASM, effState);

	//狙撃エフェクト
	effState.scl = VGet(200.0f, 200.0f, 200.0f);
	effState.speed = 1.0f;
	effectsState_.emplace(EffectManager::EFF::SOGEKI, effState);

	//必殺隕石
	effState.scl = VGet(1700.0f, 1700.0f, 1700.0f);
	effState.speed = 0.25f;
	effectsState_.emplace(EffectManager::EFF::METEO, effState);

	//魔法陣
	effState.scl = VGet(5000.0f, 5000.0f, 5000.0f);
	effState.speed = 0.25f;
	effectsState_.emplace(EffectManager::EFF::MAHOUJIN, effState);

	//ガードブレイク
	effState.scl = VGet(200.0f, 200.0f, 200.0f);
	effState.speed = 0.1f;
	effectsState_.emplace(EffectManager::EFF::GURD_BREAK, effState);

}

void Magician::InitSound(void)
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
	soundsMap_.emplace(SoundManager::SOD::METEO,
		SoundManager::GetInstance().Load(SoundManager::SOD::METEO));

	//隕石ヒット音
	soundsMap_.emplace(SoundManager::SOD::METEO_HIT,
		SoundManager::GetInstance().Load(SoundManager::SOD::METEO_HIT));

	//スナイプ音
	soundsMap_.emplace(SoundManager::SOD::SNIPE,
		SoundManager::GetInstance().Load(SoundManager::SOD::SNIPE));
}