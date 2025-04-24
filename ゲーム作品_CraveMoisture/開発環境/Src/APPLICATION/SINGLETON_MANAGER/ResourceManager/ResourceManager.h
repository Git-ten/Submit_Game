#pragma once
#include <map>
#include "../../Common/DataLoad/DataPaths.h"
#include "Resource.h"

class ResourceManager
{
public:

	// リソースのID
	enum class RESOURCE_ID
	{
		NONE,
		TITLE_BACK,								// タイトル背景
		TITLE_NAME,								// タイトル名
		PUSH_ANY_BUTTON,						// PushAnyButton画像
		SHUT_DOWN,								// シャットダウン画像
		SHUT_DOWN_DELETE,						// シャットダウンキー画像
		LOAD_BACK,								// ロード背景
		LOAD_GAGE,								// ロードゲージ
		LOAD_GAGE_CASE,							// ロードゲージのケース
		GAMEOVER_BACK,							// ゲームオーバー背景
		NUMBERS,								// 数字画像
		NUMBERS_RED,							// 数字画像(灰)
		SYMBOLS,								// 記号画像
		
		MENU_CASE,								// メニュー枠
		TUTORIAL_QUESTION,						// チュートリアルに進みますか
		YES,									// はい
		NO,										// いいえ
		SELECT_CIRCLE,							// 選択中を示す円
		PAUSE_GAME_BACK,						// 一時停止の背景
		PAUSE_RETURN_TITLE,						// 「タイトルに戻る文字」
		PAUSE_RETURN_GAME,						// 「ゲームを再開する文字」
		RESULT_TIME,							// Result画面の「Time」画像
		RESULT_DAMAGE,							// Result画面の「Damage」画像

		SELECT_LEVEL_BACK,						// 難易度セレクト画面の背景
		LEVEL_EASY,								// 難易度::EASY
		LEVEL_NORMAL,							// 難易度::NORMAL
		LEVEL_HARD,								// 難易度::HARD
		SELECT_ENEMY_EVILGOA,					// セレクト画面の「イビルゴア」イラスト

		SKYDOME_NIGHT,							// スカイドーム::夜
		SKYDOME_SUN,							// スカイドーム::快晴
		SKYDONUT_SUN,							// スカイドーナツ::快晴
		DEFAULT_JET,							// デフォルト戦闘機
		ENEMY_DEFAULT_JET,						// デフォルト敵戦闘機
		ENEMY_MOB_01,							// モブエネミー01
		ENEMY_MOB_02,							// モブエネミー01
		ENEMY_BEE_BOSS,							// 蜂型エネミー

		// プレイヤーモデル
		PLAYER_HERO,							// プレイヤー::ヒーロー

		// エネミーモデル
		ENEMY_DEMON,							// エネミー::デーモン
		ENEMY_BEAR,								// エネミー::ベア

		// ステージモデル
		STAGE_RUINS,							// ステージ::廃墟
		GATE_RUINS,								// エントランス::廃墟用
		STAGE_GROUND,							// ステージ::大地

		// 武器モデル
		HERO_SWORD,								// プレイヤーの剣

		// UI
		PLAYER_HP,								// プレイヤーUI::HP
		PLAYER_HP_CASE,							// プレイヤーUI::HPのケース
		ENEMY_HP,								// エネミーUI::HP
		ENEMY_HP_CASE,							// エネミーUI::HPのケース
		DOWN_GAGE,								// ダウンゲージ
		DOWN_GAGE_CASE,							// ダウンゲージのケース
		REPAIR_ICON,							// 回復テクニック技のアイコン
		ICON_CASE,								// テクニック技のアイコンケース
		EVIL_GOA_NAME,							// 「イビルゴア」の名前画像
		SKILL_GAGE,								// プレイヤーUI::スキルゲージ
		SKILL_GAGE_MAX,							// プレイヤーUI::スキルゲージ
		SKILL_GAGE_CASE,						// プレイヤーUI::スキルゲージケース
		WILD_BEAR_NAME,							// 「ワイルドベア」の名前画像

		// シェーダに使用するテクスチャ
		SPHERE_METAL,							// メタルスフィアマップ
		SWORD_METALIC,							// 剣用のメタリックテクスチャ
		SWORD_ROUGHNESS,						// 剣用のラフネステクスチャ
		ROUGHNESS,								// 通常のラフネステクスチャ
		RUINS_ROUGHNESS,						// 廃墟ラフネステクスチャ
		HERO_ROUGHNESS,							// プレイヤー::ヒーローラフネステクスチャ
		GROUND_ROUGHNESS,						// 草むらラフネステクスチャ
		SWORD_LOCUS_TEXTURE,					// 剣のテクスチャ
		NOISE,									// ノイズテクスチャ
		CLAW_TEXTURE,							// 爪のテクスチャ

		// 動画
		RESULT_MOVIE_HERO,						// プレイヤー::ヒーローのリザルト動画


