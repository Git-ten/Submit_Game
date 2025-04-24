#pragma once
#include<memory>
#include<map>
#include<string>
#include"Sound.h"

class SoundManager
{
public:
	
	//リソース名
	enum class SOD
	{
		TITLE_BGM,//タイトルBGM
		BATTLE_BGM,//バトルBGM
		SELECT_BGM,//セレクトBGM
		VICTORY,//リザルト音
		ATTACK_CRASH,//クラッシュ攻撃音
		ATTACK_STRONG,//強打撃
		SWING,//空振り音
		SORD_FINISH,//抜刀
		SORD_HIT,//剣が当たった音
		THUNDER_BOLT,//雷音
		SKILL_FLAME,//炎必殺技音
		SKILL_THUNDER,//雷必殺技
		SKILL_MAX,//必殺技ゲージマックス
		DECISION,//決定音
		DECISION_2,//決定音2
		DECISION_3,//決定音3
		CHANGE_SCENE,//シーン切り替え音
		JUST,//ジャスト音
		FINISH,//勝負あり
		METEO,//隕石音
		METEO_HIT,//隕石ヒット音
		SNIPE,//スナイプ音
		GURD_BREAK,//ガードブレイク
		QUICK_MOVE,//高速移動
		NOT_POSSIBLE,//不発
		COUNT_DOWN,//カウントダウン
		START,//スタート音
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static SoundManager& GetInstance(void);

	//初期化
	void Init(void);
	// 解放(シーン切替時に一旦解放)
	void Release(void);
	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	std::weak_ptr<Sound> Load(const SOD& src);

private:

	using SoundsData = std::map<SOD, Sound>;
	using LoadedData = std::map<SOD, std::shared_ptr<Sound>>;


	//静的インスタンス
	static SoundManager* instance_;

	//リソース管理の対象
	SoundsData soundsMap_;

	//読み込み済みリソース
	LoadedData loadedMap_;



	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SoundManager(void);
	//コピーコンストラクタも同様
	SoundManager(const SoundManager&);
	// デストラクタも同様
	~SoundManager(void) = default;

	// 内部ロード
	std::weak_ptr<Sound> _Load(const SOD& sod);

};

