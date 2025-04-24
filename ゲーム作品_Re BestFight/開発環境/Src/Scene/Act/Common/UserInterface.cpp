#include<DxLib.h>
#include"../../../Manager/ResourceManager.h"
#include"../../../Manager/EffectManager.h"
#include"../../../Manager/Effect.h"
#include"../../../Manager/SceneManager.h"
#include"../../../Manager/SoundManager.h"
#include"../../../Manager/Sound.h"
#include"../../../Utility/MyUtility.h"
#include"../../../Application.h"
#include "UserInterface.h"

#define IMAGE_HP ResourceManager::SRC::HP_GAGE
#define B_IMAGE_HP ResourceManager::SRC::B_HP_GAGE
#define IMAGE_STAMINA ResourceManager::SRC::STAMINA_GAGE
#define B_IMAGE_STAMINA ResourceManager::SRC::B_STAMINA_GAGE
#define IMAGE_CASE ResourceManager::SRC::CASE
#define IMAGE_LEVEL ResourceManager::SRC::LEVEL_GAGE
#define IMAGE_LEVEL_ATTACK ResourceManager::SRC::LEVEL_ATTACK_GAGE
#define B_IMAGE_LEVEL_ATTACK ResourceManager::SRC::B_LEVEL_ATTACK_GAGE
#define IMAGE_LEVEL_DEFENCE ResourceManager::SRC::LEVEL_DEFENCE_GAGE
#define B_IMAGE_LEVEL_DEFENCE ResourceManager::SRC::B_LEVEL_DEFENCE_GAGE
#define IMAGE_LEVEL_AVOID ResourceManager::SRC::LEVEL_AVOID_GAGE
#define B_IMAGE_LEVEL_AVOID ResourceManager::SRC::B_LEVEL_AVOID_GAGE
#define IMAGE_SKILL ResourceManager::SRC::SKILL_GAGE
#define B_IMAGE_SKILL ResourceManager::SRC::B_SKILL_GAGE
#define IMAGE_SKILL_CASE ResourceManager::SRC::SKILL_CASE
#define IMAGE_MAGIC ResourceManager::SRC::MAGIC_GAGE
#define B_IMAGE_MAGIC ResourceManager::SRC::B_MAGIC_GAGE
#define EFFECT_SKILL EffectManager::EFF::SKILL_MAX
#define SOUND_SKILL SoundManager::SOD::SKILL_MAX

#define RATE_PARM(a, b, c) (c * (b / a))


//�R���X�g���N�^
UserInterface::UserInterface(float hp, float attack, float stamina, float defence, float avoid, float skill)
{
	MAX_STATE stateMax = ToMaxState(hp, attack, stamina, defence, avoid, skill, LEVEL_MAX, LEVEL_MAX, LEVEL_MAX);
	MAX_EXPERIENCE exMax = ToMaxExperienceState(DEFAULT_LEVEL_MAX, DEFAULT_LEVEL_MAX, DEFAULT_LEVEL_MAX);
	EXPERIENCE ex = ToExperienceState(0.0f, 0.0f, 0.0f, exMax);
	state_ = ToState(hp, attack, stamina, 0.0f, avoid, 0.0f, stateMax, 1, 1, 1, ex);
}

//�f�X�g���N�^
UserInterface::~UserInterface()
{

}

