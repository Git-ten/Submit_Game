#include"../../../../Utility/MyUtility.h"
#include"Node/NodeBase.h"
#include"Node/ActionNode.h"
#include"../../ActorBase.h"
#include"../../Common/Userinterface.h"
#include"../../Common/LevelUpManager.h"
#include"../../Common/AnimationController.h"
#include"../../../Common/Collider.h"
#include"../../../Camera.h"
#include "MagicianAi.h"
#include<cmath>

MagicianAi::MagicianAi(std::weak_ptr<ActorBase> actor) : ActorAi(actor)
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

MagicianAi::~MagicianAi(void)
{
}

void MagicianAi::ActionComandWeek_1(void)
{
	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_1 &&
		actor_.lock()->userInterface_->GetMagicPower() >= MIN_MOVE_MAGIC)
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
	else if(actor_.lock()->GetState() == ActorBase::STATE::WEEK_1)
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
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageRightStagger(); });
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
					Position3DF effPos = actor_.lock()->enemy_.lock()->transform_->pos;
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
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			effPos.y = effPos.y + 50.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::LASER_MAGIC, ActorBase::USE_EFFECT::LASER_MAGIC, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//魔力を減らす
			actor_.lock()->userInterface_->SubMagicPower(10.0f);
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
				VScale(actor_.lock()->transform_->GetForwardLocal(), 5.0f));

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
	else if (actor_.lock()->GetState() == ActorBase::STATE::MISS ||
		actor_.lock()->userInterface_->GetMagicPower() < MIN_MOVE_MAGIC)
	{
		if (actor_.lock()->GetState() != ActorBase::STATE::MISS)
		{
			//状態を弱攻撃_1にする
			actor_.lock()->ChangeState(ActorBase::STATE::MISS);
		}
		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			//actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
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
	}
}

void MagicianAi::ActionComandWeek_2(void)
{
	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_2 &&
		actor_.lock()->userInterface_->GetMagicPower() >= MIN_MOVE_MAGIC)
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
	else if(actor_.lock()->GetState() == ActorBase::STATE::WEEK_2)
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
					Position3DF effPos = actor_.lock()->enemy_.lock()->transform_->pos;
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
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 240.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));
			//actor_.lock()->SetJumpVector({ 0.0f, 18.0f, 0.0f });
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			effPos.y = effPos.y + 50.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::LASER_MAGIC, ActorBase::USE_EFFECT::LASER_MAGIC, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//魔力を減らす
			actor_.lock()->userInterface_->SubMagicPower(10.0f);
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
	else if (actor_.lock()->GetState() == ActorBase::STATE::MISS ||
		actor_.lock()->userInterface_->GetMagicPower() < MIN_MOVE_MAGIC)
	{
		if (actor_.lock()->GetState() != ActorBase::STATE::MISS)
		{
			//状態を弱攻撃_1にする
			actor_.lock()->ChangeState(ActorBase::STATE::MISS);
		}

		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			//actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
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
	}
}

void MagicianAi::ActionComandWeek_3(void)
{
	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_3 &&
		actor_.lock()->userInterface_->GetMagicPower() >= MIN_MOVE_MAGIC)
	{
		//状態を弱攻撃_3にする
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_3);
		//ラグのセット
		SetTime(TIME::ATTACK_RAG, 0.25f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
	}
	else if(actor_.lock()->GetState() == ActorBase::STATE::WEEK_3)
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
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageRightStagger(); });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 80.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.05f);
					actor_.lock()->SetStopFrame(0.05f);
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
					Position3DF effPos = actor_.lock()->enemy_.lock()->transform_->pos;
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
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 240.0f));
			actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 70.0f));
			actor_.lock()->SetJumpVector({ 0.0f, 18.0f, 0.0f });

			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			effPos = actor_.lock()->GetPlayerForwardPos(800.0f);
			effPos.y = effPos.y + 50.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::LASER_MAGIC, ActorBase::USE_EFFECT::LASER_MAGIC, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//魔力を減らす
			actor_.lock()->userInterface_->SubMagicPower(10.0f);
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
	else if (actor_.lock()->GetState() == ActorBase::STATE::MISS ||
		actor_.lock()->userInterface_->GetMagicPower() < MIN_MOVE_MAGIC)
	{
		if (actor_.lock()->GetState() != ActorBase::STATE::MISS)
		{
			//状態を弱攻撃_1にする
			actor_.lock()->ChangeState(ActorBase::STATE::MISS);
		}

		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			//actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
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
	}
}

