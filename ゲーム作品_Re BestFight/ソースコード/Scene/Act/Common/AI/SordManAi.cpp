#include"../../../../Utility/MyUtility.h"
#include"Node/NodeBase.h"
#include"Node/ActionNode.h"
#include"../../ActorBase.h"
#include"../../Common/Userinterface.h"
#include"../../Common/LevelUpManager.h"
#include"../../Common/AnimationController.h"
#include"../../../Common/Collider.h"
#include"../../../Camera.h"
#include "SordManAi.h"
#include<cmath>

SordManAi::SordManAi(std::weak_ptr<ActorBase> actor) : ActorAi(actor)
{
	frameSpeed_ = DEFAULT_FRAME_SPEED;
	ChangeMode(MODE::MANUAL);
	actor_ = actor;
	nowIndex_ = NONE_INDEX;
	nowCost_ = NONE_COST;
	//�����蔻��t���O
	colliderFlag_ = false;
	//�ړ����Ă��邩
	moveFlag_ = false;
	//�ړ�����
	moveDir_ = MyUtility::VECTOR_ZERO;
	//�R�}���h�F��U���̉񐔂��L�^
	atackWeakCnt_ = 0;
	atackStrongCnt_ = 0;
	//�R�}���h�������Ă��邩
	comandFlag_ = false;
	//������
	avoidDir_ = MyUtility::VECTOR_ZERO;
	//�O��̉�����
	prevAvoidDir_ = MyUtility::VECTOR_ZERO;
	//�W�����v���Ă��邩
	jumpFlag_ = false;
	//�W���X�g����Ɏg��
	damageFlag_ = false;

	InitTime();//���Ԃ̏�����
}

SordManAi::~SordManAi(void)
{
}

void SordManAi::ActionComandWeek_1(void)
{
	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_1)
	{
		//��Ԃ���U��_1�ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_1);
		//���O�̃Z�b�g
		SetTime(TIME::ATTACK_RAG, 0.25f);
		//�����蔻��t���O
		colliderFlag_ = false;
		//�U���t���O
		comandFlag_ = false;

		//�����ő���̕�������
		FactEnemy();
	}
	else
	{
		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (actor_.lock()->animController_->GetStepAnimTime() >= 20.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK &&
			actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != INDEX_CON_COMAND_COUNTERATTACK)
		{
			VERTEX3D vertex[3];
			for (int ii = 1; ii < actor_.lock()->GetAttackArea(); ii++)
			{
				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					//�����蔻��t���O
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageRightStagger(); });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 70.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.2f);
					actor_.lock()->SetStopFrame(0.2f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_CRASH, DX_PLAYTYPE_BACK);
					//�A�N�V�������I��
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//���肪�J�E���g���Ă����A���U���񐔂����Z�b�g
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
					
					//�G�t�F�N�g
					Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
					Rot3DF effRot = Quaternion::ToEuler(actor_.lock()->transform_->quaRotLocal);
					actor_.lock()->PlayEffect(EffectManager::EFF::FLASH, ActorBase::USE_EFFECT::FLASH, effPos, effRot);
					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

					actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);
					actor_.lock()->enemy_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);

					SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
					break;
				}
			}
		}

		//�a���G�t�F�N�g�̓v���C���[�ɒǏ]����
		if (comandFlag_ && actor_.lock()->IsPlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD))
		{
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorRightDir = actor_.lock()->transform_->GetRightLocal();
			effPos = VAdd(effPos, VScale(actorRightDir, 50.0f));
			actor_.lock()->SetEffectPos(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			//�f�o�b�O�p�̍U���t���O
			comandFlag_ = true;
			//�U���s���ɂ��ړ�����
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 240.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 70.0f));
			actor_.lock()->SetJumpVector({ 0.0f, 18.0f, 0.0f });
			//��U��G�t�F�N�g
			//��U��Ƃ����Ă��U�������������ꍇ�ł��Đ������
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorRightDir = actor_.lock()->transform_->GetRightLocal();
			effPos = VAdd(effPos, VScale(actorRightDir, 50.0f));
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
		}
		//�U�����[�V�������I������܂ŏ������s���Ȃ�
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//���g�̃t���[�����x�����ɖ߂�
			SetFrameDefaultSpeed();
			atackWeakCnt_ = 0;//�U���񐔂����Z�b�g
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//���O���Ȃ��A�O��������Ăі������ꂽ�ꍇ
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//���g�̃t���[�����x��x������
			SetFrameSpeed(0.7f);
			//�ق�̏������O�ɐi��
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));

			//��U�������𖞂������ꍇ
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();

			}
			//���U�������𖞂������ꍇ
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
				nowCost_ = NONE_COST;
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
			}
		}
	}
}

