#pragma once
#include "StageBase.h"
class StageGround : public StageBase
{
public:

	// �R���X�g���N�^
	StageGround(void);
	// �f�X�g���N�^
	~StageGround(void)override;

	void InitSpecial(void)override;

	void Draw(void)override;

	// �R�[���o�b�N�֐�
	bool BattleStepCondition(void)override;

private:

	void InitActorTransform(void)override;

	void UpdateSpecial(void)override;

	void CollisionedGravityActor(Position3DF pos)override;

};