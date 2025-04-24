#pragma once
#include "../../Scene/GUI/Image.h"
#include "../../Scene/GUI/ImageDiv.h"
#include "PlayerData.h"
#include <functional>
#include "PlayerControle.h"
#include "PlayerState.h"
#include "PlayerWeapon/PlayerWeaponSword.h"
#include "../ActorBase.h"

class Player : public ActorBase
{
public:

	Player(void);
	~Player(void)override;

	void InitSpecial(void)override;

	// �o�g���X�^�[�g����(�K�p���邩�͊e�X�̌p����Ō��߂�)
	void ActorBattleStart(void)override;

	// �R�[���o�b�N�֐�
	bool BattleStepCondition(void)override;

	bool IsDash(void)override;
	bool IsCharge(void)override;
	bool IsTechnique(void)override;
	bool IsParysOrJustAvoid(void)override;
	bool IsSkill(void)override;

private:

	// �G�t�F�N�g�̎g�p�p�r
	// int�^�ɕϊ����Ďg�p����
	enum class EFFECT_USES
	{
		MOVE,
		ACTION_SMOKE,
		DASH_SONIC,

		ATTACK_SLASH,
		ATTACK_SLASH_FLASH,
		ATTACK_SLASH_ACCEL,
		PARYS,
		SPARK,
		ATTACK_THUNDER,
		ATTACK_LIGHTNING,
		FINISH_SKILL_THUNDER,
		CHARGE,
		REPAIR,

		DAMAGE_SPARK,
	};

	struct DataTechnique
	{
		float recastTimeMax = 20.0f;	// ���L���X�g�^�C���̍ő�l
		Time techniqueTimer;// ���L���X�g�^�C��
		bool isCanTechnique = true;
	};

	// �v���C���[�̑���
	std::unique_ptr<PlayerControle>playerControle_;

	PlayerState::STATE playerState_;

	PlayerData playerData_;

	// �e�N�j�b�N�Z�̃f�[�^
	DataTechnique playerTechnique_;

	// ����
	std::unique_ptr<PlayerWeaponSword>playerWeapon_;

	// �v���C���[�̍U�����O
	float playerAttackRag_;

	// �A�C�R���`��p::���L���X�g�^�C���p�̐����摜
	std::unique_ptr<ImageDiv>imgTimesBig_;
	std::unique_ptr<ImageDiv>imgTimesSmall_;
	// �񌅖ڂ͍��W�����󂯎��
	Position2D timesBig_Digit_10_Pos_;
	Position2D timesSmall_Point_10_Pos_;

	// �f�t�H���g�̍U����
	float defAttackPower_;

	// �X�L�����g�p�\��
	bool isSkill_;

	// �X���[���[�V�����̃^�C�}�[
	Time slowTime_;
	float slowTimeMax_;

	// ���G��Ԃ�
	bool isInvincible_;

	bool isGravity_;// �d�͂�K�p���邩
	bool isInertia_;// ������K�p���邩

	// ���ԂƑ��_���[�W���v������
	Time nowGameTime_;
	float nowSumDamage_;

	// �������֐�
	void InitActorTransform(void)override;
	void InitActorAnimation(void)override;
	void InitActorUi(void)override;
	void InitActorEffectLaunch(void)override;
	void InitActorEffectInfo(void)override;
	void InitActorCameraControl(void)override;
	void InitActorSoundLaunch(void)override;

	// �����蔻�莞�̏���
	void CollisionedGravityActor(Position3DF pos)override;
	void CollisionedHorizontalActor(void)override;
	void CollisionedDamageActor(float* damagePower)override;

	void UpdateSpecial(void)override;


	// �X���[���[�V�����̃Z�b�g(�������Ԃ����X���[�ɂȂ�)
	void SetSlowTime(const float time);
	// �X���[���[�V�����̍X�V
	void UpdateSlowTime(void);


	// �ړ��A�A�N�V����
	void Walk(void);

	void Move(void);
	
	void Attack(void);
	void Attack_1(bool& isLerpCamera);
	void Attack_2(bool& isLerpCamera);
	void Attack_3(bool& isLerpCamera);
	void Attack_4(bool& isLerpCamera);
	void Attack_5(bool& isLerpCamera);
	void Attack_6(bool& isLerpCamera);
	void Attack_NormalCharge(bool& isLerpCamera);
	void NormalChargeRun(bool& isLerpCamera);
	void ForwardChargeHorizontalChargeRun(bool& isLerpCamera);

	void Attack_ForwardCharge(bool& isLerpCamera);
	void Attack_HorizontalCharge(bool& isLerpCamera);
	void Attack_Sky_1(bool& isLerpCamera);
	void Attack_Sky_2(bool& isLerpCamera);
	void Attack_Fall(bool& isLerpCamera);


	void Jump(void);
	void Avoid(void);
	void Technique(void);
	void Skill(void);
	void Flinch(void);
	void Death(void);

	// ����̃G�t�F�N�g�ƃT�E���h��C�ӂŃX�g�b�v
	void StopAttackEffect(void)override;
	void StopWaveSound(void)override;

	// �G�l�~�[�ւ̋������b�N�I��
	// �d�l::�U�����Ɏ����ōł��߂��G�l�~�[�����b�N�I������
	void LockOnEnemy(bool isLerp = false);

	// ��Ԃ��Ƃ̏I������
	void ControleEnd(void);
	void EndTechnique(void);
	void EndSkill(void);
	void EndAvoid(void);
	void EndMove(void);
	void EndAttack(void);
	void EndJump(void);
	void EndFlinch(void);
	void EndWalk(void);

	// �X�L���Q�[�W���`���[�W
	void ChagePlayerSkill(float mul = 1.0f);

	// �v���C���[��HP����
	void RepairPlayerHp(void);

	void SetUpInertia(Dir3DF dir, float power);

	// �d�͂Ɗ����𖳌���
	void EndGravity(void);
	void EndInertia(void);

	void DrawSpecial(void)override;

	// �A�C�R���`��p::�c��̃��L���X�g�^�C����`��
	void DrawIconRecastTime(void)override;

};