void MagicianAi::ActionComandWeek_4(void)
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
						  {10.0f, 25.0f},
						  {26.0f, 36.0f},
						  {37.0f, 47.0f},};


	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_4 &&
		actor_.lock()->userInterface_->GetMagicPower() >= MIN_MOVE_MAGIC)
	{
		canCollFlag = false;
		collCnt = 0;

		//状態を弱攻撃_5にする
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_4);
		//ラグのセット
		SetTime(TIME::ATTACK_RAG, 0.95f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
	}
	else if(actor_.lock()->GetState() == ActorBase::STATE::WEEK_4)
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


			SphereF sph = actor_.lock()->Model2Sphere(700);
			if (actor_.lock()->collider_->CollisionSphere2Cap(sph) && canCollFlag)
			{

				if (collCnt >= ANIM_TIME_MAX - 1)
				{
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBackStagger(); });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 20.0f));
					collCnt = 0;
					canCollFlag = false;

					//当たり判定フラグ
					colliderFlag_ = true;
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBackStagger(); });
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 80.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.05f);
					actor_.lock()->SetStopFrame(0.05f);
					actor_.lock()->camera_.lock()->SetPos(
						VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_STRONG, DX_PLAYTYPE_BACK);

					SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
				}
				else if (collCnt % 2 || collCnt == 0)
				{
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageRightStagger(); });
					collCnt++;
					canCollFlag = false;
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_STRONG, DX_PLAYTYPE_BACK);
				}
				else
				{
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageLeftStagger(); });
					collCnt++;
					canCollFlag = false;
					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_STRONG, DX_PLAYTYPE_BACK);
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
				Position3DF effPos = actor_.lock()->enemy_.lock()->transform_->pos;
				Rot3DF effRot = Quaternion::ToEuler(actor_.lock()->transform_->quaRotLocal);
				actor_.lock()->PlayEffect(EffectManager::EFF::FLASH, ActorBase::USE_EFFECT::FLASH, effPos, effRot);
				actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

				actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);
				actor_.lock()->enemy_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);
			}
		}

		//エフェクトの追従処理
		if (actor_.lock()->IsPlayEffect(EffectManager::EFF::LASER_VIOLENCE, ActorBase::USE_EFFECT::LASER_VIOLENCE))
		{
			Position3DF effPos = actor_.lock()->transform_->pos;
			effPos.y += 450.0f;
			actor_.lock()->SetEffectPos(EffectManager::EFF::LASER_VIOLENCE, ActorBase::USE_EFFECT::LASER_VIOLENCE,
				effPos);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 10.0f)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;

			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//	actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 160.0f));
			//	actor_.lock()->SetJumpVector({ 0.0f, 35.0f, 0.0f });

				//空振りエフェクト
				//空振りといっても攻撃が当たった場合でも再生される
				//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			effPos = actor_.lock()->transform_->pos;
			effPos.y += 450.0f;
			effRot = { 0.0f, 0.0f, 0.0f };
			actor_.lock()->PlayEffect(EffectManager::EFF::LASER_VIOLENCE, ActorBase::USE_EFFECT::LASER_VIOLENCE, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//魔力を減らす
			actor_.lock()->userInterface_->SubMagicPower(10.0f);
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
	else if (actor_.lock()->GetState() == ActorBase::STATE::MISS ||
		actor_.lock()->userInterface_->GetMagicPower() < MIN_MOVE_MAGIC)
	{
		if (actor_.lock()->GetState() != ActorBase::STATE::MISS)
		{
			//状態を弱攻撃_1にする
			actor_.lock()->ChangeState(ActorBase::STATE::MISS);
		}

		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
			actor_.lock()->PlayEffect(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA, effPos, effRot);
			//actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
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
	}
}

