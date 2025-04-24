#include"../Common/Physics.h"
#include"Common/UserInterface.h"
#include"Common/AnimationController.h"
#include"Common/AI/ActorAi.h"
#include"Common/AI/NpcAi.h"
#include"CharactorData/CharactorBase.h"
#include"../Camera.h"
#include"../../Application.h"
#include"../../Utility/MyUtility.h"
#include"../Common/Collider.h"
#include"../../Manager/InputManager.h"
#include"../../Render/ModelMaterial.h"
#include"../../Render/ModelRender.h"
#include "Player.h"

#define EFFECT_CARSOL EffectManager::EFF::CARSOL_EFF

Player::Player(std::weak_ptr<Camera> camera, bool flag)
{
	camera_ = camera;
	cpuFlag_ = flag;
}

Player::~Player()
{

}

void Player::Init(std::weak_ptr<ActorBase>enemy, SceneManager::CHARA_ID id)
{
	updateFlg_ = false;

	damageAnimFlag_ = false;

	attackStrFlag_ = false;
	defenceStrFlag_ = false;
	avoidStrFlag_ = false;
	LvAtCount_ = 0;
	LvDxCount_ = 0;
	LvAvCount_ = 0;
	prevLvAttack_ = 1;
	prevLvDefence_ = 1;
	prevLvAvoid_ = 1;

	//���x���A�b�v�G�t�F�N�g
	carsolAttackEff_ = EffectManager::GetInstance().Load(EFFECT_CARSOL);
	carsolDefenceEff_ = EffectManager::GetInstance().Load(EFFECT_CARSOL);
	carsolAvoidEff_ = EffectManager::GetInstance().Load(EFFECT_CARSOL);

	//�G�̃A�h���X�Q��
	enemy_ = enemy;

	//�L�����f�[�^
	InitCharactor(id);
	charactor_->Init(enemy_);

	//�g�����X�t�H�[���̏�����
	transform_ = charactor_->transform_;

	//UI
	userInterface_ = charactor_->userInterface_;

	//���_�V�F�[�_
	InitShader();

	//�f�o�b�O�p�̃A�j���[�V�����R���g���[��
	animController_ = charactor_->animController_;
	animController_->Start(INT_CAST(STATE::IDLE));

	//���x���A�b�v�}�l�[�W��
	levelUpMng_ = std::make_unique<LevelUpManager>(userInterface_);
	levelUpMng_->Init();

	//�����蔻��̏�����
	InitCollider();

	//�G�t�F�N�g�̏�����
	effectsMap_ = charactor_->effectsMap_;
	effectsState_ = std::move(charactor_->effectsState_);

	//�T�E���h�̏�����
	soundsMap_ = std::move(charactor_->soundsMap_);

	jumpVec_ = MyUtility::VECTOR_ZERO;//�W�����v�Ƃ������㉺�����ɓ����x�N�g��
	swayVec_ = MyUtility::VECTOR_ZERO;//���C�Ƃ�����荶�E�����ɓ����x�N�g��

	//�f�o�b�O�p�̃v���C���[�̏��
	state_ = ActorBase::STATE::NONE;
	ChangeState(ActorBase::STATE::IDLE);

	//�f�o�b�O�p�̓����蔻��t���O
	collFlag_ = false;
	collObjFlag_ = false;

	//�f�o�b�O�p�̗^������_���[�W���[�V����
	damageAnim_ = [&]() {NoneDamage(); };

	SetDefaultWeight();

	InitCharactorAi(id);
	actAi_->Init();
	if (!cpuFlag_)actAi_->ChangeMode(ActorAi::MODE::MANUAL);
	else		 actAi_->ChangeMode(ActorAi::MODE::AUTO);

	npcAi_ = std::make_unique<NpcAi>(actAi_, shared_from_this());
	npcAi_->Init();
}