		// チュートリアル用
		DELETE_TUTORIAL,						// 消去ボタン
		TUTORIAL_UI,							// UIチュートリアル画像
		TUTORIAL_CAMERA,						// カメラチュートリアル
		TUTORIAL_MOVE,							// 移動チュートリアル
		TUTORIAL_JUMP,							// ジャンプチュートリアル
		TUTORIAL_AVOID,							// 回避チュートリアル
		TUTORIAL_ATTACK,						// 攻撃チュートリアル
		TUTORIAL_DASH,							// ダッシュチュートリアル
		TUTORIAL_CHARGE,						// チャージ攻撃チュートリアル
		TUTORIAL_SPECIAL,						// パリィとジャスト回避のチュートリアル
		TUTORIAL_JUST_AVOID_ATTACK,				// ジャスト回避後の特殊チャージ攻撃
		TUTORIAL_TECHNIQUE,						// テクニック技チュートリアル
		TUTORIAL_SKILL,							// 必殺技チュートリアル
		TUTORIAL_BATTLE,						// メインバトルチュートリアル
		TUTORIAL_END,							// チュートリアル終了
		TUTORIAL_CAMERA_MOVIE,					// カメラチュートリアル動画
		TUTORIAL_MOVE_MOVIE,					// 移動チュートリアル動画
		TUTORIAL_JUMP_MOVIE,					// ジャンプチュートリアル動画
		TUTORIAL_AVOID_MOVIE,					// 回避チュートリアル動画
		TUTORIAL_ATTACK_MOVIE,					// 攻撃チュートリアル動画
		TUTORIAL_DASH_MOVIE,					// ダッシュチュートリアル動画
		TUTORIAL_CHARGE_MOVIE,					// チャージチュートリアル動画
		TUTORIAL_SPECIAL_MOVIE,					// パリィとジャスト回避チュートリアル動画
		TUTORIAL_JUST_AVOID_ATTACK_MOVIE,		// パリィとジャスト回避チュートリアル動画
		TUTORIAL_TECHNIQUE_MOVIE,				// テクニック技チュートリアル動画
		TUTORIAL_SKILL_MOVIE,					// 必殺技チュートリアル動画
		TUTORIAL_BATTLE_MOVIE,					// メインバトルチュートリアル動画
		TUTORIAL_MOUSE,							// チュートリアル用のマウス画像
		TUTORIAL_WASD_KEY,						// チュートリアル用のWASDキー画像
		TUTORIAL_R_KEY,							// チュートリアル用のRキー画像
		TUTORIAL_Q_KEY,							// チュートリアル用のQキー画像
		TUTORIAL_SPACE_KEY,						// チュートリアル用のSpaceキー画像
		TUTORIAL_ALL_KEY,						// チュートリアル用のすべてのキー画像

		// コントローラ
		TUTORIAL_D_PAD,							// チュートリアル用の十字キー画像
		TUTORIAL_CONTOROLLER,					// チュートリアル用のコントローラ画像
		TUTORIAL_X_BUTTON,						// チュートリアル用のXキー画像
		TUTORIAL_A_BUTTON,						// チュートリアル用のAキー画像
		TUTORIAL_B_BUTTON,						// チュートリアル用のBキー画像

		TUTORIAL_CAMERA_EXPLAIN,				// チュートリアルカメラの説明文
		TUTORIAL_MOVE_EXPLAIN,					// チュートリアル移動の説明文
		TUTORIAL_JUMP_EXPLAIN,					// チュートリアルジャンプの説明文
		TUTORIAL_AVOID_EXPLAIN,					// チュートリアル回避の説明文
		TUTORIAL_ATTACK_EXPLAIN,				// チュートリアル攻撃の説明文
		TUTORIAL_DASH_EXPLAIN,					// チュートリアルダッシュの説明文
		TUTORIAL_CHARGE_EXPLAIN,				// チュートリアルチャージの説明文
		TUTORIAL_SPECIAL_EXPLAIN,				// チュートリアルパリィとジャスト回避の説明文
		TUTORIAL_TECHNIQUE_EXPLAIN,				// チュートリアルテクニック技の説明文
		TUTORIAL_SKILL_EXPLAIN,					// チュートリアル必殺技の説明文
		TUTORIAL_BATTLE_EXPLAIN,				// チュートリアルバトルの説明文
		TUTORIAL_JUST_AVOID_ATTACK_EXPLAIN,		// チュートリアルジャスト回避時の特殊攻撃の説明文

	};

	//インスタンスの生成
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);
	// リソースの完全破棄
	void Destroy(void);

	// ロードして、ハンドルIDを取得
	Resource Load(const RESOURCE_ID& id);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(const RESOURCE_ID& id);

	// NULLであるか
	static const bool IsNullptr(void);

private:

	// リソースデータ
	using ResourcesData = std::map<RESOURCE_ID, Resource>;
	// ロード済みデータ
	using LoadedData = std::map<RESOURCE_ID, Resource>;

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	ResourcesData resourcesMap_;

	// 読み込み済みリソース
	LoadedData loadedMap_;

	// 画像の追加(キーボード)
	void AddImageKeboard(const DataPaths& dataPath);
	// 画像の追加(コントローラ用)
	void AddImageController(const DataPaths& dataPath);

	// privateでコンストラクタを生成する
	ResourceManager(void);
	// コピーコンストラクタも同様
	ResourceManager(const ResourceManager&);
	//デストラクタ
	~ResourceManager(void) = default;


	// 内部ロード
	Resource _Load(const RESOURCE_ID& id);
};