void SordManAi::ActionComandWeek_2(void)
{
	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_2)
	{
		//��Ԃ���U��_2�ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_2);
		//���O�̃Z�b�g
		SetTime(TIME::ATTACK_RAG, 0.25f);
		//�����蔻��t���O
		colliderFlag_ = false;
		//�U���t���O
		comandFlag_ = false;
	}
	else
	{
		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (actor_.lock()->animController_->GetStepAnimTime() >= 20.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK &&
			actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != INDEX_CON_COMAND_COUNTERATTACK)
		{
			VERTEX3D vertex[3];
			for (int ii = 1; ii < actor_.lock()->GetAttackArea(); ii++)
			{
				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					//�����蔻��t���O
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageLeftStagger(); });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 70.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.2f);
					actor_.lock()->SetStopFrame(0.2f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_CRASH, DX_PLAYTYPE_BACK);
					//�A�N�V�������I��
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//���肪�J�E���g���Ă����A���U���񐔂����Z�b�g
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
				
					//�G�t�F�N�g
					Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
					Rot3DF effRot = Quaternion::ToEuler(actor_.lock()->transform_->quaRotLocal);
					actor_.lock()->PlayEffect(EffectManager::EFF::FLASH, ActorBase::USE_EFFECT::FLASH, effPos, effRot);
					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

					actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);
					actor_.lock()->enemy_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);

					SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
					break;
				}
			}
		}

		//�a���G�t�F�N�g�̓v���C���[�ɒǏ]����
		if (comandFlag_ && actor_.lock()->IsPlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD))
		{
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorRightDir = actor_.lock()->transform_->GetRightLocal();
			effPos = VAdd(effPos, VScale(actorRightDir, 200.0f));
			actor_.lock()->SetEffectPos(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			//�f�o�b�O�p�̍U���t���O
			comandFlag_ = true;
			//�U���s���ɂ��ړ�����
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 240.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 70.0f));
			actor_.lock()->SetJumpVector({ 0.0f, 18.0f, 0.0f });
			//��U��G�t�F�N�g
			//��U��Ƃ����Ă��U�������������ꍇ�ł��Đ������
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorRightDir = actor_.lock()->transform_->GetRightLocal();
			effPos = VAdd(effPos, VScale(actorRightDir, 200.0f));
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.z += 90.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
		}
		//�U�����[�V�������I������܂ŏ������s���Ȃ�
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//���g�̃t���[�����x�����ɖ߂�
			SetFrameDefaultSpeed();
			atackWeakCnt_ = 0;//�U���񐔂����Z�b�g
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//���O���Ȃ��A�O��������Ăі������ꂽ�ꍇ
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//���g�̃t���[�����x��x������
			SetFrameSpeed(0.7f);
			//�ق�̏������O�ɐi��
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//��U�������𖞂������ꍇ
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//���U�������𖞂������ꍇ
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
				nowCost_ = NONE_COST;
			}
		}
	}
}

