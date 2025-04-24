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
	//当たり判定フラグ
	colliderFlag_ = false;
	//移動しているか
	moveFlag_ = false;
	//移動方向
	moveDir_ = MyUtility::VECTOR_ZERO;
	//コマンド：弱攻撃の回数を記録
	atackWeakCnt_ = 0;
	atackStrongCnt_ = 0;
	//コマンド処理しているか
	comandFlag_ = false;
	//回避方向
	avoidDir_ = MyUtility::VECTOR_ZERO;
	//前回の回避方向
	prevAvoidDir_ = MyUtility::VECTOR_ZERO;
	//ジャンプしているか
	jumpFlag_ = false;
	//ジャスト判定に使う
	damageFlag_ = false;

	InitTime();//時間の初期化
}

SordManAi::~SordManAi(void)
{
}

void SordManAi::ActionComandWeek_1(void)
{
	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_1)
	{
		//状態を弱攻撃_1にする
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_1);
		//ラグのセット
		SetTime(TIME::ATTACK_RAG, 0.25f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;

		//自動で相手の方を向く
		FactEnemy();
	}
	else
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
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
					//当たり判定フラグ
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
					//アクションを終了
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//相手がカウントしていた連続攻撃回数をリセット
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
					
					//エフェクト
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

		//斬撃エフェクトはプレイヤーに追従する
		if (comandFlag_ && actor_.lock()->IsPlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD))
		{
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorRightDir = actor_.lock()->transform_->GetRightLocal();
			effPos = VAdd(effPos, VScale(actorRightDir, 50.0f));
			actor_.lock()->SetEffectPos(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 240.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 70.0f));
			actor_.lock()->SetJumpVector({ 0.0f, 18.0f, 0.0f });
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorRightDir = actor_.lock()->transform_->GetRightLocal();
			effPos = VAdd(effPos, VScale(actorRightDir, 50.0f));
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
		}
		//攻撃モーションが終了するまで処理が行われない
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//自身のフレーム速度を元に戻す
			SetFrameDefaultSpeed();
			atackWeakCnt_ = 0;//攻撃回数をリセット
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//ラグがなく、前提条件が再び満たされた場合
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//自身のフレーム速度を遅くする
			SetFrameSpeed(0.7f);
			//ほんの少しずつ前に進む
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));

			//弱攻撃条件を満たした場合
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();

			}
			//強攻撃条件を満たした場合
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
				nowCost_ = NONE_COST;
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();
			}
		}
	}
}

void SordManAi::ActionComandWeek_2(void)
{
	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_2)
	{
		//状態を弱攻撃_2にする
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_2);
		//ラグのセット
		SetTime(TIME::ATTACK_RAG, 0.25f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
	}
	else
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
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
					//当たり判定フラグ
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
					//アクションを終了
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//相手がカウントしていた連続攻撃回数をリセット
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
				
					//エフェクト
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

		//斬撃エフェクトはプレイヤーに追従する
		if (comandFlag_ && actor_.lock()->IsPlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD))
		{
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorRightDir = actor_.lock()->transform_->GetRightLocal();
			effPos = VAdd(effPos, VScale(actorRightDir, 200.0f));
			actor_.lock()->SetEffectPos(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 240.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 70.0f));
			actor_.lock()->SetJumpVector({ 0.0f, 18.0f, 0.0f });
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorRightDir = actor_.lock()->transform_->GetRightLocal();
			effPos = VAdd(effPos, VScale(actorRightDir, 200.0f));
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.z += 90.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
		}
		//攻撃モーションが終了するまで処理が行われない
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//自身のフレーム速度を元に戻す
			SetFrameDefaultSpeed();
			atackWeakCnt_ = 0;//攻撃回数をリセット
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//ラグがなく、前提条件が再び満たされた場合
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//自身のフレーム速度を遅くする
			SetFrameSpeed(0.7f);
			//ほんの少しずつ前に進む
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//弱攻撃条件を満たした場合
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//強攻撃条件を満たした場合
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
				nowCost_ = NONE_COST;
			}
		}
	}
}