void UserInterface::Init()
{
	//�X�e�[�^�X�\���摜
	if (!Application::GetInstance().IsWidowMode())
	{
		imgHp_ = ResourceManager::GetInstance().Load(B_IMAGE_HP).lock()->handleId_;//HP�Q�[�W
		imgStamina_ = ResourceManager::GetInstance().Load(B_IMAGE_STAMINA).lock()->handleId_;//�X�^�~�i�Q�[�W
		imgMagic_ = ResourceManager::GetInstance().Load(B_IMAGE_MAGIC).lock()->handleId_;//���̓Q�[�W
		imgSkill_ = ResourceManager::GetInstance().Load(B_IMAGE_SKILL).lock()->handleId_;//�K�E�Z�Q�[�W
		imgLevelAttack_ = ResourceManager::GetInstance().Load(B_IMAGE_LEVEL_ATTACK).lock()->handleId_;//�U���̓��x���Q�[�W
		imgLevelDefence_ = ResourceManager::GetInstance().Load(B_IMAGE_LEVEL_DEFENCE).lock()->handleId_;//�h��̓��x���Q�[�W
		imgLevelAvoid_ = ResourceManager::GetInstance().Load(B_IMAGE_LEVEL_AVOID).lock()->handleId_;//������x���Q�[�W
	}
	else
	{
		imgHp_ = ResourceManager::GetInstance().Load(IMAGE_HP).lock()->handleId_;//HP�Q�[�W
		imgStamina_ = ResourceManager::GetInstance().Load(IMAGE_STAMINA).lock()->handleId_;//�X�^�~�i�Q�[�W
		imgMagic_ = ResourceManager::GetInstance().Load(IMAGE_MAGIC).lock()->handleId_;//������x���Q�[�W
		imgSkill_ = ResourceManager::GetInstance().Load(IMAGE_SKILL).lock()->handleId_;//�K�E�Z�Q�[�W
		imgLevelAttack_ = ResourceManager::GetInstance().Load(IMAGE_LEVEL_ATTACK).lock()->handleId_;//�U���̓��x���Q�[�W
		imgLevelDefence_ = ResourceManager::GetInstance().Load(IMAGE_LEVEL_DEFENCE).lock()->handleId_;//�h��̓��x���Q�[�W
		imgLevelAvoid_ = ResourceManager::GetInstance().Load(IMAGE_LEVEL_AVOID).lock()->handleId_;//������x���Q�[�W
	}
	
	imgCase_ = ResourceManager::GetInstance().Load(IMAGE_CASE).lock()->handleId_;//�P�[�X
	imgSkillCase_ = ResourceManager::GetInstance().Load(IMAGE_SKILL_CASE).lock()->handleId_;//�K�E�Z�P�[�X
	imgLevel_ = ResourceManager::GetInstance().Load(IMAGE_LEVEL).lock()->handleId_;//���x���Q�[�W


	//�_���[�W��
	damagePower_ = 0.0f;
	damageColorTime_ = -1.0f;

	//�\�����]�t���O
	mirrorFlag_ = false;

	skillEff_ = EffectManager::GetInstance().Load(EFFECT_SKILL);
	skillSod_ = SoundManager::GetInstance().Load(SOUND_SKILL);

	//�c�薂��
	magicPower_ = MAX_MAGIC;
	//���̓Q�[�W���������ǂ���
	magicFlag_ = false;
}

void UserInterface::Update()
{
	//�_���[�W�v�Z
	//���́A�P���Ƀ_���[�W�ʂ���h��͂������Ă��邾��
	float damage = damagePower_ - state_.defence;
	if(damage > 0)damageColorTime_ = 0.2f;
	else if (damage <= 0.0f)
	{
		damage = 0.0f;
	}
	SubHp(damage);

	//���x���A�b�v����
	LevelUp();

	//�X�e�[�^�X�p�����[�^�̐���
	LimitState();

	//�󂯂��_���[�W�̗ʂ͏�����
	damagePower_ = 0.0f;

	float delta = SceneManager::GetInstance().GetDeltaTime();
	damageColorTime_ -= delta;
	if (damageColorTime_ <= 0.0f)
	{
		damageColorTime_ = -1.0f;
	}
}

void UserInterface::Draw()
{
	//HP�Q�[�W�̕`��
	DrawHp();
	//�X�^�~�i�Q�[�W�̕`��
	DrawStamina();
	
	//���̓Q�[�W
	if (magicFlag_)
	{
		DrawMagicPower();
	}

	//���x���Q�[�W�̕`��
	DrawLevel();
	//�X�L���Q�[�W�̕`��
	DrawSkill();
}

float UserInterface::GetStateStamina() const
{
	return state_.stamina;
}

float UserInterface::GetStateHp() const
{
	return state_.hp;
}

float UserInterface::GetStateAttackPower(void) const
{
	return state_.attack;
}

float UserInterface::GetStateAvoidPower(void) const
{
	return state_.avoid;
}

float UserInterface::GetStateDefenceMax(void) const
{
	return state_.max.defenceMax;
}

//�X�^�~�i�`���[�W
void UserInterface::ChargeStateStamina(float stamina)
{
	state_.stamina += stamina;
}

//�K�E�Z�`���[�W
void UserInterface::ChargeStateSkill(float skill)
{
	state_.skill += skill;
}