void SordManAi::ActionComandWeek_3(void)
{
	//���ȏ�A�������ꂼ��ʁX�ɔ��肷�邽�߂̃t���O
	static bool canCollFlag = true;

	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_3)
	{
		//��Ԃ���U��_3�ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_3);
		//���O�̃Z�b�g
		SetTime(TIME::ATTACK_RAG, 0.65f);
		//�����蔻��t���O
		colliderFlag_ = false;
		//�U���t���O
		comandFlag_ = false;

		canCollFlag = true;
	}
	else
	{
		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (actor_.lock()->animController_->GetStepAnimTime() >= 45.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK &&
			actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != INDEX_CON_COMAND_COUNTERATTACK)
		{
			bool twoAttack = false;
			if (actor_.lock()->animController_->GetStepAnimTime() >= 90.0f &&
				actor_.lock()->animController_->GetStepAnimTime() < 120.0f)
			{
				twoAttack = true;
				canCollFlag = true;
			}

			VERTEX3D vertex[3];
			for (int ii = 1; ii < actor_.lock()->GetAttackArea(); ii++)
			{
				if (!canCollFlag)break;

				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					if (!twoAttack)
					{
						canCollFlag = false;
					}
					else
					{
						actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 90.0f));
						actor_.lock()->enemy_.lock()->SetStopFrame(0.1f);
						actor_.lock()->SetStopFrame(0.1f);
						actor_.lock()->camera_.lock()->SetPos(
							VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
						
						//�����蔻��t���O
						colliderFlag_ = true;
					}

					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBackStagger(); });
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_CRASH, DX_PLAYTYPE_BACK);
					//�A�N�V�������I��
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//���肪�J�E���g���Ă����A���U���񐔂����Z�b�g
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
					
					//�G�t�F�N�g
					Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
					Rot3DF effRot = Quaternion::ToEuler(actor_.lock()->transform_->quaRotLocal);
					actor_.lock()->PlayEffect(EffectManager::EFF::FLASH, ActorBase::USE_EFFECT::FLASH, effPos, effRot);
					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

					actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);
					actor_.lock()->enemy_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);

					SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
					break;
				}
			}
		}

		//�a���G�t�F�N�g�̓v���C���[�ɒǏ]����
		if (comandFlag_ && actor_.lock()->IsPlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD))
		{
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorUpDir = actor_.lock()->transform_->GetUpLocal();
			effPos = VAdd(effPos, VScale(actorUpDir, 150.0f));
			actor_.lock()->SetEffectPos(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos);
		}
		//�a���G�t�F�N�g�̓v���C���[�ɒǏ]����_2
		if (comandFlag_ && actor_.lock()->IsPlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_2))
		{
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorUpDir = actor_.lock()->transform_->GetUpLocal();
			effPos = VAdd(effPos, VScale(actorUpDir, 150.0f));
			actor_.lock()->SetEffectPos(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_2, effPos);
		}

		//��U�艹
		if (actor_.lock()->animController_->GetStepAnimTime() > 55.0f &&
			actor_.lock()->animController_->GetStepAnimTime() <= 70.0f)
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//��U��G�t�F�N�g
			//��U��Ƃ����Ă��U�������������ꍇ�ł��Đ������
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorUpDir = actor_.lock()->transform_->GetUpLocal();
			effPos = VAdd(effPos, VScale(actorUpDir, 150.0f));
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.z += 40.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos, effRot);
		}
		else if(actor_.lock()->animController_->GetStepAnimTime() > 90.0f &&
			actor_.lock()->animController_->GetStepAnimTime() <= 95.0f)
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//��U��G�t�F�N�g
			//��U��Ƃ����Ă��U�������������ꍇ�ł��Đ������
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->transform_->pos;
			effPos.y -= 150.0f;
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.z += 180.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_2, effPos, effRot);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 4.0f)
		{
			//�f�o�b�O�p�̍U���t���O
			comandFlag_ = true;
			//�U���s���ɂ��ړ�����
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 240.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 70.0f));
			//actor_.lock()->SetJumpVector({ 0.0f, 18.0f, 0.0f });
		}
		//�U�����[�V�������I������܂ŏ������s���Ȃ�
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//���g�̃t���[�����x�����ɖ߂�
			SetFrameDefaultSpeed();
			atackWeakCnt_ = 0;//�U���񐔂����Z�b�g
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//���O���Ȃ��A�O��������Ăі������ꂽ�ꍇ
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//���g�̃t���[�����x��x������
			SetFrameSpeed(0.7f);
			//�ق�̏������O�ɐi��
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//��U�������𖞂������ꍇ
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//���U�������𖞂������ꍇ
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
				nowCost_ = NONE_COST;
			}
		}
	}
}

void SordManAi::ActionComandWeek_4(void)
{
	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_4)
	{
		//��Ԃ���U��_5�ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_4);
		//���O�̃Z�b�g
		SetTime(TIME::ATTACK_RAG, 0.42f);
		//�����蔻��t���O
		colliderFlag_ = false;
		//�U���t���O
		comandFlag_ = false;
	}
	else
	{
		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (actor_.lock()->animController_->GetStepAnimTime() >= 20.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK &&
			actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != INDEX_CON_COMAND_COUNTERATTACK)
		{
			VERTEX3D vertex[3];
			for (int ii = 1; ii < actor_.lock()->GetAttackArea(); ii++)
			{
				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					//�����蔻��t���O
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageLeftStagger(); });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 70.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.2f);
					actor_.lock()->SetStopFrame(0.2f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_CRASH, DX_PLAYTYPE_BACK);
					//�A�N�V�������I��
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//���肪�J�E���g���Ă����A���U���񐔂����Z�b�g
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
					
					//�G�t�F�N�g
					Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
					Rot3DF effRot = Quaternion::ToEuler(actor_.lock()->transform_->quaRotLocal);
					actor_.lock()->PlayEffect(EffectManager::EFF::FLASH, ActorBase::USE_EFFECT::FLASH, effPos, effRot);
					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

					actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);
					actor_.lock()->enemy_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);

					SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
					break;
				}
			}
		}

		//�a���G�t�F�N�g�̓v���C���[�ɒǏ]����
		if (comandFlag_ && actor_.lock()->IsPlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD))
		{
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorUpDir = actor_.lock()->transform_->GetUpLocal();
			effPos = VAdd(effPos, VScale(actorUpDir, 30.0f));
			actor_.lock()->SetEffectPos(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			//�f�o�b�O�p�̍U���t���O
			comandFlag_ = true;
			//�U���s���ɂ��ړ�����
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 240.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 80.0f));
			actor_.lock()->SetJumpVector({ 0.0f, 18.0f, 0.0f });
			//��U��G�t�F�N�g
			//��U��Ƃ����Ă��U�������������ꍇ�ł��Đ������
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorUpDir = actor_.lock()->transform_->GetUpLocal();
			effPos = VAdd(effPos, VScale(actorUpDir, 30.0f));
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
		}
		//�U�����[�V�������I������܂ŏ������s���Ȃ�
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//���g�̃t���[�����x�����ɖ߂�
			SetFrameDefaultSpeed();
			atackWeakCnt_ = 0;//�U���񐔂����Z�b�g
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//���O���Ȃ��A�O��������Ăі������ꂽ�ꍇ
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//���g�̃t���[�����x��x������
			SetFrameSpeed(0.7f);
			//�ق�̏������O�ɐi��
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//��U�������𖞂������ꍇ
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//���U�������𖞂������ꍇ
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
				nowCost_ = NONE_COST;
			}
		}
	}
}

