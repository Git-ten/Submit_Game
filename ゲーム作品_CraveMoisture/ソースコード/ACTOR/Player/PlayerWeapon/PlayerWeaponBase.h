#pragma once
#include <string>
#include "../../ACTOR/ActorBase.h"
#include "../../UTILITY/VectorHeader.h"


class PlayerWeaponBase : public ActorBase
{
public:

	// �R���X�g���N�^
	PlayerWeaponBase(int& modelId);
	// �f�X�g���N�^
	virtual ~PlayerWeaponBase(void)override;

	virtual void InitSpecial(void)override;

	// �U���͂��Z�b�g�A�b�v
	void SetUpAttackPower(float& power);

	// �����蔻���K�p
	void OnCollAttack(void);
	// �����蔻��𖳌�
	void OffCollAttack(void);
	// ���݂̓����蔻��̓K�p���
	bool IsCollAttack(void);

	// ������Ƃ̓���
	void SyncroPlayerWeapon(const int modelId, const std::string& frameName);

protected:

	// �E�F�|���̏����҃��f���̃A�h���X
	int& modelId_;

	ActorData actorData_;

	virtual void InitActorTransform(void)override;

	virtual void UpdateSpecial(void)override;
	virtual void DrawSpecial(void)override;

};