void UserInterface::ChargeStateAttackEx(float ex)
{
	state_.ex.attackEx += ex;
}

void UserInterface::ChargeStateDefenceEx(float ex)
{
	state_.ex.defenceEx += ex;
}

void UserInterface::ChargeStateAvoidEx(float ex)
{
	state_.ex.avoidEx += ex;
}

float UserInterface::GetStateSkill()const
{
	return state_.skill;
}

int UserInterface::GetStateAttackLevel(void) const
{
	return state_.attackLevel;
}

int UserInterface::GetStateDefenceLevel(void) const
{
	return state_.defenceLevel;
}

int UserInterface::GetStateAvoidLevel(void) const
{
	return state_.avoidLevel;
}

float UserInterface::GetStateMaxStamina() const
{
	return state_.max.staminaMax;
}

float UserInterface::GetStateMaxHp() const
{
	return state_.max.hpMax;
}

//�X�e�[�^�X�p�����[�^�̑���
void UserInterface::SubHp(float sub)
{
	state_.hp -= sub;
}
void UserInterface::SubStamina(float sub)
{
	state_.stamina -= sub;
}

void UserInterface::AddStamina(float add)
{
	state_.stamina += add;
}

void UserInterface::AddSkill(float add)
{
	state_.skill += add;
}

void UserInterface::SubDefence(float sub)
{
	state_.defence -= sub;
}

void UserInterface::AddDefence(float add)
{
	state_.defence += add;
}

void UserInterface::SetDefence(float value)
{
	state_.defence = value;
}

void UserInterface::AddDamagePower(float add)
{
	damagePower_ += add;
}

void UserInterface::SetDamagePower(float power)
{
	damagePower_ = power;
}

void UserInterface::InitSkillandLevel(void)
{
	state_.skill = 0.0f;
	state_.attackLevel = 1;
	state_.defenceLevel = 1;
	state_.avoidLevel = 1;
}

void UserInterface::SetMirrorFlag(bool flg)
{
	mirrorFlag_ = flg;
}

bool UserInterface::GetMirrorFlag(void) const
{
	return mirrorFlag_;
}

//�ϊ�
UserInterface::STATE UserInterface::ToState(float hp, float attack, float stamina, float defence, float avoid, float skill,
	const MAX_STATE& max, int attackLev, int defenceLev, int avoidLev, const EXPERIENCE& ex)const
{
	STATE ans = { hp, attack, stamina, defence, avoid, skill, max, attackLev, defenceLev, avoidLev, ex};
	return ans;
}

UserInterface::MAX_STATE UserInterface::ToMaxState(float hp, float attack, float stamina, float defence, float avoid, float skill,
	int attackLev, int defenceLev, int avoidLev)const
{
	MAX_STATE ans = { hp, attack, stamina, defence, avoid, skill, attackLev, defenceLev, avoidLev};
	return ans;
}

UserInterface::EXPERIENCE UserInterface::ToExperienceState(float ex_attack, float ex_defence, float ex_avoid, const MAX_EXPERIENCE& max)
{
	EXPERIENCE ans = {ex_attack, ex_defence, ex_avoid, max};
	return ans;
}

UserInterface::MAX_EXPERIENCE UserInterface::ToMaxExperienceState(float ex_attack, float ex_defence, float ex_avoid)
{
	MAX_EXPERIENCE ans = { ex_attack, ex_defence, ex_avoid };
	return ans;
}