void SordManAi::ActionComandWeek_5(void)
{
	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_5)
	{
		//��Ԃ���U��_5�ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_5);
		//���O�̃Z�b�g
		SetTime(TIME::ATTACK_RAG, 0.32f);
		//�����蔻��t���O
		colliderFlag_ = false;
		//�U���t���O
		comandFlag_ = false;
	}
	else
	{
		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (actor_.lock()->animController_->GetStepAnimTime() >= 20.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK &&
			actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != INDEX_CON_COMAND_COUNTERATTACK)
		{
			VERTEX3D vertex[3];
			for (int ii = 1; ii < actor_.lock()->GetAttackArea(); ii++)
			{
				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					//�����蔻��t���O
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBackStagger(); });
					//�W�����v���50�傫���G��ł��グ��
					actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, 20.0f ,0.0f });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 50.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.1f);
					actor_.lock()->SetStopFrame(0.1f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_STRONG, DX_PLAYTYPE_BACK);
					//�A�N�V�������I��
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//���肪�J�E���g���Ă����A���U���񐔂����Z�b�g
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
					
					//�G�t�F�N�g
					Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
					Rot3DF effRot = Quaternion::ToEuler(actor_.lock()->transform_->quaRotLocal);
					actor_.lock()->PlayEffect(EffectManager::EFF::FLASH, ActorBase::USE_EFFECT::FLASH, effPos, effRot);
					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

					actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);
					actor_.lock()->enemy_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);

					SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
					break;
				}
			}
		}


		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			//�f�o�b�O�p�̍U���t���O
			comandFlag_ = true;
			//�U���s���ɂ��ړ�����
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 320.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 50.0f));
			//��U��G�t�F�N�g
			//��U��Ƃ����Ă��U�������������ꍇ�ł��Đ������
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
		}
		//�U�����[�V�������I������܂ŏ������s���Ȃ�
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//���g�̃t���[�����x�����ɖ߂�
			SetFrameDefaultSpeed();
			atackWeakCnt_ = 0;//�U���񐔂����Z�b�g
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//���O���Ȃ��A�O��������Ăі������ꂽ�ꍇ
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//���g�̃t���[�����x��x������
			SetFrameSpeed(0.7f);
			//�ق�̏������O�ɐi��
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//��U�������𖞂������ꍇ
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//���U�������𖞂������ꍇ
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
				nowCost_ = NONE_COST;
			}
		}
	}
}