void MagicianAi::ActionComandWeek_5(void)
{
	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_5 &&
		actor_.lock()->userInterface_->GetMagicPower() >= MIN_MOVE_MAGIC)
	{
		//状態を弱攻撃_5にする
		actor_.lock()->ChangeState(ActorBase::STATE::WEEK_5);
		//ラグのセット
		SetTime(TIME::ATTACK_RAG, 5.45f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
	}
	else if(actor_.lock()->GetState() == ActorBase::STATE::WEEK_5)
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
		if (actor_.lock()->IsPlayEffect(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA) &&
			actor_.lock()->animController_->GetStepAnimTime() >= 30.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK &&
			actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != INDEX_CON_COMAND_COUNTERATTACK)
		{
			VERTEX3D vertex[3];
			//飛ばしたオーロラエフェクトの判定範囲(前方向)
			int area = 50;

			//飛ばしたオーロラエフェクトで当たり判定する
			for (int ii = -50; ii < area; ii++)
			{
				Position3DF auraPos = actor_.lock()->GetEffectPos(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA);
				Dir3DF auraDir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(auraPos, auraDir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					actor_.lock()->transform_->pos = auraPos;

					//攻撃ラグをなくす
					SetTime(TIME::ATTACK_RAG, 0.0f);

					//当たり判定フラグ
					colliderFlag_ = true;
					float power = actor_.lock()->GetAttackPower();
					actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
					actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBackStagger(); });
					actor_.lock()->enemy_.lock()->SetStopFrame(0.1f);
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
					Position3DF effPos = actor_.lock()->GetEffectPos(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA);
					Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), 50.0f);
					effPos = VAdd(effPos, dir);
					Rot3DF effRot = Quaternion::ToEuler(actor_.lock()->transform_->quaRotLocal);
					actor_.lock()->PlayEffect(EffectManager::EFF::FLASH, ActorBase::USE_EFFECT::FLASH, effPos, effRot);
					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

					actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);
					actor_.lock()->enemy_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);

					//オーロラエフェクトを消去
					actor_.lock()->StopEffect(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA);

					SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
					break;
				}
			}
		}

		if (actor_.lock()->animController_->GetStepAnimTime() > 15.0f)
		{
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			dir = VScale(dir, 20.0f);
			actor_.lock()->SetEffectPos(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA,
				VAdd(actor_.lock()->GetEffectPos(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA), dir));
		}

		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
			actor_.lock()->PlayEffect(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA, effPos, effRot);
			//actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//魔力を減らす
			actor_.lock()->userInterface_->SubMagicPower(10.0f);
		}
		//攻撃モーションが終了するまで処理が行われない
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			if (actor_.lock()->IsPlayEffect(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA))
			{
				//オーロラエフェクトを消去
				actor_.lock()->StopEffect(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA);
			}

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
			/*actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos,
				VScale(actor_.lock()->transform_->GetForwardLocal(), 10.0f));*/
				//弱攻撃条件を満たした場合
			if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_WEEK))]->IsCondition() != NONE_INDEX)
			{
				actor_.lock()->StopEffect(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA);
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_WEEKBRANCH);
				nowCost_ = NONE_COST;
			}
			//強攻撃条件を満たした場合
			else if (nodes_[SearchNodeIdx(INT_INDEX(INDEX_CON_COMAND_STRONG))]->IsCondition() != NONE_INDEX)
			{
				actor_.lock()->StopEffect(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA);
				//自身のフレーム速度を元に戻す
				SetFrameDefaultSpeed();
				nowIndex_ = INT_INDEX(INDEX_CON_COMAND_STRONGBRANCH);
				nowCost_ = NONE_COST;
			}
		}
	}
	else if (actor_.lock()->GetState() == ActorBase::STATE::MISS ||
		actor_.lock()->userInterface_->GetMagicPower() < MIN_MOVE_MAGIC)
	{
		if (actor_.lock()->GetState() != ActorBase::STATE::MISS)
		{
			//状態を弱攻撃_1にする
			actor_.lock()->ChangeState(ActorBase::STATE::MISS);
		}

		//オーロラエフェクトを消去
		actor_.lock()->StopEffect(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA);
		

		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
			//actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
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
	}
}