//�X�e�[�^�X�p�����[�^�̏���E��������
void UserInterface::LimitState(void)
{
	//HP�̏���E����
	if (state_.hp > state_.max.hpMax)
	{
		state_.hp = state_.max.hpMax;
	}
	if (state_.hp < 0.0f)
	{
		state_.hp = 0.0f;
	}

	//�X�^�~�i�̏���E����
	if (state_.stamina > state_.max.staminaMax)
	{
		state_.stamina = state_.max.staminaMax;
	}
	if (state_.stamina < 0.0f)
	{
		state_.stamina = 0.0f;
	}

	//�X�L���Q�[�W�̏���E����
	if (state_.skill > state_.max.skillMax)
	{
		state_.skill = state_.max.skillMax;
	}
	if (state_.skill < 0.0f)
	{
		state_.skill = 0.0f;
	}

	//�U���̓��x���o���l�̏���E����
	if (state_.ex.attackEx > state_.ex.max.attackExMax)
	{
		state_.ex.attackEx = state_.ex.max.attackExMax;
	}
	if (state_.ex.attackEx < 0.0f)
	{
		state_.ex.attackEx = 0.0f;
	}

	//�h��̓��x���o���l�̏���E����
	if (state_.ex.defenceEx > state_.ex.max.defenceExMax)
	{
		state_.ex.defenceEx = state_.ex.max.defenceExMax;
	}
	if (state_.ex.defenceEx < 0.0f)
	{
		state_.ex.defenceEx = 0.0f;
	}

	//���̓��x���o���l�̏���E����
	if (state_.ex.avoidEx > state_.ex.max.avoidExMax)
	{
		state_.ex.avoidEx = state_.ex.max.avoidExMax;
	}
	if (state_.ex.avoidEx < 0.0f)
	{
		state_.ex.avoidEx = 0.0f;
	}
	
	//���͗ʂ̏���E����
	if (magicPower_ > MAX_MAGIC)
	{
		magicPower_ = MAX_MAGIC;
	}
	if (state_.ex.avoidEx < 0.0f)
	{
		magicPower_ = 0.0f;
	}
}

void UserInterface::LevelUp()
{
	//�U�����x��������ɒB����
	if (state_.attackLevel >= LEVEL_MAX)
	{
		state_.attackLevel = LEVEL_MAX;
		state_.ex.attackEx = state_.ex.max.attackExMax;
	}
	//�U�����x���A�b�v
	else if (state_.ex.attackEx >= state_.ex.max.attackExMax)
	{
		state_.ex.attackEx = 0.0f;
		state_.ex.max.attackExMax += LEVEL_EX_MAX_ADD;
		state_.attackLevel++;
		//�K�E�Z�Q�[�W�������グ��
		state_.skill += 10.0f;
	}

	//�h�䃌�x��������ɒB����
	if (state_.defenceLevel >= LEVEL_MAX)
	{
		state_.attackLevel = LEVEL_MAX;
		state_.ex.defenceEx = state_.ex.max.defenceExMax;
	}
	//�h��̓��x���A�b�v
	else if (state_.ex.defenceEx >= state_.ex.max.defenceExMax)
	{
		state_.ex.defenceEx = 0.0f;
		state_.ex.max.defenceExMax += LEVEL_EX_MAX_ADD;
		state_.defenceLevel++;
		//�K�E�Z�Q�[�W�������グ��
		state_.skill += 10.0f;
	}

	//���̓��x��������ɒB����
	if (state_.avoidLevel >= LEVEL_MAX)
	{
		state_.avoidLevel = LEVEL_MAX;
		state_.ex.avoidEx = state_.ex.max.avoidExMax;
	}
	//���̓��x���A�b�v
	else if (state_.ex.avoidEx >= state_.ex.max.avoidExMax)
	{
		state_.ex.avoidEx = 0.0f;
		state_.ex.max.avoidExMax += LEVEL_EX_MAX_ADD;
		state_.avoidLevel++;
		//�K�E�Z�Q�[�W�������グ��
		state_.skill += 10.0f;
	}
}

void UserInterface::SkillMaxStartEff(void)
{
	//�J�n�������ǂ���
	static bool startSodFlg = false;

	//�X�L���Q�[�W�̏���E����
	if (state_.skill >= state_.max.skillMax)
	{
		if (!skillEff_.lock()->IsCheckPlay())
		{
			skillEff_.lock()->Play();
			if (!startSodFlg)
			{
				startSodFlg = true;
				skillSod_.lock()->Play(DX_PLAYTYPE_BACK);
			}

			if (!mirrorFlag_)
			{
				Size2D caseSize = { 256, 90 };
				Position2D pos = { SKILL_GAGE_XPOS + 20, SKILL_GAGE_YPOS + 20 };
				float scl = 15;
				if (!Application::GetInstance().IsWidowMode())
				{
					caseSize.x *= 2;
					caseSize.y *= 2;
					pos.x *= 2;
					pos.y += 220;
					scl *= 2.0f;
				}

				skillEff_.lock()->SetPos({ static_cast<float>(pos.x),
					static_cast<float>(pos.y), 0.0f });
				skillEff_.lock()->SetScl({ scl, scl, scl });
			}
			else
			{
				Size2D screenSize = Application::GetInstance().GetScreenSize();
				Size2D caseSize = { 256, 90 };
				Position2D pos = { screenSize.x - SKILL_GAGE_XPOS * 3 + 10, SKILL_GAGE_YPOS + 20 };
				float scl = 15;
				if (!Application::GetInstance().IsWidowMode())
				{
					caseSize.x *= 2;
					caseSize.y *= 2;
					pos.x -= 350;
					pos.y += 220;
					scl *= 2.0f;
				}

				skillEff_.lock()->SetPos({ static_cast<float>(pos.x),
					static_cast<float>(pos.y), 0.0f });
				skillEff_.lock()->SetScl({ scl, scl, scl });
			}
		}
	}
	else
	{
		startSodFlg = false;
	}
}