void SordManAi::ActionComandWeek_6(void)
{
	float dis = VSquareSize(VSub(actor_.lock()->enemy_.lock()->transform_->pos,
		actor_.lock()->transform_->pos));
	//�����ɂ��J�������[�h��؂�ւ���
	if (dis < std::powf(ActorAi::DISTANCE_MEDIUM, 2.7f) && 
		actor_.lock()->actorCam_->GetCameraMode() != Camera::MODE::SPECIAL_ATTACK_MODE)
	{
		//�J�������[�h
		actor_.lock()->actorCam_->ChangeMode(Camera::MODE::SPECIAL_ATTACK_MODE);
		actor_.lock()->actorCam_->SetSpecialCamera([&]() {actor_.lock()->WeekFinishCamera(); });
	}

	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_6)
	{
		//��Ԃ���U��_6�ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_6);
		//���O�̃Z�b�g
		SetTime(TIME::ATTACK_RAG, 1.4f);
		//�����蔻��t���O
		colliderFlag_ = false;
		//�U���t���O
		comandFlag_ = false;
		//�����ő���̕�������
		FactEnemy();
	}
	else
	{
		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (actor_.lock()->animController_->GetStepAnimTime() >= 40.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK &&
			actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != INDEX_CON_COMAND_COUNTERATTACK)
		{
			VERTEX3D vertex[3];
			for (int ii = 1; ii < actor_.lock()->GetAttackArea(); ii++)
			{
				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					//�����蔻��t���O
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });
					//�W�����v���50�傫���G��ł��グ��
					actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, 32.0f ,0.0f });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 20.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.2f);
					actor_.lock()->SetStopFrame(0.2f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::SORD_FINISH, DX_PLAYTYPE_BACK);
					//�A�N�V�������I��
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//���肪�J�E���g���Ă����A���U���񐔂����Z�b�g
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
		
					//�G�t�F�N�g
					Position3DF effPos = actor_.lock()->enemy_.lock()->GetPos();
					effPos.y += 200.0f;
					Rot3DF effRot = Quaternion::ToEuler(actor_.lock()->transform_->quaRotLocal);
					effRot.z -= 10.0f;
					actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos, effRot);
					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

					actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);
					actor_.lock()->enemy_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);

					SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
					break;
				}
			}
		}


		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 40.0f)
		{
			//�f�o�b�O�p�̍U���t���O
			comandFlag_ = true;
			//�U���s���ɂ��ړ�����
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 320.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 150.0f));
		}
		//�U�����[�V�������I������܂ŏ������s���Ȃ�
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//���g�̃t���[�����x�����ɖ߂�
			SetFrameDefaultSpeed();
			actor_.lock()->actorCam_->ChangeDefaultMode();
			atackWeakCnt_ = 0;//�U���񐔂����Z�b�g
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//���O���Ȃ��A�O��������Ăі������ꂽ�ꍇ
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//���g�̃t���[�����x��x������
			SetFrameSpeed(0.7f);
			//�ق�̏������O�ɐi��
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//��U�������𖞂������ꍇ
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//���U�������𖞂������ꍇ
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
				nowCost_ = NONE_COST;
			}
		}
	}
}

void SordManAi::ActionComandStrong_1(void)
{
	//���ȏ�A�������ꂼ��ʁX�ɔ��肷�邽�߂̃t���O
	static bool canCollFlag = false;
	static int collCnt = 0;
	struct AnimTime
	{
		float start = 0.0f;
		float end = 0.0f;
	};
	static constexpr int ANIM_TIME_MAX = 3;
	static AnimTime animTime[ANIM_TIME_MAX]{
						  {10.0f, 20.0f},
						  {40.0f, 50.0f},
						  {60.0f, 70.0f} };

	if (actor_.lock()->GetState() != ActorBase::STATE::STRONG_1)
	{
		canCollFlag = false;
		collCnt = 0;

		//��Ԃ����U��_1�ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::STRONG_1);
		SetTime(TIME::ATTACK_RAG, 0.45f);
		//�����蔻��t���O
		colliderFlag_ = false;
		//�U���t���O
		comandFlag_ = false;

		//�����ő���̕�������
		FactEnemy();
	}
	else
	{
		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (actor_.lock()->animController_->GetStepAnimTime() >= 10.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK &&
			actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != INDEX_CON_COMAND_COUNTERATTACK)
		{
			if (actor_.lock()->animController_->GetStepAnimTime() >= animTime[collCnt].start &&
				actor_.lock()->animController_->GetStepAnimTime() < animTime[collCnt].end)
			{
				canCollFlag = true;
			}

			VERTEX3D vertex[3];

			for (int ii = 1; ii < actor_.lock()->GetAttackArea(); ii++)
			{
				if (!canCollFlag)break;

				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					if (collCnt >= ANIM_TIME_MAX - 1)
					{
						actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 70.0f));
						actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBackStagger(); });
						actor_.lock()->camera_.lock()->SetPos(
							VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
						SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);

						collCnt = 0;
						canCollFlag = false;
						//�����蔻��t���O
						colliderFlag_ = true;
					}
					else if (collCnt % 2)
					{
						actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageLeftStagger(); });
						actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 60.0f));
						collCnt++;
						canCollFlag = false;
					}
					else if (!(collCnt % 2))
					{
						actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageRightStagger(); });
						actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 60.0f));
						collCnt++;
						canCollFlag = false;
					}

					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					//�A�N�V�������I��
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//���肪�J�E���g���Ă����A���U���񐔂����Z�b�g
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
				
					//�G�t�F�N�g
					Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
					Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
					actor_.lock()->PlayEffect(EffectManager::EFF::FLASH, ActorBase::USE_EFFECT::FLASH, effPos, effRot);
					effPos = actor_.lock()->GetPlayerForwardPos(200.0f);

					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_CRASH, DX_PLAYTYPE_BACK);
					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);
					break;
				}
			}
		}

		//��U�艹
		//�G�t�F�N�g�̍Đ�
		if (actor_.lock()->animController_->GetStepAnimTime() > animTime[0].start &&
			actor_.lock()->animController_->GetStepAnimTime() <= animTime[0].end)
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//��U��G�t�F�N�g
			//��U��Ƃ����Ă��U�������������ꍇ�ł��Đ������
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorRightDir = actor_.lock()->transform_->GetRightLocal();
			effPos = VAdd(effPos, VScale(actorRightDir, 200.0f));
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.z += 90.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos, effRot);
		}
		else if (actor_.lock()->animController_->GetStepAnimTime() > animTime[1].start &&
			actor_.lock()->animController_->GetStepAnimTime() <= animTime[1].end)
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//��U��G�t�F�N�g
			//��U��Ƃ����Ă��U�������������ꍇ�ł��Đ������
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorRightDir = actor_.lock()->transform_->GetRightLocal();
			effPos = VAdd(effPos, VScale(actorRightDir, 200.0f));
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.z += 90.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_2, effPos, effRot);
		}
		else if (actor_.lock()->animController_->GetStepAnimTime() > animTime[2].start &&
			actor_.lock()->animController_->GetStepAnimTime() <= animTime[2].end)
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//��U��G�t�F�N�g
			//��U��Ƃ����Ă��U�������������ꍇ�ł��Đ������
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorRightDir = actor_.lock()->transform_->GetRightLocal();
			effPos = VAdd(effPos, VScale(actorRightDir, 200.0f));
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.z += 90.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_3, effPos, effRot);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 5.0f)
		{
			//�f�o�b�O�p�̍U���t���O
			comandFlag_ = true;
			//�U���s���ɂ��ړ�����
			//Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 160.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 120.0f));
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
		}
		//�U�����[�V�������I������܂ŏ������s���Ȃ�
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			atackWeakCnt_ = 0;//�U���񐔂����Z�b�g
			atackStrongCnt_ = 0;
			actor_.lock()->actorCam_->ChangeDefaultMode();
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//���O���Ȃ��A�O��������Ăі������ꂽ�ꍇ
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//���g�̃t���[�����x��x������
			SetFrameSpeed(0.7f);
			//�ق�̏������O�ɐi��
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//��U�������𖞂������ꍇ
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//���U�������𖞂������ꍇ
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
				nowCost_ = NONE_COST;
			}
		}
	}
}