void Player::Update()
{
	//�v���C���[�̃R���g���[���[
	SimpleInput::PL_NUMBER number = SimpleInput::PL_NUMBER::PL_1;
	if (userInterface_->GetMirrorFlag())
	{
		number = SimpleInput::PL_NUMBER::PL_2;
	}

	if (!damageAnimFlag_ && stopFrame_ <= 0.0f)
	{
		if (!cpuFlag_)InputManager::GetInstance().GetSimpleInputIns().Update(number);
		else		  npcAi_->Update();
		actAi_->Update();
	}
	else
	{
		damageAnim_();
	}

	if (state_ != ActorBase::STATE::DEFENCE && state_ != ActorBase::STATE::AVOID)
	{
		actAi_->SetDamageFlag(false);
	}

	if (updateFlg_ && stopFrame_ <= 0.0f)
	{
		//���̏d�͏���
		Gravity();

		//���C����
		Friction();

		//���̓����蔻��
		Collision();

		//�K���X�V����
		transform_->Update();

		//���x���A�b�v�}�l�[�W���̍X�V
		levelUpMng_->Update();

		//���[�U�C���^�[�t�F�C�X�̍X�V
		userInterface_->Update();

		if (charactor_->IsUnleashSkill())
		{
			//�K�E�ZMAX���̃G�t�F�N�g�N������
			userInterface_->SkillMaxStartEff();
		}
		else
		{
			//�K�E�Z�Q�[�W�g�p���̃G�t�F�N�g��~����
			userInterface_->SkillMaxEndEff();
		}

		//�A�j���[�V�����̍X�V
		animController_->Update();
	}

	MarkRotation();

	LvAtCount_--;
	LvDxCount_--;
	LvAvCount_--;
	if (LvAtCount_ < 0)LvAtCount_ = 0;
	if (LvDxCount_ < 0)LvDxCount_ = 0;
	if (LvAvCount_ < 0)LvAvCount_ = 0;

	//�K�E�Z�������̃G�t�F�N�g�̏�����
	if (state_ == STATE::UNLEASH_SKILL)
	{
		attackStrFlag_ = false;
		defenceStrFlag_ = false;
		avoidStrFlag_ = false;

		if (carsolAttackEff_.lock()->IsCheckPlay())
		{
			carsolAttackEff_.lock()->Stop();
		}
		if (carsolDefenceEff_.lock()->IsCheckPlay())
		{
			carsolDefenceEff_.lock()->Stop();
		}
		if (carsolAvoidEff_.lock()->IsCheckPlay())
		{
			carsolAvoidEff_.lock()->Stop();
		}
	}

	//�U�����x���A�b�v�G�t�F�N�g
	if (prevLvAttack_ != userInterface_->GetStateAttackLevel())
	{
		if (!carsolAttackEff_.lock()->IsCheckPlay())
		{
			carsolAttackEff_.lock()->Play();
			carsolAttackEff_.lock()->SetScl({ 110.0f, 110.0f, 110.0f });
			carsolAttackEff_.lock()->SetRGB(255, 0, 0, 255);
		}
		attackStrFlag_ = true;
		LvAtCount_ = LV_COUNT_MAX;
		prevLvAttack_ = userInterface_->GetStateAttackLevel();
	}
	carsolAttackEff_.lock()->SetPos(transform_->pos);

	//�h�䃌�x���A�b�v�G�t�F�N�g
	if (prevLvDefence_ != userInterface_->GetStateDefenceLevel())
	{
		if (!carsolDefenceEff_.lock()->IsCheckPlay())
		{
			carsolDefenceEff_.lock()->Play();
			carsolDefenceEff_.lock()->SetScl({ 110.0f, 110.0f, 110.0f });
			carsolDefenceEff_.lock()->SetRGB(0, 191, 255, 255);
		}
		defenceStrFlag_ = true;
		LvDxCount_ = LV_COUNT_MAX;
		prevLvDefence_ = userInterface_->GetStateDefenceLevel();
	}
	carsolDefenceEff_.lock()->SetPos(transform_->pos);

	//������x���A�b�v�G�t�F�N�g
	if (prevLvAvoid_ != userInterface_->GetStateAvoidLevel())
	{
		if (!carsolAvoidEff_.lock()->IsCheckPlay())
		{
			carsolAvoidEff_.lock()->Play();
			carsolAvoidEff_.lock()->SetScl({ 110.0f, 110.0f, 110.0f });
			carsolAvoidEff_.lock()->SetRGB(255, 255, 100, 255);
		}
		avoidStrFlag_ = true;
		LvAvCount_ = LV_COUNT_MAX;
		prevLvAvoid_ = userInterface_->GetStateAvoidLevel();
		
	}

	carsolAvoidEff_.lock()->SetPos(transform_->pos);

	UpdateStopFlame();

	//�J�����̋����X�V
	actorCam_->CompModeUpdate();
}