void UserInterface::SkillMaxEndEff(void)
{
	//�J�n�������ǂ���
	static bool startFlg = false;

	//�X�L���Q�[�W�̏���E����
	if (state_.skill <= 0.0f)
	{
		if (!startFlg && skillEff_.lock()->IsCheckPlay())
		{
			skillEff_.lock()->Stop();
			startFlg = true;
		}
		else if (skillEff_.lock()->IsCheckPlay())
		{
			startFlg = false;
		}
	}
}

void UserInterface::SetMagicPower(bool flg)
{
	magicFlag_ = flg;
}

void UserInterface::AddMagicPower(float add)
{
	magicPower_ += add;
}

void UserInterface::SubMagicPower(float sub)
{
	magicPower_ -= sub;
}

float UserInterface::GetMagicPower(void) const
{
	return magicPower_;
}

void UserInterface::DrawHp()
{
	//���݂�HP���ő�HP�̂������p�[�Z���g���v�Z
	int xx, yy, rate;
	xx = yy = rate = -1;
	float denominator = state_.max.hpMax;
	float numerator = state_.hp;
	PercentGageX(xx, yy, rate, denominator, numerator, imgHp_);

	Size2D caseSize = {320, 32};
	if (!Application::GetInstance().IsWidowMode())
	{
		caseSize.x *= 2;
		caseSize.y *= 2;
	}

	static Position2D collecPos = Position2D();
	if (!mirrorFlag_)
	{
		if (damageColorTime_ > 0.0f)
		{
			//�_���[�W���󂯂��ꍇHP�Q�[�W�̐F��ύX
			SetDrawBright(0, 255, 120);
			collecPos.y = 10;
		}
		else
		{
			collecPos.y = 0;
		}
		//HP�{��
		DrawRectGraph(HP_GAGE_XPOS, HP_GAGE_YPOS + collecPos.y, 0, 0, rate, yy, imgHp_, true);
		SetDrawBright(255, 255, 255);
		//HP�P�[�X
		DrawExtendGraph(HP_GAGE_XPOS, HP_GAGE_YPOS + collecPos.y,
			HP_GAGE_XPOS + caseSize.x, HP_GAGE_YPOS + caseSize.y + collecPos.y, imgCase_, true);
	}
	else
	{
		if (damageColorTime_ > 0.0f)
		{
			//�_���[�W���󂯂��ꍇHP�Q�[�W�̐F��ύX
			SetDrawBright(0, 255, 120);
			collecPos.y = 10;
		}
		else
		{
			collecPos.y = 0;
		}
		Size2D size = Application::GetInstance().GetScreenSize();
		Position2D mirrorPos = { size.x - caseSize.x - HP_GAGE_XPOS , HP_GAGE_YPOS };

		//HP�{��
		DrawRectGraph(size.x - caseSize.x - HP_GAGE_XPOS, HP_GAGE_YPOS + collecPos.y, xx - rate, 0, xx, yy, imgHp_, true, true);
		SetDrawBright(255, 255, 255);
		//HP�P�[�X
		DrawExtendGraph(mirrorPos.x, mirrorPos.y + collecPos.y,
			mirrorPos.x + caseSize.x, mirrorPos.y + caseSize.y + collecPos.y, imgCase_, true);
	}
}