void SordManAi::ActionComandStrong_2(void)
{
	if (actor_.lock()->GetState() != ActorBase::STATE::STRONG_2)
	{
		//��Ԃ����U��_2�ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::STRONG_2);
		SetTime(TIME::ATTACK_RAG, 0.4f);
		//�����蔻��t���O
		colliderFlag_ = false;
		//�U���t���O
		comandFlag_ = false;
		actor_.lock()->StopEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD);
		actor_.lock()->StopEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_2);
		actor_.lock()->StopEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_3);
	}
	else
	{
		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (actor_.lock()->animController_->GetStepAnimTime() >= 30.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK &&
			actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != INDEX_CON_COMAND_COUNTERATTACK)
		{
			VERTEX3D vertex[3];

			for (int ii = 1; ii < actor_.lock()->GetAttackArea(); ii++)
			{
				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					//�����蔻��t���O
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 90.0f));
					actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, actor_.lock()->GetJumpPower() + 10.0f ,0.0f });
					actor_.lock()->enemy_.lock()->SetStopFrame(0.1f);
					actor_.lock()->SetStopFrame(0.1f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_STRONG, DX_PLAYTYPE_BACK);
					//�A�N�V�������I��
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//���肪�J�E���g���Ă����A���U���񐔂����Z�b�g
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
				
					//�G�t�F�N�g
					Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
					Rot3DF effRot = Quaternion::ToEuler(actor_.lock()->transform_->quaRotLocal);
					actor_.lock()->PlayEffect(EffectManager::EFF::STRONG, ActorBase::USE_EFFECT::STRONG, effPos, effRot);
					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

					actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);
					actor_.lock()->enemy_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);

					SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
					break;
				}
			}
		}

		//�a���G�t�F�N�g�̓v���C���[�ɒǏ]����
		if (actor_.lock()->IsPlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD))
		{
			Position3DF effPos = actor_.lock()->GetPos();
			effPos.x += 200.0f;
			actor_.lock()->SetEffectPos(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos);
		}

		//�G�t�F�N�g�̔���
		else if (actor_.lock()->animController_->GetStepAnimTime() >= 30.0f)
		{
			//��U��G�t�F�N�g
			//��U��Ƃ����Ă��U�������������ꍇ�ł��Đ������
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPos();
			effPos.x += 200.0f;
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.z += 90.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos, effRot);
		}

		if (!comandFlag_)
		{
			//�f�o�b�O�p�̍U���t���O
			comandFlag_ = true;
			//�U���s���ɂ��ړ�����
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 160.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 60.0f));
		}
		//�U�����[�V�������I������܂ŏ������s���Ȃ�
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//���g�̃t���[�����x�����ɖ߂�
			SetFrameDefaultSpeed();
			atackWeakCnt_ = 0;//�U���񐔂����Z�b�g
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//���O���Ȃ��A�O��������Ăі������ꂽ�ꍇ
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//���g�̃t���[�����x��x������
			SetFrameSpeed(0.7f);
			//�ق�̏������O�ɐi��
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 20.0f));
			//��U�������𖞂������ꍇ
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//���U�������𖞂������ꍇ
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//���g�̃t���[�����x�����ɖ߂�
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
				nowCost_ = NONE_COST;
			}
		}
	}
}