void MagicianAi::ActionComandWeek_6(void)
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

	if (actor_.lock()->GetState() != ActorBase::STATE::WEEK_6 &&
		actor_.lock()->userInterface_->GetMagicPower() >= MIN_MOVE_MAGIC)
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

		//攻撃行動による移動処理
		Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
		//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 320.0f));
		actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 50.0f));
	}
	else if(actor_.lock()->GetState() == ActorBase::STATE::WEEK_6)
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
		if (actor_.lock()->animController_->GetStepAnimTime() >= 40.0f && !colliderFlag_ &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::QUICK_MOVE &&
			actor_.lock()->enemy_.lock()->GetState() != ActorBase::STATE::COUNTER_ATTACK &&
			actor_.lock()->enemy_.lock()->actAi_->GetNowIndex() != INDEX_CON_COMAND_COUNTERATTACK)
		{
			SphereF sph = actor_.lock()->Model2Sphere(250);
			if (actor_.lock()->collider_->CollisionSphere2Cap(sph))
			{
				//当たり判定フラグ
				colliderFlag_ = true;
				float power = actor_.lock()->GetAttackPower();
				actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(power);
				actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });
				//ジャンプより50大きく敵を打ち上げる
				actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, actor_.lock()->GetJumpPower() + 5.0f ,0.0f });
				actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 90.0f));
				actor_.lock()->enemy_.lock()->SetStopFrame(0.5f);
				actor_.lock()->SetStopFrame(0.5f);
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
				Position3DF effPos = actor_.lock()->enemy_.lock()->transform_->pos;
				Rot3DF effRot = Quaternion::ToEuler(actor_.lock()->transform_->quaRotLocal);
				actor_.lock()->PlayEffect(EffectManager::EFF::FLASH, ActorBase::USE_EFFECT::FLASH, effPos, effRot);
				actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

				actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);
				actor_.lock()->enemy_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);

				SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
			}
		}


		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() > 20.0f)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			effPos = actor_.lock()->transform_->pos;
			effPos.y += 100.0f;
			actor_.lock()->PlayEffect(EffectManager::EFF::BIG_BANG, ActorBase::USE_EFFECT::BIG_BANG, effPos, effRot);
			//actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::SWING, DX_PLAYTYPE_BACK);
			//魔力を減らす
			actor_.lock()->userInterface_->SubMagicPower(10.0f);
		}
		//攻撃モーションが終了
		else if (!actor_.lock()->animController_->IsPlayAnim())
		{
			actor_.lock()->actorCam_->ChangeDefaultMode();
			//アクションを終了
			int nSearch = SearchNodeIdx(nowIndex_);
			if (nSearch >= 0)
			{
				EndNodeAction(nSearch);
			}
		}
	}
	else if (actor_.lock()->GetState() == ActorBase::STATE::MISS ||
		actor_.lock()->userInterface_->GetMagicPower() < MIN_MOVE_MAGIC)
	{
			actor_.lock()->actorCam_->ChangeDefaultMode();
		if (actor_.lock()->GetState() != ActorBase::STATE::MISS)
		{
			//状態を弱攻撃_1にする
			actor_.lock()->ChangeState(ActorBase::STATE::MISS);
		}

		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			effPos = actor_.lock()->GetPlayerForwardPos(100.0f);
			actor_.lock()->PlayEffect(EffectManager::EFF::AURA, ActorBase::USE_EFFECT::AURA, effPos, effRot);
			//actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
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
	}
}