void UserInterface::DrawStamina()
{
	//���݂�HP���ő�X�^�~�i�̂������p�[�Z���g���v�Z
	//�߂�l��std::tuple<xx,yy,rate>
	int xx, yy, rate;
	xx = yy = rate = -1;
	float denominator = state_.max.staminaMax;
	float numerator = state_.stamina;
	PercentGageX(xx, yy, rate, denominator, numerator, imgStamina_);

	Size2D caseSize = { 320, 32 };
	if (!Application::GetInstance().IsWidowMode())
	{
		caseSize.x *= 2;
		caseSize.y *= 2;
	}

	if (!mirrorFlag_)
	{
		//�������X�^�~�i�Q�[�W��`��
		DrawRectGraph(STAMINA_GAGE_XPOS, caseSize.y + 15, 0, 0, rate, yy, imgStamina_, true);
		//�X�^�~�i�P�[�X
		DrawExtendGraph(STAMINA_GAGE_XPOS, caseSize.y + 15,
			STAMINA_GAGE_XPOS + caseSize.x, caseSize.y + 15 + caseSize.y, imgCase_, true);
	}
	else
	{
		Size2D size = Application::GetInstance().GetScreenSize();
		Position2D mirrorPos = { size.x - caseSize.x - STAMINA_GAGE_XPOS,  caseSize.y + 15 };

		//�������X�^�~�i�Q�[�W��`��
		DrawRectGraph(mirrorPos.x, mirrorPos.y, caseSize.x - rate, 0, caseSize.x, caseSize.y, imgStamina_, true, true);
		//�X�^�~�i�P�[�X
		DrawExtendGraph(mirrorPos.x, mirrorPos.y,
			mirrorPos.x + caseSize.x, mirrorPos.y + caseSize.y, imgCase_, true);
	}
}

void UserInterface::DrawSkill()
{
	//���݂�HP���ő�K�E�Z�Q�[�W�̂������p�[�Z���g���v�Z
	int xx, yy, rate;
	xx = yy = rate = -1;
	float denominator = state_.max.skillMax;
	float numerator = state_.skill;
	PercentGageX(xx,yy,rate, denominator, numerator, imgSkill_);

	Size2D caseSize = { 256, 90 };
	Position2D pos = { SKILL_GAGE_XPOS, SKILL_GAGE_YPOS };
	if (!Application::GetInstance().IsWidowMode())
	{
		caseSize.x *= 2;
		caseSize.y *= 2;
		pos.x *= 2;
		pos.y += 200;
	}

	if (!mirrorFlag_)
	{
		//�������K�E�Z�Q�[�W��`��
		DrawRectGraph(pos.x, pos.y, 0, 0, rate, caseSize.y, imgSkill_, true);
		//�K�E�Z�P�[�X
		DrawExtendGraph(pos.x, pos.y, pos.x + caseSize.x, pos.y + caseSize.y, imgSkillCase_, true);
	}
	else
	{
		Size2D size = Application::GetInstance().GetScreenSize();
		Position2D mirrorPos = {size.x - caseSize.x - pos.x, pos.y };

		//�������K�E�Z�Q�[�W��`��
		DrawRectGraph(mirrorPos.x, mirrorPos.y, 0, 0, rate, caseSize.y, imgSkill_, true);
		//�K�E�Z�P�[�X
		DrawExtendGraph(mirrorPos.x, mirrorPos.y, mirrorPos.x + caseSize.x, mirrorPos.y + caseSize.y, imgSkillCase_, true);
	}
}

void UserInterface::DrawLevel()
{
	int fontSize = 32;
	if (!Application::GetInstance().IsWidowMode())fontSize *= 2;

	SetFontSize(fontSize);
	DrawLevelAttack();
	DrawLevelDefence();
	DrawLevelAvoid();
	SetFontSize(16);


	Size2D caseSize = { 80, 80 };
	Position2D pos = { LEVEL_GAGE_XPOS, LEVEL_GAGE_YPOS };
	if (!Application::GetInstance().IsWidowMode())
	{
		caseSize.x *= 2;
		caseSize.y *= 2;
		pos.x *= 2;
		pos.y += 100;
	}

	if (!mirrorFlag_)
	{
		DrawExtendGraph(pos.x, pos.y, pos.x + caseSize.x, pos.y + caseSize.y, imgLevel_, true);
	}
	else
	{
		Size2D size = Application::GetInstance().GetScreenSize();
		Position2D mirrorPos = { pos.x + size.x - caseSize.x * 3, pos.y};

		DrawExtendGraph(mirrorPos.x, mirrorPos.y,
			mirrorPos.x + caseSize.x, mirrorPos.y + caseSize.y, imgLevel_, true);
	}
}