void SordManAi::ActionComandStrong_3(void)
{
	float dis = VSquareSize(VSub(actor_.lock()->enemy_.lock()->transform_->pos,
		actor_.lock()->transform_->pos));
	//�����ɂ��J�������[�h��؂�ւ���
	if (dis < std::powf(ActorAi::DISTANCE_MEDIUM, 2.7f) &&
		actor_.lock()->actorCam_->GetCameraMode() != Camera::MODE::SPECIAL_ATTACK_MODE)
	{
		//�J�������[�h
		actor_.lock()->actorCam_->ChangeMode(Camera::MODE::SPECIAL_ATTACK_MODE);
		actor_.lock()->actorCam_->SetSpecialCamera([&]() {actor_.lock()->StrongFinishCamera(); });
	}

	if (actor_.lock()->GetState() != ActorBase::STATE::STRONG_3)
	{
		//��Ԃ����U��_3�ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::STRONG_3);
		SetTime(TIME::ATTACK_RAG, 1.0f);
		//�����蔻��t���O
		colliderFlag_ = false;
		//�U���t���O
		comandFlag_ = false;
		actor_.lock()->StopEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD);
		actor_.lock()->StopEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_2);
		actor_.lock()->StopEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_3);
	}
	else
	{
		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (actor_.lock()->animController_->GetStepAnimTime() >= 40.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK &&
			actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != INDEX_CON_COMAND_COUNTERATTACK)
		{
			VERTEX3D vertex[3];

			for (int ii = 1; ii < actor_.lock()->GetAttackArea(); ii++)
			{
				Dir3DF dir = VScale(actor_.lock()->transform_->GetUpLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormUpInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					//�����蔻��t���O
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 50.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.8f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::SORD_FINISH, DX_PLAYTYPE_BACK);
					//�A�N�V�������I��
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//���肪�J�E���g���Ă����A���U���񐔂����Z�b�g
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
					
					//�G�t�F�N�g
					Position3DF effPos = actor_.lock()->enemy_.lock()->GetPos();
				//	effPos.x += 300.0f;
					Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
					effRot.z += 40.0f;
					actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos, effRot);

					//effPos.x -= 300.0f;
					effRot.z += 180.0f;
					actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_2, effPos, effRot);

					//effPos.z -= 200.0f;
					effRot.z += 90.0f;
					actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_3, effPos, effRot);

					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

					actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);
					actor_.lock()->enemy_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);

					SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
					break;
				}
			}
		}


		if (!comandFlag_)
		{
			//�f�o�b�O�p�̍U���t���O
			comandFlag_ = true;
			//�U���s���ɂ��ړ�����
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 160.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 95.0f));
		}
		//�U�����[�V�������I������܂ŏ������s���Ȃ�
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			actor_.lock()->actorCam_->ChangeDefaultMode();
			atackWeakCnt_ = 0;//�U���񐔂����Z�b�g
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}

