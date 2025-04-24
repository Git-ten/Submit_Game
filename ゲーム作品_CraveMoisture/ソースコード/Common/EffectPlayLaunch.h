#pragma once
#include <map>
#include "../Utility/VectorHeader.h"
#include "../Application/SINGLETON_MANAGER/EffectResourceManager/EffectResourceManager.h"
#include "DataLoad/DataEffect.h"

class EffectPlayLaunch
{
public:

	// コンストラクタ
	EffectPlayLaunch(void);
	// デストラクタ
	~EffectPlayLaunch(void);

	// 再生するエフェクトを登録
	void AddPlayEffect(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const int handleId);

	// プレイ
	void Play(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const DataEffect& data);

	// 追従状態で再生する(引数で参照させる)
	void FollowPlay(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const DataEffect& data,
		Position3DF& pos, Rot3DF* rot = nullptr);

	// 追従処理(FollowPlayで再生されているときのみ)
	void UpdateFollow(void);

	// ストップ
	void Stop(const EffectResourceManager::EFFECT_RESOURCE_ID& id);
	// 再生中のエフェクトをすべて停止
	void AllStop(void);

	// 再生状態を取得
	const bool IsPlayEffect(const EffectResourceManager::EFFECT_RESOURCE_ID& id);

	// 座標を引数方向に動かす
	void MoveEffect(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const Dir3DF& dir, const float& distance);

	// プレイIDを取得
	const int GetPlayId(const EffectResourceManager::EFFECT_RESOURCE_ID& id)const;

	// 座標の取得
	const Position3DF& GetPos(const EffectResourceManager::EFFECT_RESOURCE_ID& id);

	// 再生速度を取得
	const float GetSpeed(const EffectResourceManager::EFFECT_RESOURCE_ID& id);
	// スケールの取得
	const Size3DF& GetScl(const EffectResourceManager::EFFECT_RESOURCE_ID& id);

	// 座標をセット
	void SetPos(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const Position3DF& pos);
	// 回転をセット
	void SetRot(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const Rot3DF& rot);
	// スケールをセット
	void SetScl(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const Size3DF& scl);
	// 再生速度をセット
	void SetSpeed(const EffectResourceManager::EFFECT_RESOURCE_ID& id, const float speed);

private:

	using RESOURCE_EFFECT = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, int>;
	using PLAY_EFFECT = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, int>;
	using EFFECT_POS = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, Position3DF>;
	using EFFECT_SPEED = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, float>;
	using EFFECT_SCL = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, Size3DF>;
	using FOLLOW_EFFECT_POS = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, Position3DF*>;
	using FOLLOW_EFFECT_ROT = std::map<EffectResourceManager::EFFECT_RESOURCE_ID, Rot3DF*>;

	// エフェクトの情報
	struct EFFECT_INFO
	{
		// リソース
		RESOURCE_EFFECT resourceEffects;
		// プレイハンドル
		PLAY_EFFECT playEffects;

		// 現在座標
		EFFECT_POS pos;

		// 現在の再生速度
		EFFECT_SPEED speed;

		// 現在のスケール
		EFFECT_SCL scl;

		// 追従用::追従座標の参照
		FOLLOW_EFFECT_POS followPos;

		FOLLOW_EFFECT_ROT followRot;
	};

private:

	// エフェクト
	EFFECT_INFO effect_;

	// エフェクトの登録チェック
	void CheckPlayEffect(const EffectResourceManager::EFFECT_RESOURCE_ID& id);

};