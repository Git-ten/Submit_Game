#pragma once
#include<memory>
#include<vector>
#include<map>

class Effect;

class EffectManager
{
public:

	//エフェクトの種類
	enum class EFF
	{
		CARSOL_EFF,//強化エフェクト
		JUMP,//ジャンプの土煙
		SWING,//空振りエフェクト
		STRONG,//強エフェクト
		ATTACK_WIND,//攻撃風
		FLASH,//;弱攻撃エフェクト
		SORD,//斬撃エフェクト
		FALL_ATTACK,//落下攻撃
		BARIA,//防御エフェクト
		QUICK_MOVE,//高速移動
		JUST,//ジャストエフェクト
		SKILL_MAX,//必殺技MAXエフェクト
		SKILL_FLAME,//炎必殺技
		SKILL_SORD,//ソード必殺ビーム
		LIGHTNING,//落雷
		LASER_FIRE,//炎レーザー
		BLUR,//ブラー
		FIRE_CIRCLE,//戦闘開始エフェクト
		LASER_MAGIC,//魔術レーザー
		LASER_VIOLENCE,//魔術レーザー(乱)
		AURA,//ワープオーラキューブ
		BIG_BANG,//ビックバン
		TRNADE,//竜巻
		FHANTASM,//幻影エフェクト
		SOGEKI,//狙撃魔導陣
		METEO,//隕石
		MAHOUJIN,//隕石魔導陣
		GURD_BREAK,//ガードブレーク
		EFF_MAX,//エフェクトIDの最大値
	};


	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static EffectManager& GetInstance(void);

	//初期化
	void Init(void);
	//更新
	void Update(void);
	//2Dエフェクトの描画
	void Draw2DEffect(void);
	//3Dエフェクトの描画
	void Draw3DEffect(void);
	//メモリ開放
	void Release(void);
	//リソースの完全開放
	void Destroy(void);

	//読み込み
	std::weak_ptr<Effect> Load(const EFF& eff);

private:

	using  ResourcesData = std::map<EFF, Effect>;
	using  LoadedData = std::map<EFF, std::vector<std::shared_ptr<Effect>>>;

	//シングルトンインスタンス
	static EffectManager* instance_;

	//リソース連想配列
	ResourcesData resourcesMap_;
	//ロード
	LoadedData loadedMap_;



	//内部でのみ使用する関数
	//外部からインストラクタを生成できないように
	//ここでコンストラクタを宣言する
	EffectManager(void);
	// コピーコンストラクタも同様
	EffectManager(const EffectManager&);
	//デストラクタ
	~EffectManager(void) = default;

	//内部的ロード
	std::weak_ptr<Effect> _Load(const EFF& eff);

};