#pragma once
#include <memory>
#include "EnemyState.h"
#include "EnemyBase.h"

class EnemyBear : public EnemyBase
{
public:

	// �R���X�g���N�^
	EnemyBear(void);
	// �f�X�g���N�^
	~EnemyBear(void)override;

	void InitSpecial(void)override;

	// �R�[���o�b�N�֐�
	bool BattleStepCondition(void)override;

private:
	// �G�t�F�N�g�̎g�p�p�r
	// int�^�ɕϊ����Ďg�p����
	enum class EFFECT_USES
	{
		// �U���G�t�F�N�g
		NORMAL_ATTACK,
	};

	// ������
	void InitActorTransform(void)override;
	void InitActorEffectLaunch(void)override;
	void InitActorAnimation(void)override;

	void UpdateSpecial(void)override;

	// �U��
	void EnemyAttack(void)override;

	void DrawSpecial(void)override;

};