void Player::Draw()
{
	//�f�o�b�O
	//�v���C���[�̕`��
	DrawActor();

	//UI�̕`��
	userInterface_->Draw();

	DrawMark();

	if (!userInterface_->GetMirrorFlag())
	{
		DrawLevelUp();
	}
	else
	{
		DrawLevelUpMirror();
	}
}

SceneManager::CHARA_ID Player::GetModelResource(void)
{
	//return PLAYER_MODEL;
	return charactor_->GetModelResource();
}

float Player::GetAttackArea() const
{
	//return FORWARD_ATTACK_AREA;
	return charactor_->GetAttackArea();
}

float Player::GetAvoidPower() const
{
	//return AVOID_POWER;
	return userInterface_->GetStateAvoidPower() * userInterface_->GetStateAvoidLevel();
}

float Player::GetJumpPower() const
{
	//return JUMP_POWER;
	return charactor_->GetJumpPower();
}

float Player::GetStateDefenceMax() const
{
	//return DEFENCE_MAX;
	return userInterface_->GetStateDefenceMax() * userInterface_->GetStateDefenceLevel();
}

float Player::GetAttackPower(void) const
{
	return userInterface_->GetStateAttackPower() * userInterface_->GetStateAttackLevel();
}

Color3D Player::GetMyColor(void) const
{
	//return MY_COLOR;
	return charactor_->GetMyColor();
}

float Player::GetAttackHappen(void) const
{
	return charactor_->GetAttackHappen(state_);
}

int Player::GetPlayerId(void) const
{
	if (!userInterface_->GetMirrorFlag())
	{
		return DX_INPUT_PAD1;
	}
	return DX_INPUT_PAD2;
}

float Player::GetSpeed(void) const
{
	return charactor_->GetSpeed();
}

Position3DF Player::GetPlayerForwardPos(const float& scl) const
{
	//�G�t�F�N�g
	Position3DF effPos = VAdd(transform_->pos, VScale(transform_->GetForwardLocal(), scl));
	effPos = VAdd(effPos, VGet(0.0f, 100.0f, 0.0f));
	return effPos;
}

Rot3DF Player::GetCollectionPlayerRot() const
{
	Quaternion qRot = Quaternion::AngleAxis(MyUtility::Deg2RadD(180.0f), MyUtility::AXIS_Y);
	qRot = qRot.Mult(transform_->quaRotLocal);
	return Quaternion::ToEuler(qRot);
}

bool Player::IsUnleashSkill(void) const
{
	return charactor_->IsUnleashSkill();
}

void Player::SkillCamera(void)
{
	charactor_->SkillCamera();
}

void Player::WeekFinishCamera(void)
{
	if (damageAnimFlag_)
	{
		camera_.lock()->ChangeDefaultMode();
	}

	charactor_->WeekFinishCamera();
}

void Player::StrongFinishCamera(void)
{
	if (damageAnimFlag_)
	{
		camera_.lock()->ChangeDefaultMode();
	}
	charactor_->StrongFinishCamera();
}