void SordManAi::ActionComandWeek_3(void)
{
	//二回以上連続をそれぞれ別々に判定するためのフラグ
	static bool canCollFlag = true;

	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_3)
	{
		//状態を弱攻撃_3にする
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_3);
		//ラグのセット
		SetTime(TIME::ATTACK_RAG, 0.65f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;

		canCollFlag = true;
	}
	else
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
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
						
						//当たり判定フラグ
						colliderFlag_ = true;
					}

					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBackStagger(); });
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_CRASH, DX_PLAYTYPE_BACK);
					//アクションを終了
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//相手がカウントしていた連続攻撃回数をリセット
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
					
					//エフェクト
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

		//斬撃エフェクトはプレイヤーに追従する
		if (comandFlag_ && actor_.lock()->IsPlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD))
		{
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorUpDir = actor_.lock()->transform_->GetUpLocal();
			effPos = VAdd(effPos, VScale(actorUpDir, 150.0f));
			actor_.lock()->SetEffectPos(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos);
		}
		//斬撃エフェクトはプレイヤーに追従する_2
		if (comandFlag_ && actor_.lock()->IsPlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_2))
		{
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorUpDir = actor_.lock()->transform_->GetUpLocal();
			effPos = VAdd(effPos, VScale(actorUpDir, 150.0f));
			actor_.lock()->SetEffectPos(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_2, effPos);
		}

		//空振り音
		if (actor_.lock()->animController_->GetStepAnimTime() > 55.0f &&
			actor_.lock()->animController_->GetStepAnimTime() <= 70.0f)
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
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
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->transform_->pos;
			effPos.y -= 150.0f;
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.z += 180.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_2, effPos, effRot);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 4.0f)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 240.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 70.0f));
			//actor_.lock()->SetJumpVector({ 0.0f, 18.0f, 0.0f });
		}
		//攻撃モーションが終了するまで処理が行われない
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//自身のフレーム速度を元に戻す
			SetFrameDefaultSpeed();
			atackWeakCnt_ = 0;//攻撃回数をリセット
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//ラグがなく、前提条件が再び満たされた場合
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//自身のフレーム速度を遅くする
			SetFrameSpeed(0.7f);
			//ほんの少しずつ前に進む
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//弱攻撃条件を満たした場合
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//強攻撃条件を満たした場合
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
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
		//状態を弱攻撃_5にする
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_4);
		//ラグのセット
		SetTime(TIME::ATTACK_RAG, 0.42f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
	}
	else
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
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
					//当たり判定フラグ
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
					//アクションを終了
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//相手がカウントしていた連続攻撃回数をリセット
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
					
					//エフェクト
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

		//斬撃エフェクトはプレイヤーに追従する
		if (comandFlag_ && actor_.lock()->IsPlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD))
		{
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorUpDir = actor_.lock()->transform_->GetUpLocal();
			effPos = VAdd(effPos, VScale(actorUpDir, 30.0f));
			actor_.lock()->SetEffectPos(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 240.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 80.0f));
			actor_.lock()->SetJumpVector({ 0.0f, 18.0f, 0.0f });
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorUpDir = actor_.lock()->transform_->GetUpLocal();
			effPos = VAdd(effPos, VScale(actorUpDir, 30.0f));
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
		}
		//攻撃モーションが終了するまで処理が行われない
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//自身のフレーム速度を元に戻す
			SetFrameDefaultSpeed();
			atackWeakCnt_ = 0;//攻撃回数をリセット
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//ラグがなく、前提条件が再び満たされた場合
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//自身のフレーム速度を遅くする
			SetFrameSpeed(0.7f);
			//ほんの少しずつ前に進む
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//弱攻撃条件を満たした場合
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//強攻撃条件を満たした場合
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
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
		//状態を弱攻撃_5にする
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_5);
		//ラグのセット
		SetTime(TIME::ATTACK_RAG, 0.32f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
	}
	else
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
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
					//当たり判定フラグ
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBackStagger(); });
					//ジャンプより50大きく敵を打ち上げる
					actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, 20.0f ,0.0f });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 50.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.1f);
					actor_.lock()->SetStopFrame(0.1f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_STRONG, DX_PLAYTYPE_BACK);
					//アクションを終了
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//相手がカウントしていた連続攻撃回数をリセット
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
					
					//エフェクト
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
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 320.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 50.0f));
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
		}
		//攻撃モーションが終了するまで処理が行われない
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//自身のフレーム速度を元に戻す
			SetFrameDefaultSpeed();
			atackWeakCnt_ = 0;//攻撃回数をリセット
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//ラグがなく、前提条件が再び満たされた場合
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//自身のフレーム速度を遅くする
			SetFrameSpeed(0.7f);
			//ほんの少しずつ前に進む
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//弱攻撃条件を満たした場合
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//強攻撃条件を満たした場合
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
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
	//距離によりカメラモードを切り替える
	if (dis < std::powf(ActorAi::DISTANCE_MEDIUM, 2.7f) && 
		actor_.lock()->actorCam_->GetCameraMode() != Camera::MODE::SPECIAL_ATTACK_MODE)
	{
		//カメラモード
		actor_.lock()->actorCam_->ChangeMode(Camera::MODE::SPECIAL_ATTACK_MODE);
		actor_.lock()->actorCam_->SetSpecialCamera([&]() {actor_.lock()->WeekFinishCamera(); });
	}

	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_6)
	{
		//状態を弱攻撃_6にする
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_6);
		//ラグのセット
		SetTime(TIME::ATTACK_RAG, 1.4f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
		//自動で相手の方を向く
		FactEnemy();
	}
	else
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
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
					//当たり判定フラグ
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });
					//ジャンプより50大きく敵を打ち上げる
					actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, 32.0f ,0.0f });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 20.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.2f);
					actor_.lock()->SetStopFrame(0.2f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::SORD_FINISH, DX_PLAYTYPE_BACK);
					//アクションを終了
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//相手がカウントしていた連続攻撃回数をリセット
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
		
					//エフェクト
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
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 320.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 150.0f));
		}
		//攻撃モーションが終了するまで処理が行われない
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//自身のフレーム速度を元に戻す
			SetFrameDefaultSpeed();
			actor_.lock()->actorCam_->ChangeDefaultMode();
			atackWeakCnt_ = 0;//攻撃回数をリセット
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//ラグがなく、前提条件が再び満たされた場合
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//自身のフレーム速度を遅くする
			SetFrameSpeed(0.7f);
			//ほんの少しずつ前に進む
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//弱攻撃条件を満たした場合
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//強攻撃条件を満たした場合
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
				nowCost_ = NONE_COST;
			}
		}
	}
}

