#pragma once
#include "../AI/EnemyAIBase.h"
#include "EnemyState.h"
#include "EnemyData.h"
#include "../ActorBase.h"

class EnemyBase : public ActorBase
{
public:

	static constexpr float ENEMT_ATTACK_RAG = 0.6f;

	// �A�j���[�V�����X�e�b�v�̋�؂�̊���
	static constexpr float ENEMY_ANIME_DIGIT_MOST_MIN = 1.1f;
	static constexpr float ENEMY_ANIME_DIGIT_MIDLE = 1.5f;
	static constexpr float ENEMY_ANIME_DIGIT_MAX = 2.0f;

	// �R���X�g���N�^
	EnemyBase(void);

	// �f�X�g���N�^
	~EnemyBase(void)override;

	virtual void InitSpecial(void)override;

	float NowHp(void)override;

protected:

	// �G�t�F�N�g�̎g�p�p�r
	// int�^�ɕϊ����Ďg�p����
	enum class EFFECT_USES
	{
		// �U�������G�t�F�N�g
		ATTACK_READY,

		// �U���G�t�F�N�g
		NORMAL_ATTACK,

		// �U���G�t�F�N�g(��)
		ATTACK_BOMB,

		// �U���G�t�F�N�g(�y��)
		ATTACK_SMOKE,

		// �o��(�G�t�F�N�g)
		AWAKE_1,
		AWAKE_2,
		STAGE_EFFECT,
		AWAKE_ATTACK,

		// �_���[�W�G�t�F�N�g
		DAMAGE_SPARK,
	};

	// �U�������蔻�肷�镔��(�K���A�ォ��G�l�~�[�f�[�^��collBones�̏��Ԃɂ���)
	enum class ATTACK_BONE
	{
		RIGHT_HAND,
		LEFT_HAND,
		HEAD,
	};

	// �G�l�~�[���_�E����ԂɂȂ����
	const float DOWN_TIME = 7.0f;

	// ���x���ɉ����ċ����Ȃ�
	float enemyLevel_;

	EnemyState::STATE enemyState_;

	EnemyData enemyData_;

	// �C�⎞�̃^�C�}�[
	Time enemyDownTime_;

	// �G�l�~�[��AI
	std::unique_ptr<EnemyAIBase>enemyAi_;

	// �������֐�
	virtual void InitActorTransform(void)override;
	virtual void InitActorAnimation(void)override;
	virtual void InitActorUi(void)override;
	void InitActorEffectInfo(void)override;
	void InitActorCameraControl(void)override;
	void InitActorSoundLaunch(void)override;

	void InitDamageEffect(void);
	void InitSpecialSound(void);

	// �����蔻�莞�̏����֐�
	void CollisionedHorizontalActor(void)override;
	virtual void CollisionedGravityActor(Position3DF pos)override;
	void CollisionedDamageActor(float* damagePower)override;

	void SetOtherActorCollisionSpecial(ACTOR_COLLISION* actorColl)override;

	virtual void UpdateSpecial(void)override;

	// �G�l�~�[�̃p�����[�^���Z�b�e�B���O
	void SetUpEnemyPalam(void);
	void SetUpEnemyLevelParam(void);

	// �A�N�V�����̏I������
	const bool IsEndAction(void)const;

	// �U������
	virtual void EnemyAttack(void);

	// �ړ�
	void EnemyMove(void);

	void Flinch(void);
	void Death(void);
	void Down(void);

	// ����A�j���[�V�����̏���
	void SpecialStateAnime(void);
	virtual void SpecialStateAnimeAdd(void);

	// �����蔻��p�����Z�b�e�B���O����
	void SetUpCollSphere(const ATTACK_BONE& bone, bool isMyBody = false);
	void SetUpCollEffectSphere(EffectResourceManager::EFFECT_RESOURCE_ID effectId);

	virtual void DrawSpecial(void)override;

};