void MagicianAi::ActionComandStrong_1(void)
{
	//二回以上連続をそれぞれ別々に判定するためのフラグ
	static bool canCollFlag = false;
	static int collCnt = 0;
	struct AnimTime
	{
		float start = 0.0f;
		float end = 0.0f;
	};
	static constexpr int ANIM_TIME_MAX = 4;
	static AnimTime animTime[ANIM_TIME_MAX]{
						  {20.0f, 30.0f},
						  {31.0f, 40.0f},
						  {32.0f, 37.0f}, 
						  {38.0f, 43.0f} };


	if (actor_.lock()->GetState() != ActorBase::STATE::STRONG_1 &&
		actor_.lock()->userInterface_->GetMagicPower() >= MIN_MOVE_MAGIC)
	{
		canCollFlag = false;
		collCnt = 0;

		//状態を強攻撃_1にする
		actor_.lock()->ChangeState(ActorBase::STATE::STRONG_1);
		SetTime(TIME::ATTACK_RAG, 0.65f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;

		//自動で相手の方を向く
		FactEnemy();
	}
	else if(actor_.lock()->GetState() == ActorBase::STATE::STRONG_1)
	{
		//当たり判定の発生
		//アニメーションのステップタイムで判断する
		if (actor_.lock()->animController_->GetStepAnimTime() >= 20.0f && !colliderFlag_ &&
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
				Dir3DF dir = VScale(actor_.lock()->transform_->GetForwardLocal(), static_cast<float>(ii));
				actor_.lock()->PoligonVecNormInfo(vertex, VAdd(actor_.lock()->transform_->pos, dir));
				if (actor_.lock()->collider_->CollisionTriangle2Model(vertex))
				{
					if (!canCollFlag)break;

					if (collCnt >= ANIM_TIME_MAX - 1)
					{
						collCnt = 0;
						canCollFlag = false;

						//当たり判定フラグ
						colliderFlag_ = true;
						actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });
						actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, 40.0f, 0.0f });
						actor_.lock()->enemy_.lock()->SetStopFrame(0.05f);
						actor_.lock()->SetStopFrame(0.05f);
						actor_.lock()->camera_.lock()->SetPos(
							VAdd(actor_.lock()->camera_.lock()->GetPos(), VScale(actor_.lock()->camera_.lock()->GetDown(), 360.0f)));
					
						SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT);
					}
					else if (collCnt % 2 || collCnt == 0)
					{
						actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageRightStagger(); });
						actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, 20.0f, 0.0f });
						collCnt++;
						canCollFlag = false;
					}
					else
					{
						actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageLeftStagger(); });
						actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, 20.0f, 0.0f });
						collCnt++;
						canCollFlag = false;
					}

					actor_.lock()->SoundPlay(SoundManager::SOD::ATTACK_STRONG, DX_PLAYTYPE_BACK);

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
					Position3DF effPos = actor_.lock()->enemy_.lock()->transform_->pos;
					Rot3DF effRot = Quaternion::ToEuler(actor_.lock()->transform_->quaRotLocal);
					actor_.lock()->PlayEffect(EffectManager::EFF::STRONG, ActorBase::USE_EFFECT::STRONG, effPos, effRot);
					actor_.lock()->enemy_.lock()->actAi_->SetDamageFlag(true);

					actor_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::ATTACK);
					actor_.lock()->enemy_.lock()->levelUpMng_->MeetCondision(LevelUpManager::LEVELUP_ID::DEFENCE);
					break;
				}
			}
		}

		//当たり判定発生の際エフェクトが相手に追従
		if (colliderFlag_)
		{
			Position3DF effPos = actor_.lock()->enemy_.lock()->transform_->pos;
			effPos.y = 0.0f;
			actor_.lock()->SetEffectPos(EffectManager::EFF::TRNADE, ActorBase::USE_EFFECT::TRNADE, effPos);
		}
		else if (actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			Position3DF effPos = VScale(actor_.lock()->transform_->GetForwardLocal(), actor_.lock()->GetAttackArea());
			effPos = VAdd(effPos, actor_.lock()->transform_->pos);
			effPos.y = 0.0f;
			actor_.lock()->SetEffectPos(EffectManager::EFF::TRNADE, ActorBase::USE_EFFECT::TRNADE, effPos);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 160.0f));
		//	actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 60.0f));
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			effPos = VScale(actor_.lock()->transform_->GetForwardLocal(), actor_.lock()->GetAttackArea());
			effRot = { 0.0f, 0.0f, 0.0f };
			actor_.lock()->PlayEffect(EffectManager::EFF::TRNADE, ActorBase::USE_EFFECT::TRNADE, effPos, effRot);
			//魔力を減らす
			actor_.lock()->userInterface_->SubMagicPower(10.0f);
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
	else if (actor_.lock()->GetState() == ActorBase::STATE::MISS ||
		actor_.lock()->userInterface_->GetMagicPower() < MIN_MOVE_MAGIC)
	{
		if (actor_.lock()->GetState() != ActorBase::STATE::MISS)
		{
			//状態を弱攻撃_1にする
			actor_.lock()->ChangeState(ActorBase::STATE::MISS);
		}

		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			//actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
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
	}
}