void UserInterface::DrawLevelAttack(void)
{
	//���݂�HP���ő�K�E�Z�Q�[�W�̂������p�[�Z���g���v�Z
	int xx, yy, rate;
	xx = yy = rate = -1;
	float denominator = state_.ex.max.attackExMax;
	float numerator = state_.ex.attackEx;
	PercentGageY(xx, yy, rate, denominator, numerator, imgLevelAttack_);

	Size2D caseSize = { 26, 41 };
	Position2D pos = { LEVEL_GAGE_XPOS + caseSize.x + 1, LEVEL_GAGE_YPOS + caseSize.y + 3 };
	Size2D shiftPos = {6, -75};

	if (!Application::GetInstance().IsWidowMode())
	{
		caseSize.x *= 2;
		caseSize.y *= 2;
		pos.x += 100;
		pos.y += 150;
		shiftPos.y -= 80;
	}

	if (!mirrorFlag_)
	{
		//�������U�����x���Q�[�W��`��
		DrawRectGraph(pos.x, pos.y - rate, 0, caseSize.y - rate, caseSize.x, pos.y, imgLevelAttack_, true);
		DrawFormatString(pos.x + shiftPos.x, pos.y + shiftPos.y, 0xff0000, "%d", state_.attackLevel);
	}
	else
	{
		Size2D size = Application::GetInstance().GetScreenSize();
		Position2D mirrorPos = { pos.x + size.x - caseSize.x * 9 - 6, pos.y };

		if (!Application::GetInstance().IsWidowMode())
		{
			mirrorPos.x -= 10;
		}

		//�������U�����x���Q�[�W��`��
		DrawRectGraph(mirrorPos.x, mirrorPos.y - rate, 0, caseSize.y - rate, caseSize.x, mirrorPos.y, imgLevelAttack_, true);
		DrawFormatString(mirrorPos.x + shiftPos.x, mirrorPos.y + shiftPos.y, 0xff0000, "%d", state_.attackLevel);
	}
}

void UserInterface::DrawLevelDefence(void)
{
	//���݂�HP���ő�K�E�Z�Q�[�W�̂������p�[�Z���g���v�Z
	int xx, yy, rate;
	xx = yy = rate = -1;
	float denominator = state_.ex.max.defenceExMax;
	float numerator = state_.ex.defenceEx;
	PercentGageY(xx, yy, rate, denominator, numerator, imgLevelDefence_);

	Size2D caseSize = { 36, 31 };
	Position2D pos = { LEVEL_GAGE_XPOS + 3, LEVEL_GAGE_YPOS + caseSize.y + 15};
	Size2D shiftPos = { -26, 0 };

	if (!Application::GetInstance().IsWidowMode())
	{
		caseSize.x *= 2;
		caseSize.y *= 2;
		pos.x += 73;
		pos.y += 146;
	}


	if (!mirrorFlag_)
	{
		//�������h��̓��x���Q�[�W��`��
		DrawRectGraph(pos.x, pos.y, 0, 0, caseSize.x, rate, imgLevelDefence_, true);
		DrawFormatString(pos.x + shiftPos.x, pos.y + shiftPos.y, 0x00bfff, "%d", state_.defenceLevel);
	}
	else
	{
		Size2D size = Application::GetInstance().GetScreenSize();
		Position2D mirrorPos = { pos.x + size.x - caseSize.x * 7 + 12, pos.y };

		if (!Application::GetInstance().IsWidowMode())
		{
			mirrorPos.x += 10;
		}

		//�������h��̓��x���Q�[�W��`��
		DrawRectGraph(mirrorPos.x, mirrorPos.y, 0, 0, caseSize.x, rate, imgLevelDefence_, true);
		DrawFormatString(mirrorPos.x + shiftPos.x, mirrorPos.y + shiftPos.y, 0x00bfff, "%d", state_.defenceLevel);
	}
}