void Player::StopAllEffect(void)
{
	//���x���A�b�v�G�t�F�N�g
	carsolAttackEff_.lock()->Stop();
	carsolDefenceEff_.lock()->Stop();
	carsolAvoidEff_.lock()->Stop();

	for (const auto& eff : effectsMap_)
	{
		for (const auto& effVec : eff.second)
		{
			if (effVec.lock()->IsCheckPlay())
			{
				effVec.lock()->Stop();
			}
		}
	}
}

void Player::InitCollider()
{
	collider_ = std::make_unique<Collider>(shared_from_this(), enemy_);
}

void Player::InitShader(void)
{
	material_ = std::make_unique<ModelMaterial>(
		"SkinMeshVS.cso", 2,
		"SkinMeshPS.cso", 5
	);

	//���_�V�F�[�_�萔�Z�b�g
	//�|�C���g���C�g
	material_->AddConstBufVS({ POINT_LIGHT_POS.x, POINT_LIGHT_POS.y, POINT_LIGHT_POS.z , POINT_LIGHT_POWER });
	material_->AddConstBufVS({ 0.0f, 0.0f, 0.0f, 0.0f });


	//�s�N�Z���V�F�[�_�萔�Z�b�g
	//�g�U��
	material_->AddConstBufPS({ 0.8f, 0.8f, 0.8f, 1.0f });
	//����
	material_->AddConstBufPS({ 0.0f, 0.0f, 0.0f, 1.0f });

	material_->AddConstBufPS({ POINT_LIGHT_POS.x, POINT_LIGHT_POS.y, POINT_LIGHT_POS.z , 0.0f });
	//���C�g�̐F
	material_->AddConstBufPS({ POINT_LIGHT_COLOR.x, POINT_LIGHT_COLOR.y, POINT_LIGHT_COLOR.z,
		0.0f });
	material_->AddConstBufPS({ POINT_LIGHT_COLOR.x, POINT_LIGHT_COLOR.y, POINT_LIGHT_COLOR.z,
		0.0f });

	render_ = std::make_unique<ModelRender>(transform_->modelId, *material_);
}

void Player::InitMark(void)
{
	mark_ = std::make_unique<Transform>();

	if (!userInterface_->GetMirrorFlag())
	{
		mark_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::MARK_1));
	}
	else
	{
		mark_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::MARK_2));
	}

	mark_->scl = MARK_SCALE;
	mark_->rot = MyUtility::VECTOR_ZERO;
	mark_->pos = VAdd(transform_->pos, VScale(transform_->GetUpLocal(), 300.0f));
	mark_->quaRot = Quaternion();
	mark_->quaRotLocal = Quaternion();
	mark_->def_quaRotLocal = mark_->quaRotLocal;

	mark_->Update();
}

void Player::Collision()
{
	//�������Ɉ��ȏ㉺��������0.0f�������邾��
	if (transform_->pos.y <= 0.0f)
	{
		transform_->pos.y = 0.0f;
		//�����蔻��t���O���I��
		collFlag_ = true;
	}
	else
	{
		//�����蔻��t���O���I�t
		collFlag_ = false;
	}

	//�G�Ƃ̓����蔻��
	if (collider_->CollisionLine2Line())
	{
		transform_->pos = VAdd(transform_->prevPos, VScale(transform_->GetBackLocal(), 10.0f));
		//���ɂ�����Ƃ������炷
		transform_->pos = VAdd(transform_->pos, VGet(10.0f, 0.0f, 0.0f));
		collFlag_ = true;
		collObjFlag_ = true;
	}
	else
	{
		collObjFlag_ = false;
	}
}

void Player::DrawActor()
{
	//MV1DrawModel(transform_->modelId);
	render_->Draw();
}

void Player::DrawMark(void)
{
	MV1DrawModel(mark_->modelId);
}

