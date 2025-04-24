#pragma once
#include<memory>
#include <map>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		NONE,
		TITLE,//タイトル画像
		PLAYGAME,//PlayGame
		PLAYGAME_IN,//PlayGame_Input
		ABOUTGAME,//AboutGame
		ABOUTGAME_IN,//AboutGameInput
		RESULT,//リザルト画像
		EXPLAIN_1,//ゲーム説明画像_1
		EXPLAIN_2,//ゲーム説明画像_2
		EXPLAIN_3,//ゲーム説明画像_3
		EXPLAIN_4,//ゲーム説明画像_4
		EXPLAIN_4_2,//ゲーム説明画像_4_2
		EXPLAIN_4_3,//ゲーム説明画像_4_3
		EXPLAIN_5,//ゲーム説明画像_5
		EXPLAIN_5_2,//ゲーム説明画像_5_2
		EXPLAIN_5_3,//ゲーム説明画像_5_3
		EXPLAIN_6,//ゲーム説明画像_6
		EXPLAIN_6_2,//ゲーム説明画像_6_2
		EXPLAIN_6_3,//ゲーム説明画像_6_3
		EXPLAIN_6_4,//ゲーム説明画像_6_4
		EXPLAIN_EX,//フェイド時のゲーム説明
		MENYU,//メニュー画像
		LEFT_KEY,//左ボタン
		RIGHT_KEY,//右ボタン
		MARK_1,//プレイヤー1の印
		MARK_2,//プレイヤー2の印
		PLAYER_FIST,//プレイヤー(格闘家)
		PLAYER_SORD,//プレイヤー(剣士)
		PLAYER_MAGIC,//プレイヤー(魔術師)
		STAGE,//ステージ
		SKY,//空画像
		SKYDOME_1,//スカイドーム

		//UI
		HP_GAGE,//HPゲージ
		B_HP_GAGE,//HPゲージ(Big)
		STAMINA_GAGE,//スタミナゲージ
		B_STAMINA_GAGE,//スタミナゲージ(Big)
		CASE,//ケース
		SKILL_GAGE,//必殺技ゲージ
		B_SKILL_GAGE,//必殺技ゲージ(Big)
		SKILL_CASE,//必殺技ゲージケース
		LEVEL_GAGE,//レベルゲージ
		LEVEL_ATTACK_GAGE,//攻撃力レベルゲージ
		B_LEVEL_ATTACK_GAGE,//攻撃力レベルゲージ(Big)
		LEVEL_DEFENCE_GAGE,//防御力レベルゲージ
		B_LEVEL_DEFENCE_GAGE,//防御力レベルゲージ(Big)
		LEVEL_AVOID_GAGE,//回避力レベルゲージ
		B_LEVEL_AVOID_GAGE,//回避力レベルゲージ(Big)
		MAGIC_GAGE,//魔力ゲージ
		B_MAGIC_GAGE,//魔力ゲージ(Big)
		SELECT_CARSOL,//セレクトカーソル画像
		SELECT,//セレクト画面画像
		START,//開始ボタン画像
		START_INPUT,//開始ボタン入力画像
		CHAIN,//チェーン画像
		RETURN_BUTTON,//戻るボタン
		RETURN_BUTTON_INPUT,//戻るボタンインプット
		RETURN_BUTTGAGE,//戻るボタンゲージ
		B_RETURN_BUTTGAGE,//戻るボタンゲージ(Big)
		SWITCH,//電源ボタン
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);
	// 解放(シーン切替時に一旦解放)
	void Release(void);
	//部分開放
	void Release(const SRC& src, int handleId);
	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	std::weak_ptr<Resource> Load(const SRC& src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(const SRC& src);

private:

	using ResourcesData = std::map<SRC, Resource>;
	using LoadedData = std::map<SRC, std::shared_ptr<Resource>>;


	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	ResourcesData resourcesMap_;

	// 読み込み済みリソース
	LoadedData loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	// コピーコンストラクタも同様
	ResourceManager(const ResourceManager&);
	// デストラクタも同様
	~ResourceManager(void) = default;



	//内部でのみ使用する関数
	// 内部ロード
	std::weak_ptr<Resource> _Load(const SRC& src);

};