void UserInterface::DrawLevelAvoid(void)
{
	//���݂�HP���ő�K�E�Z�Q�[�W�̂������p�[�Z���g���v�Z
	int xx, yy, rate;
	xx = yy = rate = -1;
	float denominator = state_.ex.max.avoidExMax;
	float numerator = state_.ex.avoidEx;
	PercentGageY(xx, yy, rate, denominator, numerator, imgLevelAvoid_);

	Size2D caseSize = { 37, 32 };
	Position2D pos = { LEVEL_GAGE_XPOS + caseSize.x + 2, LEVEL_GAGE_YPOS + caseSize.y + 15};
	Size2D shiftPos = { 42, 0 };

	if (!Application::GetInstance().IsWidowMode())
	{
		caseSize.x *= 2;
		caseSize.y *= 2;
		pos.x += 108;
		pos.y += 140;
		shiftPos.x += 30;
	}

	if (!mirrorFlag_)
	{
		//������������x���Q�[�W��`��
		DrawRectGraph(pos.x, pos.y, 0, 0, caseSize.x, rate, imgLevelAvoid_, true);
		DrawFormatString(pos.x + shiftPos.x, pos.y + shiftPos.y, 0xffff00, "%d", state_.avoidLevel);
	}
	else
	{
		Size2D size = Application::GetInstance().GetScreenSize();
		Position2D mirrorPos = { pos.x + size.x - caseSize.x * 7 + 20, pos.y };

		if (!Application::GetInstance().IsWidowMode())
		{
			mirrorPos.x += 13;
		}

		//������������x���Q�[�W��`��
		DrawRectGraph(mirrorPos.x, mirrorPos.y, 0, 0, caseSize.x, rate, imgLevelAvoid_, true);
		DrawFormatString(mirrorPos.x + shiftPos.x, mirrorPos.y + shiftPos.y, 0xffff00, "%d", state_.avoidLevel);
	}
}

void UserInterface::DrawMagicPower(void)
{
	//���݂�HP���ő�HP�̂������p�[�Z���g���v�Z
	int xx, yy, rate;
	xx = yy = rate = -1;
	float denominator = MAX_MAGIC;
	float numerator = magicPower_;
	PercentGageX(xx, yy, rate, denominator, numerator, imgMagic_);

	Size2D caseSize = { 320, 32 };
	if (!Application::GetInstance().IsWidowMode())
	{
		caseSize.x *= 2;
		caseSize.y *= 2;
	}

	if (!mirrorFlag_)
	{
		//HP�{��
		DrawRectGraph(MAGIC_GAGE_XPOS, caseSize.y * 2 + 20, 0, 0, rate, caseSize.y, imgMagic_, true);
		SetDrawBright(255, 255, 255);
		//HP�P�[�X
		DrawExtendGraph(MAGIC_GAGE_XPOS, caseSize.y * 2 + 20,
			MAGIC_GAGE_XPOS + caseSize.x, caseSize.y * 2 + 20 + caseSize.y, imgCase_, true);
	}
	else
	{
		Size2D size = Application::GetInstance().GetScreenSize();
		Position2D mirrorPos = { size.x - xx - MAGIC_GAGE_XPOS, caseSize.y * 2 + 20 };

		//HP�{��
		DrawRectGraph(mirrorPos.x, mirrorPos.y,
			rate - caseSize.x, 0, caseSize.x, caseSize.y, imgMagic_, true);
		SetDrawBright(255, 255, 255);
		//HP�P�[�X
		DrawExtendGraph(mirrorPos.x, mirrorPos.y,
			mirrorPos.x + caseSize.x, mirrorPos.y + caseSize.y, imgCase_, true);
	}
}

void UserInterface::PercentGageX(int& x, int& y, int& rate, const float& denominator, const float& numerator, const int& img)
{
	int err = GetGraphSize(img, &x, &y);

	if (denominator == 0 ||
		numerator == 0 ||
		err == -1)
	{
		//�G���[��������
		//0���Z
		return;
	}

	//�摜�̉����̂����������v�Z
	rate = static_cast<int>(RATE_PARM(denominator, numerator, x));
}


void UserInterface::PercentGageY(int& x, int& y, int& rate, const float& denominator, const float& numerator, const int& img)
{
	int err = GetGraphSize(img, &x, &y);

	if (denominator == 0 ||
		numerator == 0 ||
		err == -1)
	{
		//�G���[��������
		//0���Z
		return;
	}

	//�摜�̉����̂����������v�Z
	rate = static_cast<int>(RATE_PARM(denominator, numerator, y));
}