void Player::DrawLevelUp(void)
{
	int fontSize = 32;
	int shift = 0;
	int lvColorCnt = 6;
	int posY = 150;

	if (!Application::GetInstance().IsWidowMode())
	{
		fontSize = 64;
		posY += 64;
	}

	SetFontSize(fontSize);
	if (attackStrFlag_)
	{
		DrawFormatString(0, posY, 0xff0000, "Lv. :AttackUp!");
		if (LvAtCount_ % lvColorCnt)
		{
			DrawFormatString(0, posY, 0xffffff, "   %d", userInterface_->GetStateAttackLevel());
		}
		else
		{
			DrawFormatString(0, posY, 0xff0000, "   %d", userInterface_->GetStateAttackLevel());
		}
		shift += fontSize;
	}
	if (defenceStrFlag_)
	{
		DrawFormatString(0, posY + shift, 0x00bfff, "Lv.%d:DefenceUp!", userInterface_->GetStateDefenceLevel());
		if (LvDxCount_ % lvColorCnt)
		{
			DrawFormatString(0, posY + shift, 0x000000, "   %d", userInterface_->GetStateDefenceLevel());
		}
		else
		{
			DrawFormatString(0, posY + shift, 0x00bfff, "   %d", userInterface_->GetStateDefenceLevel());
		}
		shift += fontSize;
	}
	if (avoidStrFlag_)
	{
		DrawFormatString(0, posY + shift, 0xffff00, "Lv.%d:AvoidUp!", userInterface_->GetStateAvoidLevel());
		if (LvAvCount_ % lvColorCnt)
		{
			DrawFormatString(0, posY + shift, 0x000000, "   %d", userInterface_->GetStateAvoidLevel());
		}
		else
		{
			DrawFormatString(0, posY + shift, 0xffff00, "   %d", userInterface_->GetStateAvoidLevel());
		}
	}
	SetFontSize(16);
}

void Player::DrawLevelUpMirror(void)
{
	Size2D screenSize = Application::GetInstance().GetScreenSize();
	int fontSize = 32;
	int sizeShift = 280;
	int shift = 0;
	int lvColorCnt = 6;
	int posY = 150;

	if (!Application::GetInstance().IsWidowMode())
	{
		fontSize = 64;
		posY += 64;
		sizeShift += 280;
	}

	SetFontSize(fontSize);

	if (attackStrFlag_)
	{
		DrawFormatString(screenSize.x - sizeShift + fontSize, posY, 0xff0000, "Lv. :AttackUp!");
		if (LvAtCount_ % lvColorCnt)
		{
			DrawFormatString(screenSize.x - sizeShift + fontSize, posY, 0xffffff, "   %d", userInterface_->GetStateAttackLevel());
		}
		else
		{
			DrawFormatString(screenSize.x - sizeShift + fontSize, posY, 0xff0000, "   %d", userInterface_->GetStateAttackLevel());
		}
		shift += fontSize;
	}
	if (defenceStrFlag_)
	{
		DrawFormatString(screenSize.x - sizeShift + fontSize, posY + shift, 0x00bfff, "Lv.%d:DefenceUp!", userInterface_->GetStateDefenceLevel());
		if (LvDxCount_ % lvColorCnt)
		{
			DrawFormatString(screenSize.x - sizeShift + fontSize, posY + shift, 0x000000, "   %d", userInterface_->GetStateDefenceLevel());
		}
		else
		{
			DrawFormatString(screenSize.x - sizeShift + fontSize, posY + shift, 0x00bfff, "   %d", userInterface_->GetStateDefenceLevel());
		}
		shift += fontSize;
	}
	if (avoidStrFlag_)
	{
		DrawFormatString(screenSize.x - sizeShift + fontSize, posY + shift, 0xffff00, "Lv.%d:AvoidUp!", userInterface_->GetStateAvoidLevel());
		if (LvAvCount_ % lvColorCnt)
		{
			DrawFormatString(screenSize.x - sizeShift + fontSize, posY + shift, 0x000000, "   %d", userInterface_->GetStateAvoidLevel());
		}
		else
		{
			DrawFormatString(screenSize.x - sizeShift + fontSize, posY + shift, 0xffff00, "   %d", userInterface_->GetStateAvoidLevel());
		}
	}
	SetFontSize(16);
}