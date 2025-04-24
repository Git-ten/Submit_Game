#pragma once
#include <string>
#include "../../ACTOR/ActorBase.h"
#include "../../UTILITY/VectorHeader.h"


class PlayerWeaponBase : public ActorBase
{
public:

	// コンストラクタ
	PlayerWeaponBase(int& modelId);
	// デストラクタ
	virtual ~PlayerWeaponBase(void)override;

	virtual void InitSpecial(void)override;

	// 攻撃力をセットアップ
	void SetUpAttackPower(float& power);

	// 当たり判定を適用
	void OnCollAttack(void);
	// 当たり判定を無効
	void OffCollAttack(void);
	// 現在の当たり判定の適用状態
	bool IsCollAttack(void);

	// 持ち主との同期
	void SyncroPlayerWeapon(const int modelId, const std::string& frameName);

protected:

	// ウェポンの所持者モデルのアドレス
	int& modelId_;

	ActorData actorData_;

	virtual void InitActorTransform(void)override;

	virtual void UpdateSpecial(void)override;
	virtual void DrawSpecial(void)override;

};