void SordManAi::ActionComandUnleashSkill(void)
{
	//�G���ꎞ��~���Ă��邩
	static bool enemyStop = true;

	if (actor_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL)
	{
		enemyStop = true;

		//��Ԃ�K�E�Z�ɂ���
		actor_.lock()->ChangeState(ActorBase::STATE::UNLEASH_SKILL);
		actor_.lock()->SetStopFrame(0.1f);
		SetFrameSpeed(0.5f);

		//�G�t�F�N�g
		Position3DF effPos = actor_.lock()->transform_->pos;
		Rot3DF effRot = Rot3DF();
		actor_.lock()->PlayEffect(EffectManager::EFF::LIGHTNING, ActorBase::USE_EFFECT::LIGHTNING, effPos, effRot);

		//�����ő���̕�������
		FactEnemy();

		//�����蔻��t���O
		colliderFlag_ = false;
		//�U���t���O
		comandFlag_ = false;
		if (!actor_.lock()->GetCpuFlag())
		{
			//�R���g���[����U��������
			StartJoypadVibration(actor_.lock()->GetPlayerId(), 1000, 1000, -1);
		}

		//�K�E�Z��
		actor_.lock()->SoundPlay(SoundManager::SOD::SKILL_THUNDER, DX_PLAYTYPE_BACK);
	}
	else
	{
		//���g�̃t���[�����x
		if (actor_.lock()->animController_->GetStepAnimTime() >= 5.0f && enemyStop)
		{
			//�t���[�����x���f�t�H���g�ɖ߂�
			SetFrameDefaultSpeed();
		}

		//����̈ꎞ��~
		if (actor_.lock()->animController_->GetStepAnimTime() < 20.0f)
		{
			actor_.lock()->enemy_.lock()->SetStopFrame(3.0f);
		}
		else if(enemyStop)
		{
			enemyStop = false;
			actor_.lock()->enemy_.lock()->SetStopFrame(0.0f);
			//�����ő���̕�������
			//�ꉞ�̏���
			FactEnemy();
		}

		//�����蔻��̔���
		//�A�j���[�V�����̃X�e�b�v�^�C���Ŕ��f����
		if (actor_.lock()->animController_->GetStepAnimTime() >= 20.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL)
		{
			VERTEX3D vertex[3];

			for (int ii = 1; ii < actor_.lock()->GetAttackArea(); ii++)
			{
				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					//�J�������g�p���邩
					if (actor_.lock()->actorCam_->GetCameraMode() != Camera::MODE::SKILL_MODE)
					{
						//�J�������[�h
						actor_.lock()->actorCam_->ChangeMode(Camera::MODE::SKILL_MODE);
						actor_.lock()->actorCam_->SetSpecialCamera([&]() {actor_.lock()->SkillCamera();});
					}

					//�����蔻��t���O
					colliderFlag_ = true;

					//���g�𑊎�̌��ɏu�Ԉړ�
					Position3DF pos = VAdd(actor_.lock()->enemy_.lock()->transform_->pos,
						VScale(actor_.lock()->enemy_.lock()->transform_->GetBackLocal(), 100.0f));
					actor_.lock()->transform_->pos = pos;

					actor_.lock()->enemy_.lock()->SetStopFrame(2.5f);

					//���g�̃t���[�����x
					SetFrameSpeed(0.2f);

					//���肪�K�[�h���Ȃ狭������
					if (actor_.lock()->enemy_.lock()->GetState() == ActorBase::STATE::DEFENCE)
					{
						actor_.lock()->enemy_.lock()->SetDefaultWeight();
						actor_.lock()->enemy_.lock()->StopEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA);
						actor_.lock()->enemy_.lock()->userInterface_->SetDefence(0.0f);
						comandFlag_ = false;
					}
					
					//�A�N�V�������I��
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
					}
					//���肪�J�E���g���Ă����A���U���񐔂����Z�b�g
					actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();

					//��������������
					actor_.lock()->SoundPlay(SoundManager::SOD::SORD_HIT, DX_PLAYTYPE_BACK);
					break;
				}
			}
		}

		//�����蔻�肪�������ꍇ�A��莞�Ԍ�Ɏ��g�̃t���[�����x����ɖ߂�
		if (colliderFlag_ && actor_.lock()->animController_->GetStepAnimTime() > 25.0f)
		{
			SetFrameDefaultSpeed();
		}

		//�G�t�F�N�g�̔���
		if (actor_.lock()->animController_->GetStepAnimTime() >= 60.0f &&
			!actor_.lock()->IsPlayEffect(EffectManager::EFF::SKILL_SORD, ActorBase::USE_EFFECT::SKILL_SORD))
		{
			if (colliderFlag_)
			{
				//�_���[�W���G�t�F�N�g�̃^�C�~���O�ŗ^����
				float power = actor_.lock()->GetAttackPower();
				actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power * 7.5f);
				actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });
				actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 90.0f));
				actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, actor_.lock()->GetJumpPower() + 10.0f ,0.0f });
			}

			//��U��G�t�F�N�g
			//��U��Ƃ����Ă��U�������������ꍇ�ł��Đ������
			//�G�t�F�N�g
			Position3DF effPos = actor_.lock()->GetPos();
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.y += MyUtility::Deg2RadF(180.0f);
			actor_.lock()->PlayEffect(EffectManager::EFF::SKILL_SORD, ActorBase::USE_EFFECT::SKILL_SORD, effPos, effRot);

			//����
			actor_.lock()->SoundPlay(SoundManager::SOD::THUNDER_BOLT, DX_PLAYTYPE_BACK);

			SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT + 70);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			//�f�o�b�O�p�̍U���t���O
			comandFlag_ = true;
			//�U���s���ɂ��ړ�����
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 160.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 180.0f));
		}
		//�U�����[�V�������I������܂ŏ������s���Ȃ�
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			atackWeakCnt_ = 0;//�U���񐔂����Z�b�g
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			actor_.lock()->actorCam_->ChangeDefaultMode();
			actor_.lock()->StopEffect(EffectManager::EFF::SKILL_SORD, ActorBase::USE_EFFECT::SKILL_SORD);
			//�K�E�Z�Q�[�W�E�������x��������������
			actor_.lock()->userInterface_->InitSkillandLevel();
			//�A�N�V�������I��
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}