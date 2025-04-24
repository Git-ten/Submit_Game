#pragma once
#include <map>
#include "EffectResource.h"

class EffectResourceManager
{
public:

	// エフェクトリソースのID
	enum class EFFECT_RESOURCE_ID
	{
		SMOKE,						// 土煙エフェクト
		BLACK_SMOKE,				// 黒煙エフェクト
		SLASH_RIGHT,				// 剣のエフェクト(右)
		SLASH_LEFT,					// 剣のエフェクト(左)
		SLASH_HORIZ,				// 剣のエフェクト(横)
		SLASH_DUO,					// 剣のエフェクト(双)
		SLASH_UP,					// 剣のエフェクト(上)
		SLASH_TORIO,				// 剣のエフェクト(三重奏)
		SLASH_DEVIATION,			// 剣のエフェクト(乱)
		SLASH_ACCEL,				// 剣のエフェクト(アクセル)
		SLASH_FLASH,				// 剣のエフェクト(一閃)
		
		ENEMY_ATTACK_READY,			// 「イビル・ゴア」の攻撃準備エフェクト
		EVIL_GOA_CLAW,				// 「イビル・ゴア」の爪エフェクト
		EVIL_GOA_CLAW_RIGHT,		// 「イビル・ゴア」の爪エフェクト(右)
		EVIL_GOA_CLAW_LEFT,			// 「イビル・ゴア」の爪エフェクト(左)
		EVIL_GOA_CLAW_DUO,			// 「イビル・ゴア」の爪エフェクト(双)
		EVIL_GOA_CLAW_DESTROY,		// 「イビル・ゴア」の爪エフェクト(殺)
		BOMB,						// 「イビル・ゴア」の爆破エフェクト
		EVIL_GOA_AWAKE_1,			// 「イビル・ゴア」の覚醒エフェクト_1
		EVIL_GOA_AWAKE_2,			// 「イビル・ゴア」の覚醒エフェクト_2
		EVIL_GOA_AWAKE_ATTACK_1,	// 「イビル・ゴア」の覚醒攻撃エフェクト_1
		EVIL_GOA_AWAKE_ATTACK_2,	// 「イビル・ゴア」の覚醒攻撃エフェクト_2

		SPARK,						// 火花エフェクト
		SONIC_BOOM,					// ソニックブーム
		LIGHTNING,					// 稲妻
		THUNDER_BOLT,				// サンダーボルト(乱)
		THUNDER_BOLT_ONE,			// サンダーボルト(迅)
		STAGE_FIRE,					// 燃えるステージエフェクト
		CHARGE,						// チャージ中エフェクト
		CHARGE_FINISH,				// チャージ中エフェクトの終了エフェクト
		REPAIR,						// テクニック技::回復エフェクト
		PHANTOM_BULLET,				// 幻影弾丸
		DARK_BULLET,				// 暗黒弾丸

		PARYS,						// パリィエフェクト

		DAMAGE_SPARK,				// ダメージエフェクト
		PLAYER_DAMAGE_SPARK,		// プレイヤー用のダメージエフェクト

		// 新規エフェクト
		HIGHT_POLI_EXPLOSION,		// ハイポリゴン爆発エフェクト
		LOW_POLI_EXPLOSION,			// ロウポリゴン爆発エフェクト


		PLAYER_BULLET,			    // プレイヤーの銃弾
	};

	//インスタンスの生成
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static EffectResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	//更新
	void Update(void);
	//2Dエフェクトの描画
	void Draw2DEffect(void);
	//3Dエフェクトの描画
	void Draw3DEffect(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);
	// リソースの完全破棄
	void Destroy(void);

	// ロードして、ハンドルIDを取得
	EffectResource Load(const EFFECT_RESOURCE_ID& id);
	
	// ロードしたエフェクトの再生タイプ(EFFECT_TYPE)を取得
	const EffectResource::EFFECT_TYPE GetEffectType(const EFFECT_RESOURCE_ID& id)const;

	// NULLであるか
	static const bool IsNullptr(void);

private:

	// リソースデータ
	using ResourcesData = std::map<EFFECT_RESOURCE_ID, EffectResource>;
	// ロード済みデータ
	using LoadedData = std::map<EFFECT_RESOURCE_ID, EffectResource>;

private:

	// 静的インスタンス
	static EffectResourceManager* instance_;

	// リソース管理の対象
	ResourcesData resourcesMap_;

	// 読み込み済みリソース
	LoadedData loadedMap_;

	// privateでコンストラクタを生成する
	EffectResourceManager(void);
	// コピーコンストラクタも同様
	EffectResourceManager(const EffectResourceManager&);
	//デストラクタ
	~EffectResourceManager(void) = default;


	// 内部ロード
	EffectResource _Load(const EFFECT_RESOURCE_ID& id);

};