void MagicianAi::ActionComandStrong_2(void)
{
	if (actor_.lock()->GetState() != ActorBase::STATE::STRONG_2 &&
		actor_.lock()->userInterface_->GetMagicPower() >= MIN_MOVE_MAGIC)
	{
		//状態を強攻撃_2にする
		actor_.lock()->ChangeState(ActorBase::STATE::STRONG_2);
		SetTime(TIME::ATTACK_RAG, 0.6f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
	}
	else if(actor_.lock()->GetState() == ActorBase::STATE::STRONG_2)
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
					actor_.lock()->enemy_.lock()->SetStopFrame(0.2f);
					actor_.lock()->SetStopFrame(0.2f);
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
					Position3DF effPos = actor_.lock()->enemy_.lock()->transform_->pos;
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

		//当たり判定発生の際エフェクトが相手に追従
		if (colliderFlag_)
		{
			Position3DF effPos = actor_.lock()->enemy_.lock()->transform_->pos;
			effPos.y = 0.0f;
			actor_.lock()->SetEffectPos(EffectManager::EFF::LIGHTNING, ActorBase::USE_EFFECT::LIGHTNING, effPos);
		}
		else if (actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			Position3DF effPos = VScale(actor_.lock()->transform_->GetForwardLocal(), actor_.lock()->GetAttackArea());
			effPos = VAdd(effPos, actor_.lock()->transform_->pos);
			effPos.y = 0.0f;
			actor_.lock()->SetEffectPos(EffectManager::EFF::LIGHTNING, ActorBase::USE_EFFECT::LIGHTNING, effPos);
		}

		if (!comandFlag_ && actor_.lock()->animController_->GetStepAnimTime() >= 20.0f)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 160.0f));
		//	actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 60.0f));
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			actor_.lock()->PlayEffect(EffectManager::EFF::LIGHTNING, ActorBase::USE_EFFECT::LIGHTNING, effPos, effRot);
			//魔力を減らす
			actor_.lock()->userInterface_->SubMagicPower(10.0f);
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
	else if (actor_.lock()->GetState() == ActorBase::STATE::MISS ||
		actor_.lock()->userInterface_->GetMagicPower() < MIN_MOVE_MAGIC)
	{
		if (actor_.lock()->GetState() != ActorBase::STATE::MISS)
		{
			//状態を弱攻撃_1にする
			actor_.lock()->ChangeState(ActorBase::STATE::MISS);
		}

		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			//actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
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
	}
}

void MagicianAi::ActionComandStrong_3(void)
{
	float dis = VSquareSize(VSub(actor_.lock()->enemy_.lock()->transform_->pos,
		actor_.lock()->transform_->pos));
	//距離によりカメラモードを切り替える
	if (dis < std::powf(ActorAi::DISTANCE_MEDIUM, 3.7f) &&
		actor_.lock()->actorCam_->GetCameraMode() != Camera::MODE::SPECIAL_ATTACK_MODE)
	{
		//カメラモード
		actor_.lock()->actorCam_->ChangeMode(Camera::MODE::SPECIAL_ATTACK_MODE);
		actor_.lock()->actorCam_->SetSpecialCamera([&]() {actor_.lock()->StrongFinishCamera(); });
	}

	if (actor_.lock()->GetState() != ActorBase::STATE::STRONG_3 &&
		actor_.lock()->userInterface_->GetMagicPower() >= MIN_MOVE_MAGIC)
	{
		//状態を強攻撃_3にする
		actor_.lock()->ChangeState(ActorBase::STATE::STRONG_3);
		SetTime(TIME::ATTACK_RAG, 1.0f);
		//当たり判定フラグ
		colliderFlag_ = false;
		//攻撃フラグ
		comandFlag_ = false;
	}
	else if(actor_.lock()->GetState() == ActorBase::STATE::STRONG_3)
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
					actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 150.0f));
					actor_.lock()->enemy_.lock()->SetStopFrame(0.2f);
					actor_.lock()->SetStopFrame(0.2f);
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
					Position3DF effPos = actor_.lock()->enemy_.lock()->transform_->pos;
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

		//当たり判定発生の際エフェクトが相手に追従
		if (colliderFlag_)
		{
			Position3DF effPos = actor_.lock()->enemy_.lock()->transform_->pos;
			effPos.y = 0.0f;
			actor_.lock()->SetEffectPos(EffectManager::EFF::FHANTASM, ActorBase::USE_EFFECT::FHANTASM, effPos);
		}
		else if (actor_.lock()->animController_->GetStepAnimTime() >= 40.0f)
		{
			Position3DF effPos = VScale(actor_.lock()->transform_->GetForwardLocal(), actor_.lock()->GetAttackArea());
			effPos = VAdd(effPos, actor_.lock()->transform_->pos);
			effPos.y = 0.0f;
			actor_.lock()->SetEffectPos(EffectManager::EFF::FHANTASM, ActorBase::USE_EFFECT::FHANTASM, effPos);
		}


		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//攻撃行動による移動処理
			Dir3DF dir = actor_.lock()->transform_->GetForwardLocal();
			//actor_.lock()->transform_->pos = VAdd(actor_.lock()->transform_->pos, VScale(dir, 160.0f));
			//actor_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 70.0f));
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			effPos = actor_.lock()->GetPlayerForwardPos(400.0f);
			effRot = { 0.0f, 0.0f, 0.0f };
			actor_.lock()->PlayEffect(EffectManager::EFF::FHANTASM, ActorBase::USE_EFFECT::FHANTASM, effPos, effRot);
			//魔力を減らす
			actor_.lock()->userInterface_->SubMagicPower(10.0f);
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
	else if (actor_.lock()->GetState() == ActorBase::STATE::MISS ||
		actor_.lock()->userInterface_->GetMagicPower() < MIN_MOVE_MAGIC)
	{
		if (actor_.lock()->GetState() != ActorBase::STATE::MISS)
		{
			//状態を弱攻撃_1にする
			actor_.lock()->ChangeState(ActorBase::STATE::MISS);
			actor_.lock()->actorCam_->ChangeDefaultMode();
		}

		if (!comandFlag_)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
			Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
			actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			//actor_.lock()->PlayEffect(EffectManager::EFF::ATTACK_WIND, ActorBase::USE_EFFECT::ATTACK_WIND, effPos, effRot);
			actor_.lock()->SoundPlay(SoundManager::SOD::NOT_POSSIBLE, DX_PLAYTYPE_BACK);
		}
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
	}
}