void SordManAi::ActionComandStrong_1(void)
{
	//二回以上連続をそれぞれ別々に判定するためのフラグ
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

		//状態を強攻撃_1にする
		actor_.lock()->ChangeState(ActorBase::STATE::STRONG_1);
		SetTime(TIME::ATTACK_RAG, 0.45f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;

		//自動で相手の方を向く
		FactEnemy();
	}
	else
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
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
						//当たり判定フラグ
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
					//アクションを終了
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//相手がカウントしていた連続攻撃回数をリセット
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
				
					//エフェクト
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

		//空振り音
		//エフェクトの再生
		if (actor_.lock()->animController_->GetStepAnimTime() > animTime[0].start &&
			actor_.lock()->animController_->GetStepAnimTime() <= animTime[0].end)
		{
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
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
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
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
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPos();
			Dir3DF actorRightDir = actor_.lock()->transform_->GetRightLocal();
			effPos = VAdd(effPos, VScale(actorRightDir, 200.0f));
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.z += 90.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_3, effPos, effRot);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 5.0f)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			//Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 160.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 120.0f));
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
		}
		//攻撃モーションが終了するまで処理が行われない
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			atackWeakCnt_ = 0;//攻撃回数をリセット
			atackStrongCnt_ = 0;
			actor_.lock()->actorCam_->ChangeDefaultMode();
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//ラグがなく、前提条件が再び満たされた場合
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//自身のフレーム速度を遅くする
			SetFrameSpeed(0.7f);
			//ほんの少しずつ前に進む
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//弱攻撃条件を満たした場合
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//強攻撃条件を満たした場合
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
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
		//状態を強攻撃_2にする
		actor_.lock()->ChangeState(ActorBase::STATE::STRONG_2);
		SetTime(TIME::ATTACK_RAG, 0.4f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
		actor_.lock()->StopEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD);
		actor_.lock()->StopEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_2);
		actor_.lock()->StopEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_3);
	}
	else
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
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
					//当たり判定フラグ
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
					//アクションを終了
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//相手がカウントしていた連続攻撃回数をリセット
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
				
					//エフェクト
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

		//斬撃エフェクトはプレイヤーに追従する
		if (actor_.lock()->IsPlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD))
		{
			Position3DF effPos = actor_.lock()->GetPos();
			effPos.x += 200.0f;
			actor_.lock()->SetEffectPos(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos);
		}

		//エフェクトの発生
		else if (actor_.lock()->animController_->GetStepAnimTime() >= 30.0f)
		{
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPos();
			effPos.x += 200.0f;
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.z += 90.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD, effPos, effRot);
		}

		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 160.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 60.0f));
		}
		//攻撃モーションが終了するまで処理が行われない
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			//自身のフレーム速度を元に戻す
			SetFrameDefaultSpeed();
			atackWeakCnt_ = 0;//攻撃回数をリセット
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
		//ラグがなく、前提条件が再び満たされた場合
		else if (timeCounter_.at(TIME::ATTACK_RAG) <= 0.0f)
		{
			//自身のフレーム速度を遅くする
			SetFrameSpeed(0.7f);
			//ほんの少しずつ前に進む
			actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 20.0f));
			//弱攻撃条件を満たした場合
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//強攻撃条件を満たした場合
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				//自身のフレーム速度を元に戻す
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
	//距離によりカメラモードを切り替える
	if (dis < std::powf(ActorAi::DISTANCE_MEDIUM, 2.7f) &&
		actor_.lock()->actorCam_->GetCameraMode() != Camera::MODE::SPECIAL_ATTACK_MODE)
	{
		//カメラモード
		actor_.lock()->actorCam_->ChangeMode(Camera::MODE::SPECIAL_ATTACK_MODE);
		actor_.lock()->actorCam_->SetSpecialCamera([&]() {actor_.lock()->StrongFinishCamera(); });
	}

	if (actor_.lock()->GetState() != ActorBase::STATE::STRONG_3)
	{
		//状態を強攻撃_3にする
		actor_.lock()->ChangeState(ActorBase::STATE::STRONG_3);
		SetTime(TIME::ATTACK_RAG, 1.0f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
		actor_.lock()->StopEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD);
		actor_.lock()->StopEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_2);
		actor_.lock()->StopEffect(EffectManager::EFF::SORD, ActorBase::USE_EFFECT::SORD_3);
	}
	else
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
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
					//当たり判定フラグ
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 50.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.8f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::SORD_FINISH, DX_PLAYTYPE_BACK);
					//アクションを終了
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0 &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_DEFENCE &&
						actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != ActorAi::INDEX_ACT_COMAND_AVOID)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
						//相手がカウントしていた連続攻撃回数をリセット
						actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();
					}
					
					//エフェクト
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
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 160.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 95.0f));
		}
		//攻撃モーションが終了するまで処理が行われない
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			actor_.lock()->actorCam_->ChangeDefaultMode();
			atackWeakCnt_ = 0;//攻撃回数をリセット
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			//アクションを終了
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
	//敵を一時停止しているか
	static bool enemyStop = true;

	if (actor_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL)
	{
		enemyStop = true;

		//状態を必殺技にする
		actor_.lock()->ChangeState(ActorBase::STATE::UNLEASH_SKILL);
		actor_.lock()->SetStopFrame(0.1f);
		SetFrameSpeed(0.5f);

		//エフェクト
		Position3DF effPos = actor_.lock()->transform_->pos;
		Rot3DF effRot = Rot3DF();
		actor_.lock()->PlayEffect(EffectManager::EFF::LIGHTNING, ActorBase::USE_EFFECT::LIGHTNING, effPos, effRot);

		//自動で相手の方を向く
		FactEnemy();

		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
		if (!actor_.lock()->GetCpuFlag())
		{
			//コントローラを振動させる
			StartJoypadVibration(actor_.lock()->GetPlayerId(), 1000, 1000, -1);
		}

		//必殺技音
		actor_.lock()->SoundPlay(SoundManager::SOD::SKILL_THUNDER, DX_PLAYTYPE_BACK);
	}
	else
	{
		//自身のフレーム速度
		if (actor_.lock()->animController_->GetStepAnimTime() >= 5.0f && enemyStop)
		{
			//フレーム速度をデフォルトに戻す
			SetFrameDefaultSpeed();
		}

		//相手の一時停止
		if (actor_.lock()->animController_->GetStepAnimTime() < 20.0f)
		{
			actor_.lock()->enemy_.lock()->SetStopFrame(3.0f);
		}
		else if(enemyStop)
		{
			enemyStop = false;
			actor_.lock()->enemy_.lock()->SetStopFrame(0.0f);
			//自動で相手の方を向く
			//一応の処理
			FactEnemy();
		}

		//当たり判定の発生
		//アニメーションのステップタイムで判断する
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
					//カメラを使用するか
					if (actor_.lock()->actorCam_->GetCameraMode() != Camera::MODE::SKILL_MODE)
					{
						//カメラモード
						actor_.lock()->actorCam_->ChangeMode(Camera::MODE::SKILL_MODE);
						actor_.lock()->actorCam_->SetSpecialCamera([&]() {actor_.lock()->SkillCamera();});
					}

					//当たり判定フラグ
					colliderFlag_ = true;

					//自身を相手の後ろに瞬間移動
					Position3DF pos = VAdd(actor_.lock()->enemy_.lock()->transform_->pos,
						VScale(actor_.lock()->enemy_.lock()->transform_->GetBackLocal(), 100.0f));
					actor_.lock()->transform_->pos = pos;

					actor_.lock()->enemy_.lock()->SetStopFrame(2.5f);

					//自身のフレーム速度
					SetFrameSpeed(0.2f);

					//相手がガード中なら強制解除
					if (actor_.lock()->enemy_.lock()->GetState() == ActorBase::STATE::DEFENCE)
					{
						actor_.lock()->enemy_.lock()->SetDefaultWeight();
						actor_.lock()->enemy_.lock()->StopEffect(EffectManager::EFF::BARIA, ActorBase::USE_EFFECT::BARIA);
						actor_.lock()->enemy_.lock()->userInterface_->SetDefence(0.0f);
						comandFlag_ = false;
					}
					
					//アクションを終了
					int nSearch = SearchNodeIdx(actor_.lock()->enemy_.lock()->actAi_->GetNowIndex());
					if (nSearch >= 0)
					{
						actor_.lock()->enemy_.lock()->actAi_->EndNodeAction(nSearch);
					}
					//相手がカウントしていた連続攻撃回数をリセット
					actor_.lock()->enemy_.lock()->actAi_->ResetAttackCount();

					//剣が当たった音
					actor_.lock()->SoundPlay(SoundManager::SOD::SORD_HIT, DX_PLAYTYPE_BACK);
					break;
				}
			}
		}

		//当たり判定があった場合、一定時間後に自身のフレーム速度を基に戻す
		if (colliderFlag_ && actor_.lock()->animController_->GetStepAnimTime() > 25.0f)
		{
			SetFrameDefaultSpeed();
		}

		//エフェクトの発生
		if (actor_.lock()->animController_->GetStepAnimTime() >= 60.0f &&
			!actor_.lock()->IsPlayEffect(EffectManager::EFF::SKILL_SORD, ActorBase::USE_EFFECT::SKILL_SORD))
		{
			if (colliderFlag_)
			{
				//ダメージをエフェクトのタイミングで与える
				float power = actor_.lock()->GetAttackPower();
				actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power * 7.5f);
				actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });
				actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 90.0f));
				actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, actor_.lock()->GetJumpPower() + 10.0f ,0.0f });
			}

			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPos();
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			effRot.y += MyUtility::Deg2RadF(180.0f);
			actor_.lock()->PlayEffect(EffectManager::EFF::SKILL_SORD, ActorBase::USE_EFFECT::SKILL_SORD, effPos, effRot);

			//雷音
			actor_.lock()->SoundPlay(SoundManager::SOD::THUNDER_BOLT, DX_PLAYTYPE_BACK);

			SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT + 70);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 160.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 180.0f));
		}
		//攻撃モーションが終了するまで処理が行われない
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			atackWeakCnt_ = 0;//攻撃回数をリセット
			atackStrongCnt_ = 0;
			comandFlag_ = false;
			actor_.lock()->actorCam_->ChangeDefaultMode();
			actor_.lock()->StopEffect(EffectManager::EFF::SKILL_SORD, ActorBase::USE_EFFECT::SKILL_SORD);
			//必殺技ゲージ・強化レベルを初期化する
			actor_.lock()->userInterface_->InitSkillandLevel();
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
}