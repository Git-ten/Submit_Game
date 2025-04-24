#pragma once
#include <memory>
#include "EnemyBase.h"

class EnemyDemon : public EnemyBase
{
public:

	// �R���X�g���N�^
	EnemyDemon(void);
	// �f�X�g���N�^
	~EnemyDemon(void)override;

	void InitSpecial(void)override;

	// �o�g���X�^�[�g����(�K�p���邩�͊e�X�̌p����Ō��߂�)
	void ActorBattleStart(void)override;

	// �R�[���o�b�N�֐�
	bool BattleStepCondition(void)override;

private:
	
	// �o����ԂɂȂ��Ă��邩
	bool isDemonAwake_;

	// �������֐�
	void InitActorTransform(void)override;
	void InitActorAnimation(void)override;
	void InitActorEffectLaunch(void)override;
	void InitActorSoundLaunch(void)override;

	void UpdateSpecial(void)override;

	
	// �U��
	void EnemyAttack(void)override;
	void EnemyAttack_1(void);
	void EnemyAttack_2(void);
	void EnemyAttack_3(void);
	void EnemyAttack_4(void);
	void EnemyAttack_5(void);
	void EnemyAttack_6(void);
	void EnemyAwakeAttack(void);

	// �o��
	void EnemyAwake(void);

	void SpecialStateAnimeAdd(void)override;

	// �~�`�ɍL����Ȃ���A�����蔻�苅�𓮂����Ă���
	void SetUpCollEffectSphereDotDir(EffectResourceManager::EFFECT_RESOURCE_ID effectId);

	void HappenNormalClowEffect(EffectResourceManager::EFFECT_RESOURCE_ID effectId);
	void StopAttackEffect(void)override;

	void DrawSpecial(void)override;

};