void MagicianAi::ActionComandUnleashSkill(void)
{
	//敵を一時停止しているか
	static bool enemyStop = true;

	if (actor_.lock()->GetState() != ActorBase::STATE::UNLEASH_SKILL)
	{
		enemyStop = true;

		SceneManager::GetInstance().SetVibrationCnt(ATTACK_VIBRATION_CNT + 70);

		//状態を必殺技にする
		actor_.lock()->ChangeState(ActorBase::STATE::UNLEASH_SKILL);
		actor_.lock()->SetStopFrame(0.1f);
		SetFrameSpeed(0.5f);

		//エフェクト
		Position3DF effPos = actor_.lock()->transform_->pos;
		effPos = VAdd(effPos, VScale(actor_.lock()->transform_->GetForwardLocal(), 100.0f));
		effPos.y += 200.0f;
		Rot3DF effRot = Rot3DF();
		actor_.lock()->PlayEffect(EffectManager::EFF::SOGEKI, ActorBase::USE_EFFECT::SOGEKI, effPos, effRot);
		effPos = MyUtility::VECTOR_ZERO;
		actor_.lock()->PlayEffect(EffectManager::EFF::MAHOUJIN, ActorBase::USE_EFFECT::MAHOUJIN, effPos, effRot);

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
		actor_.lock()->SoundPlay(SoundManager::SOD::METEO, DX_PLAYTYPE_BACK);
		//スナイプ音
		actor_.lock()->SoundPlay(SoundManager::SOD::SNIPE, DX_PLAYTYPE_BACK);
	}
	else
	{
		//自身のフレーム速度
		if (actor_.lock()->animController_->GetStepAnimTime() >= 3.0f)
		{
			//フレーム速度をデフォルトに戻す
			SetFrameDefaultSpeed();
		}
		//相手の一時停止
		if (actor_.lock()->animController_->GetStepAnimTime() < 30.0f)
		{
			actor_.lock()->enemy_.lock()->SetStopFrame(4.0f);
		}
		else if (enemyStop)
		{
			enemyStop = false;
			actor_.lock()->enemy_.lock()->SetStopFrame(0.0f);
			//自動で相手の方を向く
			//一応の処理
			FactEnemy();
		}


		if (actor_.lock()->animController_->GetStepAnimTime() > 300.0f &&
			actor_.lock()->animController_->GetStepAnimTime() <= 310.0f)
		{
			//カメラを使用するか
			if (actor_.lock()->actorCam_->GetCameraMode() != Camera::MODE::SKILL_MODE)
			{
				//カメラモード
				actor_.lock()->actorCam_->ChangeMode(Camera::MODE::SKILL_MODE);
				actor_.lock()->actorCam_->SetSpecialCamera([&]() {actor_.lock()->SkillCamera();});
			}

			//プレイヤーを強制移動
			actor_.lock()->transform_->pos = { 0.0f, 0.0f, -2500.0f };
			actor_.lock()->transform_->Update();
			actor_.lock()->enemy_.lock()->transform_->pos = {0.0f, 0.0f, 500.0f};
			actor_.lock()->enemy_.lock()->transform_->Update();
			actor_.lock()->enemy_.lock()->SetStopFrame(100.0f);

			Position3DF effPos = {0.0f, 4000.0f, 0.0f};
			Rot3DF effRot = Rot3DF();
			actor_.lock()->PlayEffect(EffectManager::EFF::METEO, ActorBase::USE_EFFECT::METEO, effPos, effRot);
		}
		else if (actor_.lock()->animController_->GetStepAnimTime() >= 460.0f)
		{
			actor_.lock()->actorCam_->ChangeDefaultMode();
		}

		//隕石の落下
		if (actor_.lock()->IsPlayEffect(EffectManager::EFF::METEO, ActorBase::USE_EFFECT::METEO))
		{
			Position3DF pos = actor_.lock()->GetEffectPos(EffectManager::EFF::METEO, ActorBase::USE_EFFECT::METEO);
			pos.y -= 23.0f;
			if (pos.y < -1000.0f)
			{
				pos.y = -1000.0f;
			}
			actor_.lock()->SetEffectPos(EffectManager::EFF::METEO, ActorBase::USE_EFFECT::METEO, pos);
		}

		//当たり判定の発生(必ず当たる)
		//アニメーションのステップタイムで判断する
		if (actor_.lock()->animController_->GetStepAnimTime() > 665.0f)
		{
			if (!colliderFlag_)
			{
				colliderFlag_ = true;
				//敵は即死
				actor_.lock()->enemy_.lock()->userInterface_->AddDamagePower(1000.0f);
				actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });

				actor_.lock()->enemy_.lock()->SetStopFrame(0.8f);

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
				//エフェクト
				Position3DF effPos = actor_.lock()->enemy_.lock()->transform_->pos;
				Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
				actor_.lock()->PlayEffect(EffectManager::EFF::STRONG, ActorBase::USE_EFFECT::STRONG, effPos, effRot);
				//隕石ヒット
				actor_.lock()->SoundPlay(SoundManager::SOD::METEO_HIT, DX_PLAYTYPE_BACK);

				//音を止める
				actor_.lock()->SoundStop(SoundManager::SOD::METEO);
			}
		}

		if (actor_.lock()->animController_->GetStepAnimTime() > 665.0f)
		{
			if (colliderFlag_ && actor_.lock()->animController_->GetStepAnimTime() < 675.0f)
			{
				actor_.lock()->enemy_.lock()->SetDamageMotion([&]() {actor_.lock()->DamageBlowStagger(); });

				actor_.lock()->enemy_.lock()->SetSwayVector(VScale(actor_.lock()->transform_->GetForwardLocal(), 90.0f));
				actor_.lock()->enemy_.lock()->SetJumpVector({ 0.0f, 100.0f ,0.0f });
			}
		}

		if (actor_.lock()->animController_->GetStepAnimTime() >= 30.0f)
		{
			//デバッグ用の攻撃フラグ
			comandFlag_ = true;
			//空振りエフェクト
			//空振りといっても攻撃が当たった場合でも再生される
			//エフェクト
			if (!actor_.lock()->IsPlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING))
			{
				Position3DF effPos = actor_.lock()->GetPlayerForwardPos(200.0f);
				Rot3DF effRot = actor_.lock()->GetCollectionPlayerRot();
				actor_.lock()->PlayEffect(EffectManager::EFF::SWING, ActorBase::USE_EFFECT::SWING, effPos, effRot);
			}
		}
		//攻撃モーションが終了するまで処理が行われない
		if (!actor_.lock()->animController_->IsPlayAnim())
		{
			atackWeakCnt_ = 0;//攻撃回数をリセット
			atackStrongCnt_ = 0;
			comandFlag_ = false;
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