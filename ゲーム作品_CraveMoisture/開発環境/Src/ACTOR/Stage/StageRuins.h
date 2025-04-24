#pragma once
#include "EntranceObject.h"
#include "StageBase.h"

class StageRuins : public StageBase
{
public:

	// �R���X�g���N�^
	StageRuins(void);
	// �f�X�g���N�^
	~StageRuins(void)override;

	void InitSpecial(void)override;

	void Draw(void)override;

	// �R�[���o�b�N�֐�
	bool BattleStepCondition(void)override;

private:

	std::unique_ptr<EntranceObject>entrance_;

	void SetOtherActorCollisionSpecial(ACTOR_COLLISION* actorColl)override;

	void InitActorTransform(void)override;

	void UpdateSpecial(void)override;

	void CollisionedGravityActor(Position3DF pos)override;

};